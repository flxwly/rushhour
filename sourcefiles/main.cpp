#include "Game.h"

int main() {

    // Zufällige Level generieren
    //    for (int i = 0; i < 20; ++i) {
    //        LevelHandler::generateRandom(20, 20, 120, "original" + std::to_string(i + 50));
    //    }

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
