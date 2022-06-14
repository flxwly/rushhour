#ifndef RUSHHOUR_LEVELHANDLER_H
#define RUSHHOUR_LEVELHANDLER_H


#include <vector>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/ostreamwrapper.h>
#include "Car.hpp"

namespace LevelHandler {


    bool doesCarMoveHorizontally(std::vector<sf::Vector2i> occupiedPositions);
    bool doesCarFit(sf::Vector2i pos, int length, bool horizontal, std::vector<sf::Vector2i> possiblePositions);
    std::vector<sf::Vector2i> generateCar(sf::Vector2i pos, int length, bool horizontal);
    std::vector<sf::Vector2i> generateFittingCar(std::vector<sf::Vector2i> possiblePositions);

    void generateLevel(const std::string& levelName);
    void generateRandom(int sizeX, int sizeY, int carNum, const std::string& levelName);
    bool loadLevel(std::vector<Car> *cars, std::vector<std::vector<Car *>> *board, const std::string& levelName);
    void exportLevel(const std::vector<Car>& cars, const std::vector<std::vector<Car *>>& board, const std::string& levelName);
    rapidjson::Value carToJsonObject(const Car &car, rapidjson::Document::AllocatorType &allocator);

};


#endif //RUSHHOUR_LEVELHANDLER_H
