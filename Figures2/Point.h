#ifndef POINT_H
#define POINT_H

#include <string>
#include <sstream>

/**
 * Структура точки с координатами
 */
struct Point {
    int x, y;
    
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    
    /**
     * Сериализовать точку в строку
     */
    std::string serialize() const {
        return std::to_string(x) + " " + std::to_string(y);
    }
    
    /**
     * Десериализовать точку из строки
     */
    static Point deserialize(const std::string& str) {
        std::istringstream iss(str);
        int x, y;
        iss >> x >> y;
        return Point(x, y);
    }
    
    /**
     * Десериализовать точку из токенов
     */
    static Point deserialize(const std::vector<std::string>& tokens, size_t index) {
        if (tokens.size() > index + 1) {
            return Point(std::stoi(tokens[index]), std::stoi(tokens[index + 1]));
        }
        return Point();
    }
    
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

#endif
