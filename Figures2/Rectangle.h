#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"

class Rectangle : public Shape {
protected:
    int width, height;
    
public:
    Rectangle(int x = 0, int y = 0, int w = 0, int h = 0, sf::Color color = sf::Color::Green)
        : Shape(x, y, color), width(w), height(h) {}
    
    std::string toString() const override {
        return "Rectangle(" + std::to_string(position.x) + ", " + 
               std::to_string(position.y) + ", " + 
               std::to_string(width) + ", " + 
               std::to_string(height) + ")";
    }
    
    std::string getType() const override { return "Rectangle"; }
    
    std::vector<int> getParameters() const override {
        return {position.x, position.y, width, height};
    }
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    void setWidth(int w) { width = w; }
    void setHeight(int h) { height = h; }
    
    // Сериализация
    std::string serialize() const override {
        std::stringstream ss;
        ss << Shape::serialize() << " "
           << width << " " << height;
        return ss.str();
    }
    
    size_t deserialize(const std::vector<std::string>& tokens, size_t index) override {
        index = Shape::deserialize(tokens, index);
        if (tokens.size() > index + 1) {
            width = std::stoi(tokens[index]);
            height = std::stoi(tokens[index + 1]);
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
            case 2: return width;
            case 3: return height;
            default: return 0;
        }
    }
    
    void setParameter(int paramIndex, int value) override {
        switch (paramIndex) {
            case 0: position.x = value; break;
            case 1: position.y = value; break;
            case 2: width = value; break;
            case 3: height = value; break;
        }
    }
};

#endif
