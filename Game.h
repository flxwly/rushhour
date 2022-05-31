#ifndef RUSHHOUR_GAME_H
#define RUSHHOUR_GAME_H


#include <SFML/Graphics/RenderWindow.hpp>
#include "Car.hpp"
#include "GUI.h"
#include "LevelHandler.h"

class Game {
public:
    Game(sf::Vector2i size, sf::Font &font);

    void update();
    void render();

    bool isRunning() const { return window.isOpen(); }

private:
    sf::RenderWindow window;
    Car *selectedCar = nullptr;

    sf::Clock blinker;
    bool blink = false;

    // GUI
    GUI gui;

    // Font
    sf::Font font;

    // Game content
    std::vector<Car> cars;
    std::vector<std::vector<Car *>> board;
};


#endif //RUSHHOUR_GAME_H
