#include "Car.hpp"

Car::Car(std::vector<sf::Vector2i> occupiedPositions, bool movesHorizontally, bool movesVertically, sf::Color color,
         sf::Vector2i outDir) {
    this->occupiedPositions = std::move(occupiedPositions);
    this->movesHorizontally = movesHorizontally;
    this->movesVertically = movesVertically;
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


bool Car::canMoveInDirection(sf::Vector2i direction) const {
    if (direction.x != 0) {
        return movesHorizontally;
    }

    if (direction.y != 0) {
        return movesVertically;
    }

    return false;
}
