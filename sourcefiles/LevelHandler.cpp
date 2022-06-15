#include <cmath>
#include <random>
#include "LevelHandler.h"

bool
LevelHandler::loadLevel(std::vector<Car> *cars, std::vector<std::vector<Car *>> *board, const std::string &levelName) {

    rapidjson::Document document;
    std::string filePath = "../levels/";
    if (levelName.find("original") != std::string::npos) {
        filePath += "originals.json";
    } else {
        filePath += "usermade.json";
    }

    std::ifstream file(filePath);
    std::string json((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());

    document.Parse(json.c_str());

    board->clear();
    if (!document.HasMember(levelName.c_str())) {
        return false;
    }

    rapidjson::Value &level = document[levelName.c_str()];

    for (int i = 0; i < level["BoardSize"]["x"].GetInt(); i++) {
        board->push_back(std::vector<Car *>());

        for (int j = 0; j < level["BoardSize"]["y"].GetInt(); j++) {
            board->at(i).push_back(nullptr);
        }
    }

    cars->clear();

    for (auto &car: level["Cars"].GetArray()) {
        std::vector<sf::Vector2i> positions;

        for (auto &position: car["occupiedPositions"].GetArray()) {
            positions.emplace_back(position["x"].GetInt(), position["y"].GetInt());
        }

        bool movesHorizontally = car["movesHorizontally"].GetBool();
        bool movesVertically = car["movesVertically"].GetBool();

        sf::Color color(car["color"]["r"].GetInt(), car["color"]["g"].GetInt(), car["color"]["b"].GetInt());

        sf::Vector2i outDir = {car["outDir"]["x"].GetInt(), car["outDir"]["y"].GetInt()};

        cars->push_back(Car(positions, movesHorizontally, movesVertically, color, outDir));
    }

    for (auto &car: *cars) {
        for (auto position: car.getOccupiedPositions()) {
            board->at(position.x)[position.y] = &car;
        }
    }

    // std::cout << "Loaded " << cars->size() << " cars" << std::endl;
    return true;
}

void LevelHandler::exportLevel(const std::vector<Car> &cars, const std::vector<std::vector<Car *>> &board,
                               const std::string &levelName) {

    std::cout << "Exporting level..." << std::endl;

    rapidjson::Document document;
    std::string filePath = (levelName.find("original") != std::string::npos) ? "../levels/originals.json"
                                                                             : "../levels/usermade.json";
    std::ifstream file(filePath);
    std::string json((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());
    document.Parse(json.c_str());

    rapidjson::Value level(rapidjson::kObjectType);

    // Board size
    rapidjson::Value boardSize(rapidjson::kObjectType);
    boardSize.AddMember("x", board.size(), document.GetAllocator());
    boardSize.AddMember("y", board.at(0).size(), document.GetAllocator());
    level.AddMember("BoardSize", boardSize, document.GetAllocator());

    // Cars
    rapidjson::Value carsArray(rapidjson::kArrayType);
    for (const auto &car: cars) {
        carsArray.PushBack(carToJsonObject(car, document.GetAllocator()), document.GetAllocator());
    }

    level.AddMember("Cars", carsArray, document.GetAllocator());

    if (document.HasMember(levelName.c_str())) {
        document.RemoveMember(levelName.c_str());
    }
    document.AddMember(rapidjson::Value(levelName.c_str(), document.GetAllocator()), level, document.GetAllocator());

    std::ofstream ofs(filePath);
    ofs.clear();
    rapidjson::OStreamWrapper osw(ofs);
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
    document.Accept(writer);
    ofs.close();
}

rapidjson::Value LevelHandler::carToJsonObject(const Car &car, rapidjson::Document::AllocatorType &allocator) {

    rapidjson::Value carObject(rapidjson::kObjectType);

    // occupiedPositions
    rapidjson::Value dataArray(rapidjson::kArrayType);
    for (const auto &pos: car.getOccupiedPositions()) {
        rapidjson::Value data(rapidjson::kObjectType);
        data.AddMember("x", pos.x, allocator);
        data.AddMember("y", pos.y, allocator);
        dataArray.PushBack(data, allocator);
    }
    carObject.AddMember("occupiedPositions", dataArray, allocator);

    // moving directions
    carObject.AddMember("movesHorizontally", car.canMoveHorizontally(), allocator);
    carObject.AddMember("movesVertically", car.canMoveVertically(), allocator);

    // outDir
    rapidjson::Value outDir(rapidjson::kObjectType);
    outDir.AddMember("x", car.getOutDir().x, allocator);
    outDir.AddMember("y", car.getOutDir().y, allocator);
    carObject.AddMember("outDir", outDir, allocator);

    // color
    rapidjson::Value color(rapidjson::kObjectType);
    color.AddMember("r", car.getColor().r, allocator);
    color.AddMember("g", car.getColor().g, allocator);
    color.AddMember("b", car.getColor().b, allocator);
    carObject.AddMember("color", color, allocator);

    return carObject;
}

void LevelHandler::generateLevel(const std::string &levelName) {

    sf::Image image;
    image.loadFromFile("../levels/images/" + levelName + ".png");

    std::vector<std::vector<Car *>> board(image.getSize().x, std::vector<Car *>(image.getSize().y));
    std::vector<Car> cars;

    // Cars
    std::vector<std::pair<std::vector<sf::Vector2i>, sf::Color>> occupiedPositions;
    for (int y = 0; y < image.getSize().y; y += 1) {
        for (int x = 0; x < image.getSize().x; x += 1) {
            sf::Color color = image.getPixel(x, y);
            if (color == sf::Color::White || color == sf::Color::Transparent) {
                continue;
            }

            auto it = std::find_if(occupiedPositions.begin(), occupiedPositions.end(),
                                   [&color](std::pair<std::vector<sf::Vector2i>, sf::Color> &c) {
                                       return c.second == color;
                                   });
            if (it == occupiedPositions.end()) {
                occupiedPositions.emplace_back(std::vector<sf::Vector2i>(1, sf::Vector2i(x, y)), color);
            } else {
                it->first.emplace_back(x, y);
            }

        }
    }

    for (const auto &car: occupiedPositions) {

        bool movesHorizontally = doesCarMoveHorizontally(car.first);
        bool movesVertically = !movesHorizontally;

        sf::Vector2i outDir = {0, 0};

        cars.emplace_back(car.first, movesHorizontally, movesVertically, car.second, outDir);

        std::cout << "Car: " << car.first.size() << " positions" << std::endl;
    }

    for (int i = 0; i < cars.size(); ++i) {
        if (cars.at(i).getColor() == sf::Color::Red) {
            std::swap(cars.at(i), cars.at(0));
        }
    }

    exportLevel(cars, board, levelName);
    std::cout << "Done..." << std::endl;
}

void LevelHandler::generateRandom(int sizeX, int sizeY, int carNum, const std::string &levelName) {
    std::vector<std::vector<Car *>> board(sizeX, std::vector<Car *>(sizeY));
    std::vector<Car> cars;

    std::vector<sf::Vector2i> possiblePositions(sizeX * sizeY);
    for (int i = 0; i < sizeX; ++i) {
        for (int j = 0; j < sizeY; ++j) {
            if (j != std::floor(sizeY / 2)) {
                possiblePositions.emplace_back(i, j);
            }
        }
    }

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> randX(1, sizeX - 2); // define the range
    std::uniform_int_distribution<> randY(1, sizeY - 2); // define the range
    std::uniform_int_distribution<> randColor(0, 255); // define the range

    sf::Vector2i pos_ = sf::Vector2i (randX(gen), std::floor(sizeY / 2));
    sf::Vector2i outDir_ = (pos_.x < std::floor(sizeX / 2)) ? sf::Vector2i(1, 0) : sf::Vector2i(-1, 0);

    cars.push_back(Car({pos_, pos_ + sf::Vector2i (1, 0)}, true, false, sf::Color::Red, outDir_));

    for (int i = 0; i < carNum; ++i) {
        std::vector<sf::Vector2i> car = generateFittingCar(possiblePositions);
        if (!car.empty()) {
            possiblePositions.erase(std::remove_if(possiblePositions.begin(), possiblePositions.end(),
                                                   [&car](const sf::Vector2i &pos) {
                                                       return std::find(car.begin(), car.end(), pos) != car.end();
                                                   }), possiblePositions.end());
            sf::Color color = sf::Color(randColor(gen), randColor(gen), randColor(gen));
            bool movesHorizontally = doesCarMoveHorizontally(car);

            cars.push_back(Car(car, movesHorizontally, !movesHorizontally, color, {0, 0}));
        }
    }

    for (auto &car : cars) {
        for (auto pos : car.getOccupiedPositions()) {
            board[pos.x][pos.y] = &car;
        }
    }

    int counter = 0;
    for (int i = 0; i < 18; ++i) {
        for (int index = 1; index < cars.size(); ++index) {
            std::uniform_int_distribution<> randTurns(1, 8);
            std::uniform_int_distribution<> randBool(0, 1);
            int turns = randTurns(gen);
            sf::Vector2i dir = (cars.at(index).canMoveVertically()) ? sf::Vector2i (0, (randBool(gen)) ? 1 : -1) : sf::Vector2i ((randBool(gen)) ? 1 : -1, 0);
            for (int j = 0; j < turns; ++j) {
                counter += cars.at(index).move(dir, board) ? 1 : 0;
            }
        }
    }
    std::cout << "Moved: " << counter << std::endl;
    exportLevel(cars, board, levelName);
}

std::vector<sf::Vector2i>
LevelHandler::generateFittingCar(std::vector<sf::Vector2i> possiblePositions) {
    sf::Vector2i position = possiblePositions.at(rand() % possiblePositions.size());
    int length = 2 + rand() % 2;
    bool horizontal = rand() % 2;

    if (possiblePositions.size() < length) {
        if (possiblePositions.size() < 2) {
            return {};
        } else {
            return generateFittingCar(possiblePositions);
        }
    }

    if (doesCarFit(position, length, horizontal, possiblePositions)) {
        return generateCar(position, length, horizontal);
    } else if (doesCarFit(position, length, !horizontal, possiblePositions)) {
        return generateCar(position, length, !horizontal);
    } else if (doesCarFit(position, -length, horizontal, possiblePositions)) {
        return generateCar(position, -length, horizontal);
    } else if (doesCarFit(position, -length, !horizontal, possiblePositions)) {
        return generateCar(position, -length, !horizontal);
    } else {
        return generateFittingCar(possiblePositions);
    }
}

bool
LevelHandler::doesCarFit(sf::Vector2i position, int length, bool horizontal,
                         std::vector<sf::Vector2i> possiblePositions) {
    std::vector<sf::Vector2i> car = generateCar(position, length, horizontal);
    return !std::any_of(car.begin(), car.end(), [&possiblePositions](sf::Vector2i &position) {
        return std::find(possiblePositions.begin(), possiblePositions.end(), position) == possiblePositions.end();
    });
}

std::vector<sf::Vector2i> LevelHandler::generateCar(sf::Vector2i pos, int length, bool horizontal) {
    std::vector<sf::Vector2i> car;
    for (int i = std::min(length, 0); i < std::max(length, 0); ++i) {
        if (horizontal) {
            car.emplace_back(pos.x + i, pos.y);
        } else {
            car.emplace_back(pos.x, pos.y + i);
        }
    }
    return car;
}

bool LevelHandler::doesCarMoveHorizontally(std::vector<sf::Vector2i> occupiedPositions) {

    if (occupiedPositions.empty()) {
        return false;
    }

    int xMax = 0;
    int yMax = 0;
    int xMin = occupiedPositions.at(0).x;
    int yMin = occupiedPositions.at(0).y;

    for (auto position: occupiedPositions) {
        xMax = std::max(xMax, position.x);
        yMax = std::max(yMax, position.y);
        xMin = std::min(xMin, position.x);
        yMin = std::min(yMin, position.y);
    }

    return xMax - xMin > yMax - yMin;
}
