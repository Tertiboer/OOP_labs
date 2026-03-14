#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include "PluginInterface.h"

// ===== КЛАСС ТРАПЕЦИИ =====
class Trapezium : public Shape {
private:
    int topWidth;     // верхнее основание
    int bottomWidth;  // нижнее основание
    int height;       // высота
    
public:
    Trapezium(int x = 0, int y = 0, int topW = 0, int bottomW = 0, int h = 0,
              sf::Color color = sf::Color(255, 165, 0))  // Оранжевый
        : Shape(x, y, color), topWidth(topW), bottomWidth(bottomW), height(h) {}
    
    std::string toString() const override {
        return "Trapezium(" + std::to_string(position.x) + ", " + 
               std::to_string(position.y) + ", " + 
               std::to_string(topWidth) + ", " + 
               std::to_string(bottomWidth) + ", " + 
               std::to_string(height) + ")";
    }
    
    std::string getType() const override { return "Trapezium"; }
    
    std::vector<int> getParameters() const override {
        return {position.x, position.y, topWidth, bottomWidth, height};
    }
    
    int getTopWidth() const { return topWidth; }
    int getBottomWidth() const { return bottomWidth; }
    int getHeight() const { return height; }
    
    void setTopWidth(int w) { topWidth = w; }
    void setBottomWidth(int w) { bottomWidth = w; }
    void setHeight(int h) { height = h; }
    
    std::string serialize() const override {
        std::stringstream ss;
        ss << "Trapezium " << position.x << " " << position.y << " "
           << static_cast<int>(color.r) << " "
           << static_cast<int>(color.g) << " "
           << static_cast<int>(color.b) << " "
           << topWidth << " " << bottomWidth << " " << height;
        return ss.str();
    }
    
    size_t deserialize(const std::vector<std::string>& tokens, size_t index) override {
        index = Shape::deserialize(tokens, index);
        if (tokens.size() > index + 2) {
            topWidth = std::stoi(tokens[index]);
            bottomWidth = std::stoi(tokens[index + 1]);
            height = std::stoi(tokens[index + 2]);
            return index + 3;
        }
        return index;
    }
    
    int getParameterCount() const override { return 5; }
    
    int getParameter(int paramIndex) const override {
        switch (paramIndex) {
            case 0: return position.x;
            case 1: return position.y;
            case 2: return topWidth;
            case 3: return bottomWidth;
            case 4: return height;
            default: return 0;
        }
    }
    
    void setParameter(int paramIndex, int value) override {
        switch (paramIndex) {
            case 0: position.x = value; break;
            case 1: position.y = value; break;
            case 2: topWidth = value; break;
            case 3: bottomWidth = value; break;
            case 4: height = value; break;
        }
    }
};

// ===== РИСОВАЛЬЩИК ТРАПЕЦИИ =====
class TrapeziumDrawer : public ShapeDrawer {
public:
    void draw(const Shape* shape, sf::RenderWindow& window) override {
        const Trapezium* trap = dynamic_cast<const Trapezium*>(shape);
        if (!trap) return;
        
        Point pos = trap->getPosition();
        int topW = trap->getTopWidth();
        int bottomW = trap->getBottomWidth();
        int h = trap->getHeight();
        
        // Центрируем верхнее основание относительно нижнего
        float offset = (bottomW - topW) / 2.0f;
        
        sf::ConvexShape trapezium;
        trapezium.setPointCount(4);
        
        trapezium.setPoint(0, sf::Vector2f(pos.x + offset, pos.y));
        trapezium.setPoint(1, sf::Vector2f(pos.x + offset + topW, pos.y));
        trapezium.setPoint(2, sf::Vector2f(pos.x + bottomW, pos.y + h));
        trapezium.setPoint(3, sf::Vector2f(pos.x, pos.y + h));
        
        trapezium.setFillColor(trap->getColor());
        window.draw(trapezium);
    }
};

// ===== ФУНКЦИИ ДЛЯ ПЛАГИНА (extern "C" - чтобы имена не искажались) =====
extern "C" {

// Информация о плагине (статические, живут всё время)
static PluginInfo pluginInfo = {
    "Trapezium",
    "Рисует трапецию с разными основаниями",
    "1.0"
};

PluginInfo* getPluginInfo() {
    return &pluginInfo;
}

Shape* createShape(const std::vector<int>& params, sf::Color color) {
    if (params.size() >= 5) {
        return new Trapezium(params[0], params[1], params[2], params[3], params[4], color);
    }
    return nullptr;
}

ShapeDrawer* createDrawer() {
    return new TrapeziumDrawer();
}

void destroyShape(Shape* shape) {
    delete shape;
}

void destroyDrawer(ShapeDrawer* drawer) {
    delete drawer;
}

} // extern "C"
