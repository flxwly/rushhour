#ifndef RUSHHOUR_GUI_H
#define RUSHHOUR_GUI_H

#include <SFML/Graphics.hpp>
#include "Button.h"
#include "GameSettings.hpp"

class GUI : public sf::Drawable {
public:
    GUI(sf::Vector2f size, sf::Font &font);

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

    int curLevel = 0;
    int getShowState() const { return showState; }
    int setShowState(int showState) { return this->showState = showState; }

private:
    sf::Font font;

    sf::Vector2f scale;
    int showState = 1;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //RUSHHOUR_GUI_H
