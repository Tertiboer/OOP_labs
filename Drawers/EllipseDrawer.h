#ifndef ELLIPSEDRAWER_H
#define ELLIPSEDRAWER_H

#include "ShapeDrawer.h"
#include "../Figures/Ellipse.h"

class EllipseDrawer : public ShapeDrawer {
public:
    void draw(const Shape* shape, sf::RenderWindow& window) override {
        const Ellipse* ellipse = dynamic_cast<const Ellipse*>(shape);
        if (!ellipse) return;
        
        sf::CircleShape circle(static_cast<float>(ellipse->getRadiusX()));
        Point pos = ellipse->getPosition();
        
        circle.setPosition(sf::Vector2f(
            static_cast<float>(pos.x - ellipse->getRadiusX()), 
            static_cast<float>(pos.y - ellipse->getRadiusY())
        ));
        
        circle.setFillColor(ellipse->getColor());
        
        if (ellipse->getRadiusX() != ellipse->getRadiusY()) {
            float scaleY = static_cast<float>(ellipse->getRadiusY()) / 
                          static_cast<float>(ellipse->getRadiusX());
            // SFML ожидает Vector2f для setScale
            circle.setScale(sf::Vector2f(1.0f, scaleY));
        }
        
        window.draw(circle);
    }
};

#endif
