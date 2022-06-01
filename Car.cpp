#include "Car.hpp"

Car::Car(const std::vector<sf::Vector2i> &occupiedPositions, bool movesHorizontally, bool movesVertically, sf::Color color,
         sf::Vector2i outDir) {
    this->occupiedPositions = occupiedPositions;
    this->movesHorizontally = movesHorizontally;
    this->movesVertically = movesVertically;
    this->color = color;
    this->outDir = outDir;
}

void Car::move(sf::Vector2i direction, std::vector<std::vector<Car *>> &board) {
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


bool Car::canFinish(sf::Vector2i direction, std::vector<std::vector<Car *>> &board) {
    if (!canMoveInDirection(direction)) {
        return false;
    }

    if (direction != outDir) {
        return false;
    }

    return std::any_of(occupiedPositions.begin(), occupiedPositions.end(), [&](sf::Vector2i position) {
        sf::Vector2i newPos = position + direction;
        return newPos.x < 0 || newPos.x >= board.size() || newPos.y < 0 || newPos.y >= board[0].size() ||
               board[newPos.x][newPos.y] != nullptr;
    });
}

bool Car::canMove(sf::Vector2i direction, std::vector<std::vector<Car *>> &board) {
    if (!canMoveInDirection(direction)) {
        return false;
    }

    bool canMove = !std::any_of(occupiedPositions.begin(), occupiedPositions.end(), [&](const sf::Vector2i &position) {
        sf::Vector2i newPosition = position + direction;

        return newPosition.x < 0 || newPosition.x >= board.size()
               || newPosition.y < 0 || newPosition.y >= board[0].size()
               || (board[newPosition.x][newPosition.y] && board[newPosition.x][newPosition.y] != this);
    });

    return canMove;
}


bool Car::canMoveInDirection(sf::Vector2i direction) const {
    if (direction.x != 0) {
        return movesHorizontally;
    }

    if (direction.y != 0) {
        return movesVertically;
    }

    return false;
}

std::vector<sf::Vector2i> Car::getOutPositions(std::vector<std::vector<Car *>> &board) {
    if (occupiedPositions.empty() || outDir == sf::Vector2i(0, 0) || board.empty()) {
        return {};
    }

    std::vector<sf::Vector2i> outPositions;

    for (auto position: occupiedPositions) {

        sf::Vector2i newPosition = sf::Vector2i {
            std::max(-1, std::min(position.x + outDir.x * static_cast<int>(board.size()), static_cast<int>(board.size()))),
            std::max(-1, std::min(position.y + outDir.y * static_cast<int>(board[0].size()), static_cast<int>(board[0].size())))
        };
        if (std::find(outPositions.begin(), outPositions.end(), newPosition) == outPositions.end()) {
            outPositions.push_back(newPosition);
        }
    }
    return outPositions;
}
