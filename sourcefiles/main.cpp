#include "Game.h"

int main() {

    // Die Schriftart wird geladen
    sf::Font font;
    font.loadFromFile("../fonts/arial.ttf");

    // Ein neues Game-Objekt wird erstellt.
    Game game(sf::Vector2i(WINDOW_WIDTH, WINDOW_HEIGHT), font);

    // Die update-Loop vom Game
    while (game.isRunning()) {
        game.update();
        game.render();
    }
}
