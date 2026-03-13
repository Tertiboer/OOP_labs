#ifndef SHAPEFACTORY_H
#define SHAPEFACTORY_H

#include <functional>
#include <map>
#include <string>
#include <memory>
#include "../Figures/Shape.h"
#include "../Figures/Line.h"
#include "../Figures/Rectangle.h"
#include "../Figures/Ellipse.h"
#include "../Figures/Triangle.h"
#include "../Figures/Circle.h"
#include "../Figures/Square.h"

// Фабрика использует регистрацию классов - при добавлении новой фигуры
// нужно только зарегистрировать её здесь, не меняя существующий код
class ShapeFactory {
private:
    // Словарь, сопоставляющий имя фигуры с функцией-создателем
    using CreatorFunc = std::function<std::unique_ptr<Shape>(const std::vector<int>& params, sf::Color)>;
    static std::map<std::string, CreatorFunc>& getRegistry() {
        static std::map<std::string, CreatorFunc> registry;
        return registry;
    }
    
public:
    // Регистрация новой фигуры
    static void registerShape(const std::string& name, CreatorFunc creator) {
        getRegistry()[name] = creator;
    }
    
    // Создание фигуры по имени
    static std::unique_ptr<Shape> createShape(const std::string& name, 
                                              const std::vector<int>& params, 
                                              sf::Color color) {
        auto& registry = getRegistry();
        auto it = registry.find(name);
        if (it != registry.end()) {
            return it->second(params, color);
        }
        return nullptr;
    }
    
    // Получение списка всех доступных фигур
    static std::vector<std::string> getAvailableShapes() {
        std::vector<std::string> names;
        for (const auto& pair : getRegistry()) {
            names.push_back(pair.first);
        }
        return names;
    }
};

// Автоматическая регистрация фигур при запуске программы
// Это гарантирует, что новые фигуры регистрируются без изменения кода
class ShapeRegistrar {
public:
    ShapeRegistrar(const std::string& name, 
                   std::function<std::unique_ptr<Shape>(const std::vector<int>&, sf::Color)> creator) {
        ShapeFactory::registerShape(name, creator);
    }
};

// Макрос для удобной регистрации
#define REGISTER_SHAPE(shapeName, className) \
    static ShapeRegistrar registrar_##className( \
        shapeName, \
        [](const std::vector<int>& params, sf::Color color) -> std::unique_ptr<Shape> { \
            switch(params.size()) { \
                case 2: return std::make_unique<className>(params[0], params[1], color); \
                case 3: return std::make_unique<className>(params[0], params[1], params[2], color); \
                case 4: return std::make_unique<className>(params[0], params[1], params[2], params[3], color); \
                case 6: return std::make_unique<className>(params[0], params[1], params[2], params[3], params[4], params[5], color); \
                default: return nullptr; \
            } \
        })

// Регистрируем все фигуры
static ShapeRegistrar registerLine("Line", 
    [](const std::vector<int>& params, sf::Color color) -> std::unique_ptr<Shape> {
        return std::make_unique<Line>(params[0], params[1], params[2], params[3], color);
    });

static ShapeRegistrar registerRectangle("Rectangle", 
    [](const std::vector<int>& params, sf::Color color) -> std::unique_ptr<Shape> {
        return std::make_unique<Rectangle>(params[0], params[1], params[2], params[3], color);
    });

static ShapeRegistrar registerEllipse("Ellipse", 
    [](const std::vector<int>& params, sf::Color color) -> std::unique_ptr<Shape> {
        return std::make_unique<Ellipse>(params[0], params[1], params[2], params[3], color);
    });

static ShapeRegistrar registerTriangle("Triangle", 
    [](const std::vector<int>& params, sf::Color color) -> std::unique_ptr<Shape> {
        return std::make_unique<Triangle>(params[0], params[1], params[2], params[3], params[4], params[5], color);
    });

static ShapeRegistrar registerCircle("Circle", 
    [](const std::vector<int>& params, sf::Color color) -> std::unique_ptr<Shape> {
        return std::make_unique<Circle>(params[0], params[1], params[2], color);
    });

static ShapeRegistrar registerSquare("Square", 
    [](const std::vector<int>& params, sf::Color color) -> std::unique_ptr<Shape> {
        return std::make_unique<Square>(params[0], params[1], params[2], color);
    });

#endif
