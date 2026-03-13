#ifndef CIRCLEDRAWER_H
#define CIRCLEDRAWER_H

#include "ShapeDrawer.h"
#include "../Figures/Circle.h"

// Рисовальщик для круга
class CircleDrawer : public ShapeDrawer {
public:
    void draw(const Shape* shape, sf::RenderWindow& window) override {
        const Circle* circle = dynamic_cast<const Circle*>(shape);
        if (!circle) return;
        
        // Создаем круг SFML
        sf::CircleShape sfmlCircle(static_cast<float>(circle->getRadius()));
        Point pos = circle->getPosition();
        
        // Устанавливаем позицию (центр круга)
        sfmlCircle.setPosition(sf::Vector2f(
            static_cast<float>(pos.x - circle->getRadius()), 
            static_cast<float>(pos.y - circle->getRadius())
        ));
        
        sfmlCircle.setFillColor(circle->getColor());
        
        window.draw(sfmlCircle);
    }
};

#endif
