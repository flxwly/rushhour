//
// Created by flxwl on 20.05.2022.
//

#include <iostream>
#include <utility>
#include "Car.hpp"

Car::Car(std::vector<sf::Vector2i> occupiedPositions, std::array<bool, 4> canMoveToDirection, sf::Color color,
         sf::Vector2i outDir) {
    this->occupiedPositions = std::move(occupiedPositions);
    this->movingDirections = canMoveToDirection;
    this->color = color;
    this->outDir = outDir;
}

void Car::move(sf::Vector2i direction, std::array<std::array<Car *, GRID_HEIGHT>, GRID_WIDTH> &board) {
    if (canMove(direction, board)) {
        for (auto &position: occupiedPositions) {
            board[position.x][position.y] = nullptr;
            position = position + direction;
        }
        for (auto position: occupiedPositions) {
            board[position.x][position.y] = this;
        }
    } else if (canFinish(direction, board)) {
        for (auto &position: occupiedPositions) {
            board[position.x][position.y] = nullptr;
        }
        occupiedPositions.clear();
    }
}


bool Car::canFinish(sf::Vector2i direction, std::array<std::array<Car *, 6>, 6> &board) {
    if (!canMoveInDirection(direction)) {
        return false;
    }

    if (direction != outDir) {
        return false;
    }

    return std::any_of(occupiedPositions.begin(), occupiedPositions.end(), [&](sf::Vector2i position) {
        sf::Vector2i newPos = position + direction;
        return newPos.x < 0 || newPos.x >= GRID_WIDTH || newPos.y < 0 || newPos.y >= GRID_HEIGHT;
    });
}

bool Car::canMove(sf::Vector2i direction, std::array<std::array<Car *, GRID_WIDTH>, GRID_HEIGHT> &board) {
    if (!canMoveInDirection(direction)) {
        return false;
    }

    bool canMove = !std::any_of(occupiedPositions.begin(), occupiedPositions.end(), [&](const sf::Vector2i &position) {
        sf::Vector2i newPosition = position + direction;

        return newPosition.x < 0 || newPosition.x >= GRID_WIDTH
               || newPosition.y < 0 || newPosition.y >= GRID_HEIGHT
               || (board[newPosition.x][newPosition.y] && board[newPosition.x][newPosition.y] != this);
    });

    return canMove;
}


bool Car::canMoveInDirection(sf::Vector2i direction) {
    if (direction.x > 0) {
        return movingDirections[1];
    } else if (direction.x < 0) {
        return movingDirections[3];
    } else if (direction.y > 0) {
        return movingDirections[2];
    } else if (direction.y < 0) {
        return movingDirections[0];
    }
    return false;
}

std::string Car::toJsonString() const {
    std::string jsonString = "{\n";
    jsonString += "\"occupiedPositions\": [";
    for (auto &position: occupiedPositions) {
        jsonString += "{\"x\": " + std::to_string(position.x) + ", \"y\": " + std::to_string(position.y) + "}";
        if (&position != &occupiedPositions.back()) {
            jsonString += ", ";
        }
    }
    jsonString += "],\n";
    jsonString += "\"movingDirections\": [";
    for (auto &direction: movingDirections) {
        jsonString += (direction) ? "true" : "false";
        if (&direction != &movingDirections.back()) {
            jsonString += ", ";
        }
    }
    jsonString += "],\n";
    jsonString += "\"color\": ["
                  + std::to_string(color.r) + ", "
                  + std::to_string(color.g) + ", "
                  + std::to_string(color.b) + "],\n";

    jsonString += "\"outDir:\" {\"x\":" + std::to_string(outDir.x) + ", \"y\":" + std::to_string(outDir.y) + "}\n";

    jsonString += "}";

    return jsonString;
}
