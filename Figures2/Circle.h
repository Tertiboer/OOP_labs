#ifndef CIRCLE_H
#define CIRCLE_H

#include "Ellipse.h"

class Circle : public Ellipse {
public:
    Circle(int x = 0, int y = 0, int radius = 0, sf::Color color = sf::Color::Cyan)
        : Ellipse(x, y, radius, radius, color) {}
    
    std::string toString() const override {
        return "Circle(" + std::to_string(position.x) + ", " + 
               std::to_string(position.y) + ", " + 
               std::to_string(radiusX) + ")";
    }
    
    std::string getType() const override { return "Circle"; }
    
    int getRadius() const { return radiusX; }
    void setRadius(int r) { radiusX = r; radiusY = r; }
    
    std::vector<int> getParameters() const override {
        return {position.x, position.y, radiusX};
    }
    
    // Сериализация
    std::string serialize() const override {
        std::stringstream ss;
        ss << "Circle " << position.x << " " << position.y << " "
           << static_cast<int>(color.r) << " "
           << static_cast<int>(color.g) << " "
           << static_cast<int>(color.b) << " "
           << radiusX;
        return ss.str();
    }
    
    size_t deserialize(const std::vector<std::string>& tokens, size_t index) override {
        index = Shape::deserialize(tokens, index);
        if (tokens.size() > index) {
            radiusX = std::stoi(tokens[index]);
            radiusY = radiusX;
            return index + 1;
        }
        return index;
    }
    
    // Методы для редактирования
    int getParameterCount() const override { return 3; }
    
    int getParameter(int paramIndex) const override {
        switch (paramIndex) {
            case 0: return position.x;
            case 1: return position.y;
            case 2: return radiusX;
            default: return 0;
        }
    }
    
    void setParameter(int paramIndex, int value) override {
        switch (paramIndex) {
            case 0: position.x = value; break;
            case 1: position.y = value; break;
            case 2: radiusX = value; radiusY = value; break;
        }
    }
};

#endif
