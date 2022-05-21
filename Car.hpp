#ifndef RUSHHOUR_CAR_HPP
#define RUSHHOUR_CAR_HPP

#include "GameSettings.hpp"

#define NORTH sf::Vector2i(0, -1);
#define EAST sf::Vector2i(1, 0);
#define SOUTH sf::Vector2i(0, 1);
#define WEST sf::Vector2i(-1, 0);


#include <vector>
#include <SFML/Graphics.hpp>

class Car {

public:
    Car() = default;

    Car(std::vector<sf::Vector2i> occupiedPositions, std::array<bool, 4> canMoveToDirection, sf::Color color, sf::Vector2i outDir);

    [[nodiscard]] std::vector<sf::Vector2i> getOccupiedPositions() const {return occupiedPositions;}

    [[nodiscard]] bool contains(sf::Vector2f position) const;

    bool canMoveInDirection(sf::Vector2i direction);

    bool canMove(sf::Vector2i direction, std::array<std::array<Car *, GRID_HEIGHT>, GRID_WIDTH> &board);

    bool canFinish(sf::Vector2i direction, std::array<std::array<Car *, GRID_HEIGHT>, GRID_WIDTH> &board);

    void move(sf::Vector2i direction, std::array<std::array<Car *, GRID_HEIGHT>, GRID_WIDTH> &board);

    [[nodiscard]] std::string toJsonString() const;

    [[nodiscard]] sf::Color getColor() const {return color;}

    [[nodiscard]] sf::Vector2i getOutDir() const {return outDir;}

private:
    std::array<bool, 4> movingDirections;
    std::vector<sf::Vector2i> occupiedPositions;
    sf::Color color;
    sf::Vector2i outDir = sf::Vector2i(0, -1);
};


#endif //RUSHHOUR_CAR_HPP
