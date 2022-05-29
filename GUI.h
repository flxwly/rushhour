#ifndef RUSHHOUR_GUI_H
#define RUSHHOUR_GUI_H


#include "Button.h"
#include <SFML/Graphics.hpp>

class GUI : public sf::Drawable {
public:
    explicit GUI(sf::Vector2f size);

    void update();

    const sf::Vector2f size = sf::Vector2f(50, 50);

    Button close;

    Button levelEditor;
    Button save;
    Button load;

    Button levelSelect;
    Button nextLevel;
    Button previousLevel;
    Button play;

    std::string levelName = "";

private:
    sf::Font font;

    sf::Vector2f scale;
    int showSate = 0;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //RUSHHOUR_GUI_H
