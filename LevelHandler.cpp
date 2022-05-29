#include "LevelHandler.h"

void
LevelHandler::loadLevel(std::vector<Car> *cars, std::vector<std::vector<Car *>> *board, const std::string &levelName) {

    rapidjson::Document document;
    if (levelName.find("originalLevel") != std::string::npos) {
        std::ifstream file("../levels/originals.json");
        std::string json((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
        document.Parse(json.c_str());
    } else {
        std::ifstream file("../levels/usermade.json");
        std::string json((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
        document.Parse(json.c_str());
    }

    auto &level = document[levelName.c_str()];
    board->clear();
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

        sf::Vector2i outDir = {0, 0};
        switch (car["outDir"].GetInt()) {
            case 0:
                outDir = sf::Vector2i(1, 0);
                break;
            case 1:
                outDir = sf::Vector2i(0, 1);
                break;
            case 2:
                outDir = sf::Vector2i(-1, 0);
                break;
            case 3:
                outDir = sf::Vector2i(0, -1);
                break;
            default:
                break;
        }

        cars->push_back(Car(positions, movesHorizontally, movesVertically, color, outDir));
    }
}

void LevelHandler::exportLevel(const std::vector<Car> &cars, const std::vector<std::vector<Car *>> &board,
                               const std::string &levelName) {

    std::cout << "Exporting level..." << std::endl;

    rapidjson::Document document;
    std::string filePath = (levelName.find("originalLevel") != std::string::npos) ? "../levels/originals.json"
                                                                                  : "../levels/usermade.json";
    std::ifstream file(filePath);
    std::string json((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());
    document.Parse(json.c_str());

    rapidjson::Value level(rapidjson::kObjectType);

    std::cout << "Parsed existing document" << std::endl;

    // Board size
    rapidjson::Value boardSize(rapidjson::kObjectType);
    boardSize.AddMember("x", board.size(), document.GetAllocator());
    boardSize.AddMember("y", board.at(0).size(), document.GetAllocator());
    level.AddMember("BoardSize", boardSize, document.GetAllocator());

    std::cout << "Added board size" << std::endl;

    // Cars
    rapidjson::Value carsArray(rapidjson::kArrayType);
    for (const auto &car: cars) {
        carsArray.PushBack(carToJsonObject(car), document.GetAllocator());
    }

    level.AddMember("Cars", carsArray, document.GetAllocator());
    std::cout << "Added cars" << std::endl;

    if (document.HasMember(levelName.c_str())) {
        document.RemoveMember(levelName.c_str());
    }
    document.AddMember(rapidjson::Value(levelName.c_str(), document.GetAllocator()), level, document.GetAllocator());

    std::ofstream ofs(filePath);
    rapidjson::OStreamWrapper osw(ofs);
    rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
    document.Accept(writer);
    ofs.close();
}

rapidjson::Value LevelHandler::carToJsonObject(const Car &car) {

    rapidjson::Document document;
    rapidjson::Document::AllocatorType &allocator = document.GetAllocator();

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
        int xMax = 0;
        int yMax = 0;
        int xMin = image.getSize().x;
        int yMin = image.getSize().y;

        for (auto position: car.first) {
            if (position.x > xMax) {
                xMax = position.x;
            }
            if (position.x < xMin) {
                xMin = position.x;
            }
            if (position.y > yMax) {
                yMax = position.y;
            }
            if (position.y < yMin) {
                yMin = position.y;
            }
        }

        bool movesHorizontally = xMax - xMin > yMax - yMin;
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
