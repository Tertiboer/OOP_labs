#ifndef SQUARE_H
#define SQUARE_H

#include "Rectangle.h"

class Square : public Rectangle {
public:
    Square(int x = 0, int y = 0, int side = 0, sf::Color color = sf::Color::Magenta)
        : Rectangle(x, y, side, side, color) {}
    
    std::string toString() const override {
        return "Square(" + std::to_string(position.x) + ", " + 
               std::to_string(position.y) + ", " + 
               std::to_string(width) + ")";
    }
    
    std::string getType() const override { return "Square"; }
    
    int getSide() const { return width; }
    void setSide(int s) { width = s; height = s; }
    
    std::vector<int> getParameters() const override {
        return {position.x, position.y, width};
    }
    
    // Сериализация
    std::string serialize() const override {
        std::stringstream ss;
        ss << "Square " << position.x << " " << position.y << " "
           << static_cast<int>(color.r) << " "
           << static_cast<int>(color.g) << " "
           << static_cast<int>(color.b) << " "
           << width;
        return ss.str();
    }
    
    size_t deserialize(const std::vector<std::string>& tokens, size_t index) override {
        index = Shape::deserialize(tokens, index);
        if (tokens.size() > index) {
            width = std::stoi(tokens[index]);
            height = width;
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
            case 2: return width;
            default: return 0;
        }
    }
    
    void setParameter(int paramIndex, int value) override {
        switch (paramIndex) {
            case 0: position.x = value; break;
            case 1: position.y = value; break;
            case 2: width = value; height = value; break;
        }
    }
};

#endif
