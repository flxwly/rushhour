#ifndef RUSHHOUR_BUTTON_H
#define RUSHHOUR_BUTTON_H


#include <SFML/Graphics.hpp>
#include <string>

class Button : public sf::Drawable{
public:
    Button() = default;
    Button(sf::Vector2f position, sf::Vector2f size, sf::Color color, sf::Color hoverColor, sf::Color pressedColor, sf::Color textColor, std::string text, sf::Font font);

    void update();

    bool isHover() const { return hover; }
    bool isPressed() const { return pressed; }
private:
    sf::RectangleShape shape;
    sf::Font font;
    sf::Text text;
    sf::Color color;
    sf::Color hoverColor;
    sf::Color pressedColor;
    sf::Color textColor;

    bool hover = false;
    bool pressed = false;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //RUSHHOUR_BUTTON_H
