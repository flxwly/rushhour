#include "GUI.h"

GUI::GUI(sf::Vector2f size) {

    // TODO: Create gui with certain size

    // close button in the top right corner
    close = Button(sf::Vector2f(40, 0), sf::Vector2f(10, 10), sf::Color::Red, sf::Color::Magenta, sf::Color::Green,
                   sf::Color::Blue, "X", font);

    // level select button in the bottom middle
    levelSelect = Button(sf::Vector2f(15, 40), sf::Vector2f(20, 10), sf::Color::Red, sf::Color::Magenta,
                         sf::Color::Green, sf::Color::Blue, "Level Select", font);
    play = Button(sf::Vector2f(15, 40), sf::Vector2f(20, 10), sf::Color::Red, sf::Color::Magenta, sf::Color::Green,
                  sf::Color::Blue, "Play", font);

    nextLevel = Button(sf::Vector2f(35, 40), sf::Vector2f(10, 10), sf::Color::Red, sf::Color::Magenta, sf::Color::Green,
                       sf::Color::Blue, ">", font);
    previousLevel = Button(sf::Vector2f(5, 40), sf::Vector2f(10, 10), sf::Color::Red, sf::Color::Magenta,
                           sf::Color::Green, sf::Color::Blue, "<", font);

    update();
}

void GUI::update() {

    switch (showSate) {
        // show title screen
        case 0:
            levelSelect.update();
            levelEditor.update();
            break;

            // show level select
        case 1:
            play.update();
            nextLevel.update();
            previousLevel.update();
            break;

            // show level editor
        case 2:
            save.update();
            load.update();
            break;

            // show level
        case 3:
            break;

    }

    close.update();

    if (close.isPressed()) {
        showSate = 0;
    } else if (levelSelect.isPressed()) {
        showSate = 1;
    } else if (levelEditor.isPressed()) {
        showSate = 2;
    } else if (play.isPressed()) {
        showSate = 3;
    } else if (nextLevel.isPressed()) {

        showSate = 1;
    } else if (previousLevel.isPressed()) {
        showSate = 1;
    }
}

void GUI::draw(sf::RenderTarget &target, sf::RenderStates states) const {

    switch (showSate) {
        // show title screen
        case 0:
            target.draw(levelSelect);
            target.draw(levelEditor);
            break;

            // show level select
        case 1:
            target.draw(play);
            target.draw(nextLevel);
            target.draw(previousLevel);
            break;

            // show level editor
        case 2:
            target.draw(save);
            target.draw(load);
            break;

            // show level
        case 3:
            break;

    }

    target.draw(close);

}
