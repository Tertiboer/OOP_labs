#ifndef TRIANGLEDRAWER_H
#define TRIANGLEDRAWER_H

#include "ShapeDrawer.h"
#include "../Figures/Triangle.h"

// Рисовальщик для треугольника
class TriangleDrawer : public ShapeDrawer {
public:
    void draw(const Shape* shape, sf::RenderWindow& window) override {
        const Triangle* triangle = dynamic_cast<const Triangle*>(shape);
        if (!triangle) return;
        
        // Получаем все точки треугольника
        std::vector<Point> points = triangle->getPoints();
        if (points.size() < 3) return;
        
        // Создаем выпуклую фигуру с 3 точками
        sf::ConvexShape convex;
        convex.setPointCount(3);
        
        // Устанавливаем точки
        convex.setPoint(0, sf::Vector2f(
            static_cast<float>(points[0].x), 
            static_cast<float>(points[0].y)
        ));
        convex.setPoint(1, sf::Vector2f(
            static_cast<float>(points[1].x), 
            static_cast<float>(points[1].y)
        ));
        convex.setPoint(2, sf::Vector2f(
            static_cast<float>(points[2].x), 
            static_cast<float>(points[2].y)
        ));
        
        convex.setFillColor(triangle->getColor());
        
        window.draw(convex);
    }
};

#endif
