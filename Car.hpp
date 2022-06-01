#ifndef RUSHHOUR_CAR_HPP
#define RUSHHOUR_CAR_HPP

#include <iostream>
#include <utility>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Car.hpp"
#include "GameSettings.hpp"

class Car {

public:
    Car() = default;

    Car(const std::vector<sf::Vector2i> &occupiedPositions, bool movesHorizontally, bool movesVertically, sf::Color color, sf::Vector2i outDir);

    [[nodiscard]] std::vector<sf::Vector2i> getOccupiedPositions() const {return occupiedPositions;}

    [[nodiscard]] bool contains(sf::Vector2f position) const;

    bool canMoveInDirection(sf::Vector2i direction) const;

    bool canMove(sf::Vector2i direction, std::vector<std::vector<Car *>> &board);

    bool canFinish(sf::Vector2i direction, std::vector<std::vector<Car *>> &board);

    void move(sf::Vector2i direction, std::vector<std::vector<Car *>> &board);

    [[nodiscard]] sf::Color getColor() const {return color;}

    [[nodiscard]] sf::Vector2i getOutDir() const {return outDir;}

    std::vector<sf::Vector2i> getOutPositions(std::vector<std::vector<Car *>> &board);

    [[nodiscard]] bool canMoveHorizontally() const {return movesHorizontally;}

    [[nodiscard]] bool canMoveVertically() const {return movesVertically;}

private:

    bool movesHorizontally = false;
    bool movesVertically = false;
    sf::Vector2i outDir = sf::Vector2i(0, 0);

    sf::Color color = sf::Color::White;
    std::vector<sf::Vector2i> occupiedPositions = {};
};


#endif //RUSHHOUR_CAR_HPP
