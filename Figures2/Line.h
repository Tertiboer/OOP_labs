#ifndef LINE_H
#define LINE_H

#include "Shape.h"

class Line : public Shape {
private:
    Point endPoint;
    
public:
    Line(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0, sf::Color color = sf::Color::Red)
        : Shape(x1, y1, color), endPoint(x2, y2) {}
    
    std::string toString() const override {
        return "Line(" + std::to_string(position.x) + ", " + 
               std::to_string(position.y) + ", " + 
               std::to_string(endPoint.x) + ", " + 
               std::to_string(endPoint.y) + ")";
    }
    
    std::string getType() const override { return "Line"; }
    
    std::vector<int> getParameters() const override {
        return {position.x, position.y, endPoint.x, endPoint.y};
    }
    
    // Специфичные методы
    Point getEndPoint() const { return endPoint; }
    void setEndPoint(int x, int y) { endPoint = Point(x, y); }
    
    // Сериализация
    std::string serialize() const override {
        std::stringstream ss;
        ss << Shape::serialize() << " "
           << endPoint.x << " " << endPoint.y;
        return ss.str();
    }
    
    size_t deserialize(const std::vector<std::string>& tokens, size_t index) override {
        index = Shape::deserialize(tokens, index);
        if (tokens.size() > index + 1) {
            endPoint.x = std::stoi(tokens[index]);
            endPoint.y = std::stoi(tokens[index + 1]);
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
            case 2: return endPoint.x;
            case 3: return endPoint.y;
            default: return 0;
        }
    }
    
    void setParameter(int paramIndex, int value) override {
        switch (paramIndex) {
            case 0: position.x = value; break;
            case 1: position.y = value; break;
            case 2: endPoint.x = value; break;
            case 3: endPoint.y = value; break;
        }
    }
};

#endif
