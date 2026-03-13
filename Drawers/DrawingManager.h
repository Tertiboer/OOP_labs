#ifndef DRAWINGMANAGER_H
#define DRAWINGMANAGER_H

#include <map>
#include <memory>
#include <string>
#include <typeinfo>
#include "../Figures/Shape.h"
#include "ShapeDrawer.h"
#include "LineDrawer.h"
#include "RectangleDrawer.h"
#include "EllipseDrawer.h"
#include "TriangleDrawer.h"
#include "CircleDrawer.h"
#include "SquareDrawer.h"

// Класс для управления рисованием фигур
class DrawingManager {
private:
    std::map<size_t, std::unique_ptr<ShapeDrawer>> drawers;
    
public:
    DrawingManager() {
        // Регистрируем рисовальщики для каждого типа фигур
        drawers[typeid(Line).hash_code()] = std::make_unique<LineDrawer>();
        drawers[typeid(Rectangle).hash_code()] = std::make_unique<RectangleDrawer>();
        drawers[typeid(Ellipse).hash_code()] = std::make_unique<EllipseDrawer>();
        drawers[typeid(Triangle).hash_code()] = std::make_unique<TriangleDrawer>();
        drawers[typeid(Circle).hash_code()] = std::make_unique<CircleDrawer>();
        drawers[typeid(Square).hash_code()] = std::make_unique<SquareDrawer>();
    }
    
    // Рисование фигуры
    void draw(const Shape* shape, sf::RenderWindow& window) {
        if (!shape) return;
        
        size_t typeHash = typeid(*shape).hash_code();
        auto it = drawers.find(typeHash);
        if (it != drawers.end()) {
            it->second->draw(shape, window);
        }
    }
};

#endif
