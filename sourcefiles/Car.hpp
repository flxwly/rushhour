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

    // Der Konstruktor der Car-Klasse
    Car(const std::vector<sf::Vector2i> &occupiedPositions, bool movesHorizontally, bool movesVertically, sf::Color color, sf::Vector2i outDir);

    // Gibt alle von dem Auto belegten Felder in einer dynamischen Reihung zurück.
    [[nodiscard]] std::vector<sf::Vector2i> getOccupiedPositions() const {return occupiedPositions;}

    // Diese Methode testet, ob sich das Auto in eine Richtung bewegen kann.
    // Wichtig: Diese Methode beachtet nicht die Umgebung. Sie überprüft nur,
    // wie das Auto ausgerichtet ist.
    bool canMoveInDirection(sf::Vector2i direction) const;

    // Diese Methode überprüft wie auch "canMoveInDirection(...)", ob sich das
    // Auto in eine Richtung Bewegen kann. Hierbei wird jedoch noch zusätzlich
    // getestet, ob Autos im Weg sind.
    bool canMove(sf::Vector2i direction, std::vector<std::vector<Car *>> &board);

    // Hier wird überprüft, ob ein Auto mit der nächsten Bewegung vom Parkplatz
    // herunter kann. Dabei kann nur wahr zurück gegeben werden, wenn das Auto
    // auch wirklich vom Parkplatz "will".
    bool canFinish(sf::Vector2i direction, std::vector<std::vector<Car *>> &board);

    // Das Auto wird in die angegebene Richtung bewegt, wenn es möglich ist.
    // Wurde das Auto bewegt, gibt die Methode wahr zurück.
    bool move(sf::Vector2i direction, std::vector<std::vector<Car *>> &board);

    // Gibt die Farbe des Autos zurück
    [[nodiscard]] sf::Color getColor() const {return color;}

    // Gibt die Richtung aus, in die das Auto raus möchte.
    [[nodiscard]] sf::Vector2i getOutDir() const {return outDir;}

    // Berechnet die Felder, auf denen das Auto raus kann,
    std::vector<sf::Vector2i> getOutPositions(std::vector<std::vector<Car *>> &board);

    // Gibt zurück, ob sich das Auto horizontal bewegen kann
    [[nodiscard]] bool canMoveHorizontally() const {return movesHorizontally;}

    // Gibt zurück, ob sich das Auto vertikal bewegen kann
    [[nodiscard]] bool canMoveVertically() const {return movesVertically;}

private:
    bool movesHorizontally = false;
    bool movesVertically = false;
    sf::Vector2i outDir = sf::Vector2i(0, 0);

    sf::Color color = sf::Color::White;
    std::vector<sf::Vector2i> occupiedPositions = {};
};


#endif //RUSHHOUR_CAR_HPP
