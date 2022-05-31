#ifndef RUSHHOUR_LEVELHANDLER_H
#define RUSHHOUR_LEVELHANDLER_H


#include <vector>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/ostreamwrapper.h>
#include "Car.hpp"

namespace LevelHandler {

    void generateLevel(const std::string& levelName);
    void loadLevel(std::vector<Car> *cars, std::vector<std::vector<Car *>> *board, const std::string& levelName);
    void exportLevel(const std::vector<Car>& cars, const std::vector<std::vector<Car *>>& board, const std::string& levelName);
    rapidjson::Value carToJsonObject(const Car &car, rapidjson::Document::AllocatorType &allocator);

};


#endif //RUSHHOUR_LEVELHANDLER_H
