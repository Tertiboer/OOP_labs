#ifndef FIGURESLIST_H
#define FIGURESLIST_H

#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include "Figures/Shape.h"
#include "Serialization/SerializationManager.h"

/**
 * Класс для хранения списка фигур
 * Поддерживает добавление, удаление, редактирование и сериализацию
 */
class FiguresList {
private:
    std::vector<std::unique_ptr<Shape>> figures;
    
public:
    ~FiguresList() = default;
    
    // Добавление фигуры
    void addFigure(std::unique_ptr<Shape> figure) {
        if (figure) {
            figures.push_back(std::move(figure));
        }
    }
    
    // Удаление фигуры по индексу
    bool removeFigure(size_t index) {
        if (index < figures.size()) {
            figures.erase(figures.begin() + index);
            return true;
        }
        return false;
    }
    
    // Очистка всех фигур
    void clear() {
        figures.clear();
    }
    
    // Получение фигуры по индексу (для чтения)
    const Shape* getFigure(size_t index) const {
        if (index < figures.size()) {
            return figures[index].get();
        }
        return nullptr;
    }
    
    // Получение фигуры по индексу (для редактирования)
    Shape* getFigureForEdit(size_t index) {
        if (index < figures.size()) {
            return figures[index].get();
        }
        return nullptr;
    }
    
    // Количество фигур
    size_t getCount() const { return figures.size(); }
    
    // Вывод информации о всех фигурах
    void printAllInfo() const {
        std::cout << "\n=== Список фигур (" << figures.size() << ") ===\n";
        for (size_t i = 0; i < figures.size(); ++i) {
            std::cout << "  [" << i << "] " << figures[i]->toString() << std::endl;
        }
        std::cout << "================================\n";
    }
    
    // Сохранение в файл
    bool saveToFile(const std::string& filename) const {
        return SerializationManager::saveToFile(figures, filename);
    }
    
    // Загрузка из файла
    bool loadFromFile(const std::string& filename) {
        auto loaded = SerializationManager::loadFromFile(filename);
        if (!loaded.empty()) {
            figures = std::move(loaded);
            return true;
        }
        return false;
    }
    
    // Редактирование фигуры
    bool editFigure(size_t index, int paramIndex, int newValue) {
        Shape* shape = getFigureForEdit(index);
        if (shape && paramIndex >= 0 && paramIndex < shape->getParameterCount()) {
            shape->setParameter(paramIndex, newValue);
            return true;
        }
        return false;
    }
    
    // Получение всех фигур (для итерации)
    const std::vector<std::unique_ptr<Shape>>& getFigures() const {
        return figures;
    }
};

#endif
