#include <iostream>
#include <fstream>
#include <rapidjson/document.h>
#include "GameSettings.hpp"
#include "Car.hpp"

std::vector<Car> createCarsFromImage(sf::Image image) {
    std::vector<std::pair<std::vector<sf::Vector2i>, sf::Color>> occupiedPositions;
    for (int y = 0; y < image.getSize().y; y += 1) {
        for (int x = 0; x < image.getSize().x; x += 1) {
            sf::Color color = image.getPixel(x, y);
            if (color == sf::Color::White) {
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

    std::vector<Car> cars;
    cars.reserve(occupiedPositions.size());
    for (const auto &position: occupiedPositions) {
        cars.push_back(Car(position.first, {true, true, true, true}, position.second, sf::Vector2i(0, 0)));
    }

    return cars;
}

void writeStateOfTheGameToJson(const std::vector<Car> &cars) {
    std::string json = "{\"Cars\": [";
    for (const auto &car: cars) {
        json += car.toJsonString();
        if (&car != &cars.back()) {
            json += ",";
        }
    }
    json += "]}";

    std::ofstream file("game.json");
    file << json;
    file.close();
}

void readStateOfTheGameFromJson(std::vector<Car> *cars, const std::string &path) {
    cars->clear();
    std::ifstream file(path);
    std::string json((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());

    rapidjson::Document document;
    document.Parse(json.c_str());

    for (auto &car: document["Cars"].GetArray()) {
        std::vector<sf::Vector2i> positions;
        for (auto &position: car["occupiedPositions"].GetArray()) {
            positions.emplace_back(position["x"].GetInt(), position["y"].GetInt());
        }
        std::array<bool, 4> movingDirections{};
        for (int i = 0; i < 4; i++) {
            movingDirections[i] = car["movingDirections"][i].GetBool();
        }

        sf::Color color(car["color"][0].GetInt(), car["color"][1].GetInt(), car["color"][2].GetInt());
        sf::Vector2i outDir(car["outDir"]["x"].GetInt(), car["outDir"]["y"].GetInt());
        cars->push_back(Car(positions, movingDirections, color, outDir));
    }
}

int main() {

    std::vector<Car> cars;
//    sf::Image image;
//    image.loadFromFile("../resources/cars2.png");
//    cars = createCarsFromImage(image);
//    writeStateOfTheGameToJson(cars);
//    return 0;)
    readStateOfTheGameFromJson(&cars, "game.json");

    sf::Font font;
    font.loadFromFile("../resources/arial.ttf");


    std::array<std::array<Car *, GRID_HEIGHT>, GRID_WIDTH> board{};
    for (auto &car: cars) {
        for (auto &position: car.getOccupiedPositions()) {
            board[position.x][position.y] = &car;
        }
    }

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "It's rush hour!");
    Car *selectedCar = nullptr;

    sf::Clock blinker;
    bool blink = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i cell = sf::Vector2i((event.mouseButton.x - MARGIN_LEFT) * GRID_WIDTH / GAME_WIDTH,
                                                     (event.mouseButton.y - MARGIN_TOP) * GRID_HEIGHT / GAME_HEIGHT);

                    if (cell.x >= 0 && cell.x < GRID_WIDTH && cell.y >= 0 && cell.y < GRID_HEIGHT) {
                        if (board[cell.x][cell.y]) {
                            selectedCar = board[cell.x][cell.y];
                        } else {
                            selectedCar = nullptr;
                        }
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    if (selectedCar != nullptr) {
                        selectedCar->move({-1, 0}, board);
                    }
                }
                if (event.key.code == sf::Keyboard::Right) {
                    if (selectedCar != nullptr) {
                        selectedCar->move({1, 0}, board);
                    }
                }
                if (event.key.code == sf::Keyboard::Up) {
                    if (selectedCar != nullptr) {
                        selectedCar->move({0, -1}, board);
                    }
                }
                if (event.key.code == sf::Keyboard::Down) {
                    if (selectedCar != nullptr) {
                        selectedCar->move({0, 1}, board);
                    }
                }
            }
        }

        window.clear();
        sf::RectangleShape cell(sf::Vector2f(CELL_WIDTH, CELL_HEIGHT));
        //cell.setOutlineThickness(2);
        for (int x = 0; x < GRID_WIDTH; x += 1) {
            for (int y = 0; y < GRID_HEIGHT; y += 1) {
                cell.setPosition(MARGIN_LEFT + x * CELL_WIDTH, MARGIN_TOP + y * CELL_HEIGHT);
                cell.setFillColor(board[x][y] == nullptr ? sf::Color::White : board[x][y]->getColor());

                if (board[x][y] && board[x][y]->getOutDir() != sf::Vector2i(0, 0)) {
                    cell.setFillColor(sf::Color(255, 0, 0));
                }

                window.draw(cell);
            }
        }

        if (selectedCar) {
            if (blink) {
                for (auto segment: selectedCar->getOccupiedPositions()) {
                    cell.setPosition(MARGIN_LEFT + segment.x * CELL_WIDTH, MARGIN_TOP + segment.y * CELL_HEIGHT);
                    cell.setFillColor(sf::Color(255, 255, 255, 100));
                    window.draw(cell);
                }
            }

            if (selectedCar->getOutDir() != sf::Vector2i(0, 0)) {

                if (blink) {
                    cell.setFillColor(sf::Color(255, 0, 0));
                } else {
                    cell.setFillColor(sf::Color(255, 100, 100, 0));
                }

                if (selectedCar->getOutDir().x > 0) {
                    cell.setPosition(MARGIN_LEFT + GAME_WIDTH, MARGIN_TOP);
                    cell.setSize(sf::Vector2f(MARGIN_RIGHT, GAME_HEIGHT));
                    window.draw(cell);
                } else if (selectedCar->getOutDir().x < 0) {
                    cell.setPosition(0, MARGIN_TOP);
                    cell.setSize(sf::Vector2f(MARGIN_LEFT, GAME_HEIGHT));
                    window.draw(cell);
                }

                if (selectedCar->getOutDir().y > 0) {
                    cell.setPosition(MARGIN_LEFT, MARGIN_TOP + GAME_HEIGHT);
                    cell.setSize(sf::Vector2f(GAME_WIDTH, MARGIN_BOTTOM));
                    window.draw(cell);
                } else if (selectedCar->getOutDir().y < 0) {
                    cell.setPosition(MARGIN_LEFT, 0);
                    cell.setSize(sf::Vector2f(GAME_WIDTH, MARGIN_TOP));
                    window.draw(cell);
                }
            }
        }

        bool gameOver = true;
        for (const auto &car: cars) {
            if (car.getOutDir() != sf::Vector2i(0, 0) && !car.getOccupiedPositions().empty()) {
                gameOver = false;
            }
        }

        if (gameOver) {
            selectedCar = nullptr;
            sf::Text text("Du hast gewonnen!!!!", font, 50);
            text.setPosition(WINDOW_WIDTH / 2 - text.getLocalBounds().width / 2,
                             WINDOW_HEIGHT / 2 - text.getLocalBounds().height / 2);
            text.setOutlineThickness(4);

            if (blink) {
                text.setFillColor(sf::Color(255, 200, 50));
                text.setOutlineColor(sf::Color::White);
            } else {
                text.setFillColor(sf::Color::Red);
                text.setOutlineColor(sf::Color::Black);
            }

            window.draw(text);
        }


        if (blinker.getElapsedTime().asSeconds() > 0.5) {
            blink = !blink;
            blinker.restart();
        }

        window.display();
    }

}
