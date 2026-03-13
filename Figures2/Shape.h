#ifndef SHAPE_H
#define SHAPE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <sstream>
#include "Point.h"

class Shape {
protected:
    Point position;
    sf::Color color;
    
public:
    Shape(int x = 0, int y = 0, sf::Color color = sf::Color::White) 
        : position(x, y), color(color) {}
    
    virtual ~Shape() {}
    
    // Виртуальные методы (были)
    virtual std::string toString() const = 0;
    virtual std::vector<int> getParameters() const = 0;
    
    // НОВЫЕ МЕТОДЫ ДЛЯ СЕРИАЛИЗАЦИИ
    /**
     * Получить тип фигуры для сериализации
     */
    virtual std::string getType() const = 0;
    
    /**
     * Сериализовать фигуру в строку
     * Формат: TYPE x y r g b [параметры]
     */
    virtual std::string serialize() const {
        std::stringstream ss;
        ss << getType() << " "
           << position.x << " " << position.y << " "
           << static_cast<int>(color.r) << " "
           << static_cast<int>(color.g) << " "
           << static_cast<int>(color.b);
        return ss.str();
    }
    
    /**
     * Десериализовать фигуру из токенов
     * @param tokens вектор строк с параметрами
     * @param index начальный индекс для чтения
     * @return новый индекс после чтения
     */
    virtual size_t deserialize(const std::vector<std::string>& tokens, size_t index) {
        if (tokens.size() <= index + 4) return index;
        
        try {
            position.x = std::stoi(tokens[index]);
            position.y = std::stoi(tokens[index + 1]);
            color.r = static_cast<uint8_t>(std::stoi(tokens[index + 2]));
            color.g = static_cast<uint8_t>(std::stoi(tokens[index + 3]));
            color.b = static_cast<uint8_t>(std::stoi(tokens[index + 4]));
            return index + 5;
        } catch (...) {
            return index;
        }
    }
    
    // Геттеры и сеттеры
    void setColor(sf::Color newColor) { color = newColor; }
    sf::Color getColor() const { return color; }
    
    void setPosition(int x, int y) { position = Point(x, y); }
    Point getPosition() const { return position; }
    
    // Методы для редактирования
    virtual void setParameter(int paramIndex, int value) = 0;
    virtual int getParameter(int paramIndex) const = 0;
    virtual int getParameterCount() const = 0;
};

#endif
