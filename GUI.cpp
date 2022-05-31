#include <iostream>
#include "GUI.h"

GUI::GUI(sf::Vector2f size, sf::Font &font) : size(size), font(font) {

    // TODO: Create gui with certain size
    sf::Vector2f scale = sf::Vector2f (size.x / 50, size.y / 50);

    // close button in the top right corner
    close = Button(sf::Vector2f(40 * scale.x, 0), sf::Vector2f(10 * scale.x, 10 * scale.y), sf::Color::Red, sf::Color::Magenta, sf::Color::Green,
                   sf::Color::Blue, "X", font);

    // level select button in the bottom middle
    levelSelect = Button(sf::Vector2f(15 * scale.x, 40 * scale.y), sf::Vector2f(20 * scale.x, 10 * scale.y), sf::Color::Red, sf::Color::Magenta,
                         sf::Color::Green, sf::Color::Blue, "Level Select", font);
    play = Button(sf::Vector2f(15 * scale.x, 40 * scale.y), sf::Vector2f(20 * scale.x, 10 * scale.y), sf::Color::Red, sf::Color::Magenta, sf::Color::Green,
                  sf::Color::Blue, "Play", font);

    nextLevel = Button(sf::Vector2f(35 * scale.x, 40 * scale.y), sf::Vector2f(10 * scale.x, 10 * scale.y), sf::Color::Red, sf::Color::Magenta, sf::Color::Green,
                       sf::Color::Blue, ">", font);
    previousLevel = Button(sf::Vector2f(5 * scale.x, 40 * scale.y), sf::Vector2f(10 * scale.x, 10 * scale.y), sf::Color::Red, sf::Color::Magenta,
                           sf::Color::Green, sf::Color::Blue, "<", font);

    std::cout << "GUI created" << std::endl;

    update();
}

void GUI::update() {

    switch (showState) {
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
        showState = 0;
    } else if (levelSelect.isPressed()) {
        showState = 1;
    } else if (levelEditor.isPressed()) {
        showState = 2;
    } else if (play.isPressed()) {
        showState = 3;
    } else if (nextLevel.isPressed()) {
        curLevel = (curLevel + 1) % LEVEL_COUNT;
        showState = 1;
    } else if (previousLevel.isPressed()) {
        curLevel = (curLevel - 1) % LEVEL_COUNT;
        showState = 1;
    }
}

void GUI::draw(sf::RenderTarget &target, sf::RenderStates states) const {

    switch (showState) {
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
