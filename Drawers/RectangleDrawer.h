#ifndef RECTANGLEDRAWER_H
#define RECTANGLEDRAWER_H

#include "ShapeDrawer.h"
#include "../Figures/Rectangle.h"

class RectangleDrawer : public ShapeDrawer {
public:
    void draw(const Shape* shape, sf::RenderWindow& window) override {
        const Rectangle* rect = dynamic_cast<const Rectangle*>(shape);
        if (!rect) return;
        
        sf::RectangleShape sfmlRect;
        Point pos = rect->getPosition();
        sfmlRect.setPosition(sf::Vector2f(pos.x, pos.y));
        sfmlRect.setSize(sf::Vector2f(rect->getWidth(), rect->getHeight()));
        sfmlRect.setFillColor(rect->getColor());
        
        window.draw(sfmlRect);
    }
};

#endif
