#ifndef RUSHHOUR_GAME_H
#define RUSHHOUR_GAME_H


#include <SFML/Graphics/RenderWindow.hpp>
#include "Car.hpp"
#include "GUI.h"
#include "LevelHandler.h"

class Game {
public:
    Game(int sizeX, int sizeY);

    void update();
    void render();

private:
    sf::RenderWindow window;
    Car *selectedCar = nullptr;

    sf::Clock blinker;
    bool blink = false;

    // GUI
    GUI gui;


    // Game content
    std::vector<Car> cars;
    std::vector<std::vector<Car *>> board;
};


#endif //RUSHHOUR_GAME_H
