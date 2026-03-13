#ifndef SHAPEDRAWER_H
#define SHAPEDRAWER_H

#include <SFML/Graphics.hpp>
#include "../Figures/Shape.h"

// Базовый класс для всех рисовальщиков
class ShapeDrawer {
public:
    virtual ~ShapeDrawer() {}
    virtual void draw(const Shape* shape, sf::RenderWindow& window) = 0;
};

#endif
