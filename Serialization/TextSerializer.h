#ifndef TEXTSERIALIZER_H
#define TEXTSERIALIZER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <functional>
#include "Serializable.h"
#include "../Figures/Shape.h"
#include "../Figures/Line.h"
#include "../Figures/Rectangle.h"
#include "../Figures/Ellipse.h"
#include "../Figures/Triangle.h"
#include "../Figures/Circle.h"
#include "../Figures/Square.h"

/**
 * Класс для текстовой сериализации
 * Формат файла:
 * [TYPE] param1 param2 param3 ... paramN
 * 
 * Пример:
 * Line 50 50 150 50 255 0 0
 * Rectangle 200 50 100 60 0 255 0
 */
class TextSerializer {
private:
    // Реестр фабрик для создания фигур по типу
    using FactoryFunc = std::function<std::unique_ptr<Shape>(const std::vector<std::string>&)>;
    static std::map<std::string, FactoryFunc>& getRegistry() {
        static std::map<std::string, FactoryFunc> registry;
        return registry;
    }
    
    // Преобразование цвета в строку
    static std::string colorToString(const sf::Color& color) {
        return std::to_string(color.r) + " " + 
               std::to_string(color.g) + " " + 
               std::to_string(color.b);
    }
    
    // Преобразование строки в цвет
    static sf::Color stringToColor(const std::vector<std::string>& tokens, size_t startIndex) {
        if (tokens.size() > startIndex + 2) {
            return sf::Color(
                std::stoi(tokens[startIndex]),
                std::stoi(tokens[startIndex + 1]),
                std::stoi(tokens[startIndex + 2])
            );
        }
        return sf::Color::White; // Цвет по умолчанию
    }
    
public:
    /**
     * Регистрация фабрики для создания фигур
     * Это позволяет добавлять новые классы без изменения кода
     */
    static void registerShape(const std::string& type, FactoryFunc factory) {
        getRegistry()[type] = factory;
    }
    
    /**
     * Сохранение списка фигур в файл
     */
    static bool saveToFile(const std::vector<std::unique_ptr<Shape>>& figures, const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Cannot open file for writing: " << filename << std::endl;
            return false;
        }
        
        for (const auto& fig : figures) {
            // Каждая фигура сама знает, как себя сериализовать
            // Но для этого нужно добавить метод serialize в Shape
            // Пока сделаем простой вариант
            std::string type = getTypeForShape(fig.get());
            if (!type.empty()) {
                file << type << " ";
                // Здесь нужно вызвать метод фигуры для сериализации
                // Добавим позже
            }
        }
        
        file.close();
        return true;
    }
    
    /**
     * Загрузка списка фигур из файла
     */
    static std::vector<std::unique_ptr<Shape>> loadFromFile(const std::string& filename) {
        std::vector<std::unique_ptr<Shape>> result;
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cerr << "Cannot open file for reading: " << filename << std::endl;
            return result;
        }
        
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue; // Пропускаем пустые строки и комментарии
            
            auto tokens = splitLine(line);
            if (tokens.empty()) continue;
            
            std::string type = tokens[0];
            auto it = getRegistry().find(type);
            if (it != getRegistry().end()) {
                auto shape = it->second(tokens);
                if (shape) {
                    result.push_back(std::move(shape));
                }
            }
        }
        
        file.close();
        return result;
    }
    
private:
    static std::vector<std::string> splitLine(const std::string& line) {
        std::vector<std::string> tokens;
        std::istringstream iss(line);
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }
    
    // Временная функция для определения типа фигуры
    static std::string getTypeForShape(const Shape* shape) {
        // Это нужно будет заменить на виртуальный метод getType()
        // Добавим позже в Shape
        return "";
    }
};

// Макрос для автоматической регистрации фигур
#define REGISTER_SHAPE_FOR_SERIALIZATION(shapeType, className) \
    static bool registered_##className = []() { \
        TextSerializer::registerShape(shapeType, \
            [](const std::vector<std::string>& tokens) -> std::unique_ptr<Shape> { \
                try { \
                    return std::make_unique<className>(/* параметры из tokens */); \
                } catch (...) { \
                    return nullptr; \
                } \
            }); \
        return true; \
    }();

#endif
