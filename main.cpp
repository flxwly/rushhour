#include "Game.h"

int main() {

    sf::Font font;
    font.loadFromFile("../fonts/arial.ttf");

    Game game(sf::Vector2i(WINDOW_WIDTH, WINDOW_HEIGHT), font);

    while (game.isRunning()) {
        game.update();
        game.render();
    }
}
