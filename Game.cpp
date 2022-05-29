#include "Game.h"

Game::Game(int sizeX, int sizeY) : window(sf::VideoMode(sizeX, sizeY), "Rush Hour"), gui(sf::Vector2f(sizeX, sizeY)) {
}

void Game::update() {
    gui.update();

    if (gui.close.isPressed()) {
        window.close();
    } else if (gui.play.isPressed()) {
        LevelHandler::loadLevel(&cars, &board, gui.levelName);
    } else if (gui.load.isPressed()) {

    } else if (gui.save.isPressed()) {

    }
}
