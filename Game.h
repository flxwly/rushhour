#ifndef RUSHHOUR_GAME_H
#define RUSHHOUR_GAME_H


#include <SFML/Graphics/RenderWindow.hpp>
#include "Car.hpp"
#include "LevelHandler.h"
#include "Button.h"

class Game {
public:
    Game(sf::Vector2i size, sf::Font &font);

    void update();
    void render();

    bool isRunning() const { return window.isOpen(); }

private:

    void renderButtons();
    void renderCars();
    void renderBackground();

    sf::RenderWindow window;

    // animations
    sf::Clock blinker;
    bool blink = false;

    // Buttons
    sf::Font font;
    std::map<std::string, Button> buttons;

    // Game content
    std::vector<Car> cars;
    std::vector<std::vector<Car *>> board;
    Car *selectedCar = nullptr;
    int currentLevel = 0;
};


#endif //RUSHHOUR_GAME_H
