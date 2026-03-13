#ifndef SERIALIZATIONMANAGER_H
#define SERIALIZATIONMANAGER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <functional>
#include "../Figures/Shape.h"
#include "../Figures/Line.h"
#include "../Figures/Rectangle.h"
#include "../Figures/Ellipse.h"
#include "../Figures/Triangle.h"
#include "../Figures/Circle.h"
#include "../Figures/Square.h"

/**
 * Менеджер сериализации для текстового формата
 * Реализует паттерн Factory Method для создания фигур по типу
 */
class SerializationManager {
private:
    // Реестр фабрик для создания фигур
    using ShapeFactory = std::function<std::unique_ptr<Shape>(const std::vector<std::string>&)>;
    static std::map<std::string, ShapeFactory>& getRegistry() {
        static std::map<std::string, ShapeFactory> registry;
        return registry;
    }
    
    // Разделение строки на токены
    static std::vector<std::string> splitLine(const std::string& line) {
        std::vector<std::string> tokens;
        std::istringstream iss(line);
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }
    
public:
    /**
     * Регистрация фабрики для создания фигуры определенного типа
     * Это позволяет добавлять новые классы без изменения кода
     */
    static void registerShape(const std::string& type, ShapeFactory factory) {
        getRegistry()[type] = factory;
    }
    
    /**
     * Сохранить список фигур в текстовый файл
     * Формат: каждая фигура на отдельной строке
     */
    static bool saveToFile(const std::vector<std::unique_ptr<Shape>>& figures, const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка: не удалось открыть файл для записи: " << filename << std::endl;
            return false;
        }
        
        // Заголовок файла
        file << "# Graphics Editor Shapes File\n";
        file << "# Format: TYPE X Y R G B [params...]\n";
        file << "#\n";
        
        for (const auto& fig : figures) {
            if (fig) {
                file << fig->serialize() << "\n";
            }
        }
        
        file.close();
        std::cout << "✅ Фигуры сохранены в файл: " << filename << std::endl;
        return true;
    }
    
    /**
     * Загрузить список фигур из текстового файла
     */
    static std::vector<std::unique_ptr<Shape>> loadFromFile(const std::string& filename) {
        std::vector<std::unique_ptr<Shape>> result;
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cerr << "Ошибка: не удалось открыть файл для чтения: " << filename << std::endl;
            return result;
        }
        
        std::string line;
        int lineNumber = 0;
        
        while (std::getline(file, line)) {
            lineNumber++;
            
            // Пропускаем пустые строки и комментарии
            if (line.empty() || line[0] == '#') continue;
            
            auto tokens = splitLine(line);
            if (tokens.empty()) continue;
            
            std::string type = tokens[0];
            auto it = getRegistry().find(type);
            
            if (it != getRegistry().end()) {
                try {
                    auto shape = it->second(tokens);
                    if (shape) {
                        result.push_back(std::move(shape));
                        std::cout << "  ✅ Загружена фигура: " << type << std::endl;
                    } else {
                        std::cerr << "  ⚠️ Строка " << lineNumber << ": не удалось создать фигуру " << type << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "  ⚠️ Строка " << lineNumber << ": ошибка - " << e.what() << std::endl;
                }
            } else {
                std::cerr << "  ⚠️ Строка " << lineNumber << ": неизвестный тип фигуры '" << type << "'" << std::endl;
            }
        }
        
        file.close();
        std::cout << "✅ Загружено фигур: " << result.size() << " из файла " << filename << std::endl;
        return result;
    }
    
    /**
     * Получить список всех зарегистрированных типов фигур
     */
    static std::vector<std::string> getRegisteredTypes() {
        std::vector<std::string> types;
        for (const auto& pair : getRegistry()) {
            types.push_back(pair.first);
        }
        return types;
    }
};

// Макрос для автоматической регистрации фигур (без комментариев внутри)
#define REGISTER_SHAPE(shapeType, className) \
    static bool registered_##className = []() { \
        SerializationManager::registerShape(shapeType, \
            [](const std::vector<std::string>& tokens) -> std::unique_ptr<Shape> { \
                try { \
                    auto shape = std::make_unique<className>(); \
                    size_t index = 1; \
                    shape->deserialize(tokens, index); \
                    return shape; \
                } catch (const std::exception& e) { \
                    std::cerr << "Ошибка создания " << shapeType << ": " << e.what() << std::endl; \
                    return nullptr; \
                } \
            }); \
        return true; \
    }();

// Регистрируем все фигуры
REGISTER_SHAPE("Line", Line)
REGISTER_SHAPE("Rectangle", Rectangle)
REGISTER_SHAPE("Ellipse", Ellipse)
REGISTER_SHAPE("Triangle", Triangle)
REGISTER_SHAPE("Circle", Circle)
REGISTER_SHAPE("Square", Square)

#endif
