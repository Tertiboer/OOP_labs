#ifndef LINEDRAWER_H
#define LINEDRAWER_H

#include "ShapeDrawer.h"
#include "../Figures/Line.h"

class LineDrawer : public ShapeDrawer {
public:
    void draw(const Shape* shape, sf::RenderWindow& window) override {
        const Line* line = dynamic_cast<const Line*>(shape);
        if (!line) return;
        
        sf::Vertex vertices[2];
        Point p1 = line->getPosition();
        Point p2 = line->getEndPoint();
        
        vertices[0].position = sf::Vector2f(p1.x, p1.y);
        vertices[0].color = line->getColor();
        vertices[1].position = sf::Vector2f(p2.x, p2.y);
        vertices[1].color = line->getColor();
        
        window.draw(vertices, 2, sf::PrimitiveType::Lines);
    }
};

#endif
