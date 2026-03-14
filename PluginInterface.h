#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#include <memory>
#include <string>
#include <vector>
#include "Figures/Shape.h"
#include "Drawers/ShapeDrawer.h"

// Структура с информацией о плагине
struct PluginInfo {
    std::string name;        // "Trapezium"
    std::string description; // "Рисует трапецию"
    std::string version;     // "1.0"
};

// Функции, которые должен предоставить плагин
// (все с extern "C" чтобы имя не искажалось компилятором)
extern "C" {
    // Получить информацию о плагине
    PluginInfo* getPluginInfo();
    
    // Создать фигуру по параметрам
    Shape* createShape(const std::vector<int>& params, sf::Color color);
    
    // Создать рисовальщик
    ShapeDrawer* createDrawer();
    
    // Уничтожить фигуру (чтобы правильно освободить память)
    void destroyShape(Shape* shape);
    
    // Уничтожить рисовальщик
    void destroyDrawer(ShapeDrawer* drawer);
}

#endif
