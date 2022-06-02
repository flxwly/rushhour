#include <iostream>
#include "Button.h"

Button::Button(sf::Vector2f position, sf::Vector2f size, sf::Color color, sf::Color hoverColor, sf::Color pressedColor,
               sf::Color textColor, const std::string& text, const sf::Font& font) {

    standardShape.setPosition(position);
    standardShape.setSize(size);
    standardShape.setFillColor(color);

    hoverShape = standardShape;
    pressedShape = standardShape;

    hoverShape.setFillColor(hoverColor);
    pressedShape.setFillColor(pressedColor);

    this->text.setString(text);
    this->text.setFont(font);
    this->text.setFillColor(textColor);
    this->text.setCharacterSize(size.y);
    this->text.setPosition(position.x - this->text.getLocalBounds().width / 2 + size.x / 2,
                           position.y - this->text.getLocalBounds().height / 2 + size.y / 4);

}

bool Button::isHover(sf::Window &window) const{
    return standardShape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)));
}

bool Button::isPressed(sf::Window &window, sf::Event event) const {
    return isHover(window) && event.type == sf::Event::MouseButtonReleased;
}

bool Button::isPressed(sf::Window &window) const {
    return isHover(window) && sf::Mouse::isButtonPressed(sf::Mouse::Left);
}
