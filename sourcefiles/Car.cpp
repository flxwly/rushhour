#include "Car.hpp"

Car::Car(const std::vector<sf::Vector2i> &occupiedPositions, bool movesHorizontally, bool movesVertically,
         sf::Color color,
         sf::Vector2i outDir) {
    this->occupiedPositions = occupiedPositions;
    this->movesHorizontally = movesHorizontally;
    this->movesVertically = movesVertically;
    this->color = color;
    this->outDir = outDir;
}

bool Car::move(sf::Vector2i direction, std::vector<std::vector<Car *>> &board) {

    // Bevor das Auto bewegt werden kann, muss überprüft werden, ob es sich überhaupt
    // in die gegebene Richtung bewegen kann.
    if (canMove(direction, board)) {

        // Bewege das Auto auf dem Brett, in dem alle belegten Plätze erst geleert werden
        // und dann die neuen auf das Auto gesetzt werden.
        for (auto &position: occupiedPositions) {
            board[position.x][position.y] = nullptr;

            // position = position + direction verändert den Wert in occupiedPositions, weil
            // position eine Referenz ist (wegen dem & vor dem Namen)
            position = position + direction;
        }
        for (auto position: occupiedPositions) {
            board[position.x][position.y] = this;
        }
        return true;

        // Wenn das Auto mit diesem Zug herauskommt und das auch soll,
        // dann wird es vom Brett genommen
    } else if (canFinish(direction, board)) {
        for (auto &position: occupiedPositions) {
            board[position.x][position.y] = nullptr;
        }
        occupiedPositions.clear();
        return true;
    }

    // Das Auto konnte nicht bewegt werden.
    return false;
}


bool Car::canFinish(sf::Vector2i direction, std::vector<std::vector<Car *>> &board) {
    // Wenn das Auto nicht in die Richtung kann, dann kann es auch nicht beenden
    if (!canMoveInDirection(direction)) {
        return false;
    }

    // Überprüft, ob das Auto in diese Richtung rauswill.
    // Wenn nicht, dann wird falsch zurückgegeben.
    if (direction != outDir) {
        return false;
    }

    // Für jedes Feld des Autos wird geprüft, ob es mit dem Zug außerhalb des Spielfeldes landen würde.
    return std::any_of(occupiedPositions.begin(), occupiedPositions.end(), [&](sf::Vector2i position) {
        sf::Vector2i newPos = position + direction;
        return newPos.x < 0 || newPos.x >= board.size() || newPos.y < 0 || newPos.y >= board[0].size();
    });
}

bool Car::canMove(sf::Vector2i direction, std::vector<std::vector<Car *>> &board) {
    // Wenn das Auto nicht in die Richtung kann, dann kann es sich auch nicht in die Richtung bewegen. Logisch!
    if (!canMoveInDirection(direction)) {
        return false;
    }

    // Es wird überprüft, ob eines der belegten Felder auf einem anderen Auto landen würde oder außerhalb
    // des Spielfeldes wäre, wenn es in die Richtung bewegt wird. Die Verneinung dazu gibt dann an, ob sich
    // das Auto in die Richtung bewegen kann.
    return !std::any_of(occupiedPositions.begin(), occupiedPositions.end(), [&](const sf::Vector2i &position) {
        sf::Vector2i newPosition = position + direction;

        return newPosition.x < 0 || newPosition.x >= board.size()
               || newPosition.y < 0 || newPosition.y >= board[0].size()
               || (board[newPosition.x][newPosition.y] && board[newPosition.x][newPosition.y] != this);
    });
}


bool Car::canMoveInDirection(sf::Vector2i direction) const {
    // Horizontale Bewegung heißt in x-Richtung, vertikale in y-Richtung
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

        sf::Vector2i newPosition = sf::Vector2i{
                std::max(-1, std::min(position.x + outDir.x * static_cast<int>(board.size()),
                                      static_cast<int>(board.size()))),
                std::max(-1, std::min(position.y + outDir.y * static_cast<int>(board[0].size()),
                                      static_cast<int>(board[0].size())))
        };
        if (std::find(outPositions.begin(), outPositions.end(), newPosition) == outPositions.end()) {
            outPositions.push_back(newPosition);
        }
    }
    return outPositions;
}
