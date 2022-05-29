#include "Button.h"

Button::Button(sf::Vector2f position, sf::Vector2f size, sf::Color color, sf::Color hoverColor, sf::Color pressedColor,
               sf::Color textColor, std::string text, sf::Font font) {

    shape.setPosition(position);
    shape.setSize(size);

    color = color;
    hoverColor = hoverColor;
    pressedColor = pressedColor;
    textColor = textColor;
    text = text;
    font = font;

    update();
}

void Button::update() {
    if (sf::Mouse::getPosition().x > shape.getPosition().x &&
        sf::Mouse::getPosition().x < shape.getPosition().x + shape.getSize().x &&
        sf::Mouse::getPosition().y > shape.getPosition().y &&
        sf::Mouse::getPosition().y < shape.getPosition().y + shape.getSize().y) {

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            hover = false;
            pressed = true;
        } else {
            hover = true;
            pressed = false;
        }
    } else {
        hover = false;
        pressed = false;
    }

    if (hover) {
        shape.setFillColor(hoverColor);
    } else if (pressed) {
        shape.setFillColor(pressedColor);
    } else {
        shape.setFillColor(color);
    }
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {

    target.draw(text, states);
    if (pressed) {
        target.draw(shape, states);
    } else if (hover) {
        target.draw(shape, states);
    } else {
        target.draw(shape, states);
    }
}
