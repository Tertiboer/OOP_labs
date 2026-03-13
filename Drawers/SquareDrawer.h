#ifndef SQUAREDRAWER_H
#define SQUAREDRAWER_H

#include "ShapeDrawer.h"
#include "../Figures/Square.h"

// Рисовальщик для квадрата
class SquareDrawer : public ShapeDrawer {
public:
    void draw(const Shape* shape, sf::RenderWindow& window) override {
        const Square* square = dynamic_cast<const Square*>(shape);
        if (!square) return;
        
        // Создаем прямоугольник (квадрат - частный случай)
        sf::RectangleShape sfmlSquare;
        Point pos = square->getPosition();
        
        // Устанавливаем позицию и размер
        sfmlSquare.setPosition(sf::Vector2f(
            static_cast<float>(pos.x), 
            static_cast<float>(pos.y)
        ));
        sfmlSquare.setSize(sf::Vector2f(
            static_cast<float>(square->getSide()), 
            static_cast<float>(square->getSide())
        ));
        
        sfmlSquare.setFillColor(square->getColor());
        
        window.draw(sfmlSquare);
    }
};

#endif
