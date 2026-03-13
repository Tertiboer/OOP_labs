#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.h"
#include <vector>

class Triangle : public Shape {
private:
    Point point2, point3;
    
public:
    Triangle(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0, int x3 = 0, int y3 = 0,
             sf::Color color = sf::Color::Yellow)
        : Shape(x1, y1, color), point2(x2, y2), point3(x3, y3) {}
    
    std::string toString() const override {
        return "Triangle(" + std::to_string(position.x) + ", " + 
               std::to_string(position.y) + ", " + 
               std::to_string(point2.x) + ", " + 
               std::to_string(point2.y) + ", " + 
               std::to_string(point3.x) + ", " + 
               std::to_string(point3.y) + ")";
    }
    
    std::string getType() const override { return "Triangle"; }
    
    std::vector<Point> getPoints() const {
        return {position, point2, point3};
    }
    
    std::vector<int> getParameters() const override {
        return {position.x, position.y, point2.x, point2.y, point3.x, point3.y};
    }
    
    void setPoint2(int x, int y) { point2 = Point(x, y); }
    void setPoint3(int x, int y) { point3 = Point(x, y); }
    
    // Сериализация
    std::string serialize() const override {
        std::stringstream ss;
        ss << "Triangle " << position.x << " " << position.y << " "
           << static_cast<int>(color.r) << " "
           << static_cast<int>(color.g) << " "
           << static_cast<int>(color.b) << " "
           << point2.x << " " << point2.y << " "
           << point3.x << " " << point3.y;
        return ss.str();
    }
    
    size_t deserialize(const std::vector<std::string>& tokens, size_t index) override {
        index = Shape::deserialize(tokens, index);
        if (tokens.size() > index + 3) {
            point2.x = std::stoi(tokens[index]);
            point2.y = std::stoi(tokens[index + 1]);
            point3.x = std::stoi(tokens[index + 2]);
            point3.y = std::stoi(tokens[index + 3]);
            return index + 4;
        }
        return index;
    }
    
    // Методы для редактирования
    int getParameterCount() const override { return 6; }
    
    int getParameter(int paramIndex) const override {
        switch (paramIndex) {
            case 0: return position.x;
            case 1: return position.y;
            case 2: return point2.x;
            case 3: return point2.y;
            case 4: return point3.x;
            case 5: return point3.y;
            default: return 0;
        }
    }
    
    void setParameter(int paramIndex, int value) override {
        switch (paramIndex) {
            case 0: position.x = value; break;
            case 1: position.y = value; break;
            case 2: point2.x = value; break;
            case 3: point2.y = value; break;
            case 4: point3.x = value; break;
            case 5: point3.y = value; break;
        }
    }
};

#endif
