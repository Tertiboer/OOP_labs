#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "Shape.h"

class Ellipse : public Shape {
protected:
    int radiusX, radiusY;
    
public:
    Ellipse(int x = 0, int y = 0, int rx = 0, int ry = 0, sf::Color color = sf::Color::Blue)
        : Shape(x, y, color), radiusX(rx), radiusY(ry) {}
    
    std::string toString() const override {
        return "Ellipse(" + std::to_string(position.x) + ", " + 
               std::to_string(position.y) + ", " + 
               std::to_string(radiusX) + ", " + 
               std::to_string(radiusY) + ")";
    }
    
    std::string getType() const override { return "Ellipse"; }
    
    std::vector<int> getParameters() const override {
        return {position.x, position.y, radiusX, radiusY};
    }
    
    int getRadiusX() const { return radiusX; }
    int getRadiusY() const { return radiusY; }
    void setRadiusX(int rx) { radiusX = rx; }
    void setRadiusY(int ry) { radiusY = ry; }
    
    // Сериализация
    std::string serialize() const override {
        std::stringstream ss;
        ss << Shape::serialize() << " "
           << radiusX << " " << radiusY;
        return ss.str();
    }
    
    size_t deserialize(const std::vector<std::string>& tokens, size_t index) override {
        index = Shape::deserialize(tokens, index);
        if (tokens.size() > index + 1) {
            radiusX = std::stoi(tokens[index]);
            radiusY = std::stoi(tokens[index + 1]);
            return index + 2;
        }
        return index;
    }
    
    // Методы для редактирования
    int getParameterCount() const override { return 4; }
    
    int getParameter(int paramIndex) const override {
        switch (paramIndex) {
            case 0: return position.x;
            case 1: return position.y;
            case 2: return radiusX;
            case 3: return radiusY;
            default: return 0;
        }
    }
    
    void setParameter(int paramIndex, int value) override {
        switch (paramIndex) {
            case 0: position.x = value; break;
            case 1: position.y = value; break;
            case 2: radiusX = value; break;
            case 3: radiusY = value; break;
        }
    }
};

#endif
