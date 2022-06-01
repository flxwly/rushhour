#ifndef RUSHHOUR_BUTTON_H
#define RUSHHOUR_BUTTON_H


#include <SFML/Graphics.hpp>
#include <string>

class Button {
public:
    Button() = default;
    Button(sf::Vector2f position, sf::Vector2f size, sf::Color color, sf::Color hoverColor, sf::Color pressedColor, sf::Color textColor, const std::string& text, const sf::Font& font);

    bool isHover(sf::Window &window) const;
    bool isPressed(sf::Window &window, sf::Event event) const;
    bool isPressed(sf::Window &window) const;

    sf::RectangleShape standardShape;
    sf::RectangleShape hoverShape;
    sf::RectangleShape pressedShape;
    sf::Text text;
};


#endif //RUSHHOUR_BUTTON_H
