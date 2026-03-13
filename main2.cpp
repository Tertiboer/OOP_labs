#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include "Figures/Point.h"
#include "Figures/Shape.h"
#include "Figures/Line.h"
#include "Figures/Rectangle.h"
#include "Figures/Ellipse.h"
#include "Figures/Triangle.h"
#include "Figures/Circle.h"
#include "Figures/Square.h"
#include "FiguresList.h"
#include "Drawers/DrawingManager.h"
#include "Serialization/SerializationManager.h"

// Объявления функций
int toInt(const std::string& str);
std::vector<std::string> splitString(const std::string& str);
void printHelp();
bool processCommand(const std::string& cmd, FiguresList& figuresList, int& colorIndex);
sf::Color getNextColor(int index);

// Цвета для фигур
sf::Color getNextColor(int index) {
    static sf::Color colors[] = {
        sf::Color::Red,
        sf::Color::Green,
        sf::Color::Blue,
        sf::Color::Yellow,
        sf::Color::Cyan,
        sf::Color::Magenta,
        sf::Color(255, 165, 0), // Оранжевый
        sf::Color(128, 0, 128), // Фиолетовый
        sf::Color(0, 255, 0),   // Ярко-зеленый
        sf::Color(255, 192, 203) // Розовый
    };
    return colors[index % 10];
}

// Разделение строки
std::vector<std::string> splitString(const std::string& str) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// Преобразование строки в число
int toInt(const std::string& str) {
    return std::stoi(str);
}

// Вывод справки
void printHelp() {
    std::cout << "\n========== ГРАФИЧЕСКИЙ РЕДАКТОР ==========\n";
    std::cout << "КОМАНДЫ ДЛЯ СОЗДАНИЯ:\n";
    std::cout << "  line x1 y1 x2 y2           - линия\n";
    std::cout << "  rect x y w h                - прямоугольник\n";
    std::cout << "  ellipse x y rx ry           - эллипс\n";
    std::cout << "  triangle x1 y1 x2 y2 x3 y3  - треугольник\n";
    std::cout << "  circle x y r                 - круг\n";
    std::cout << "  square x y side              - квадрат\n";
    std::cout << "\nКОМАНДЫ ДЛЯ РЕДАКТИРОВАНИЯ:\n";
    std::cout << "  list                          - показать все фигуры\n";
    std::cout << "  edit index param value        - изменить параметр фигуры\n";
    std::cout << "  del index                     - удалить фигуру\n";
    std::cout << "  clear                         - удалить все фигуры\n";
    std::cout << "\nКОМАНДЫ ДЛЯ СЕРИАЛИЗАЦИИ:\n";
    std::cout << "  save filename                  - сохранить в файл\n";
    std::cout << "  load filename                  - загрузить из файла\n";
    std::cout << "\nПРОЧЕЕ:\n";
    std::cout << "  help                          - эта справка\n";
    std::cout << "  exit                          - выход\n";
    std::cout << "==========================================\n\n";
}

// Обработка команд
bool processCommand(const std::string& cmd, FiguresList& figuresList, int& colorIndex) {
    auto tokens = splitString(cmd);
    if (tokens.empty()) return true;
    
    std::string command = tokens[0];
    
    if (command == "exit" || command == "quit") {
        return false;
    }
    else if (command == "help") {
        printHelp();
    }
    else if (command == "list") {
        figuresList.printAllInfo();
    }
    else if (command == "clear") {
        figuresList.clear();
        std::cout << "✅ Все фигуры удалены" << std::endl;
    }
    else if (command == "del" && tokens.size() >= 2) {
        try {
            int index = toInt(tokens[1]);
            if (figuresList.removeFigure(index)) {
                std::cout << "✅ Фигура " << index << " удалена" << std::endl;
            } else {
                std::cout << "❌ Неверный индекс" << std::endl;
            }
        } catch (...) {
            std::cout << "❌ Ошибка в индексе" << std::endl;
        }
    }
    else if (command == "edit" && tokens.size() >= 4) {
        try {
            int index = toInt(tokens[1]);
            int param = toInt(tokens[2]);
            int value = toInt(tokens[3]);
            
            if (figuresList.editFigure(index, param, value)) {
                std::cout << "✅ Фигура " << index << " изменена" << std::endl;
            } else {
                std::cout << "❌ Ошибка редактирования" << std::endl;
            }
        } catch (...) {
            std::cout << "❌ Ошибка в параметрах" << std::endl;
        }
    }
    else if (command == "save" && tokens.size() >= 2) {
        std::string filename = tokens[1];
        if (figuresList.saveToFile(filename)) {
            std::cout << "✅ Сохранено в " << filename << std::endl;
        } else {
            std::cout << "❌ Ошибка сохранения" << std::endl;
        }
    }
    else if (command == "load" && tokens.size() >= 2) {
        std::string filename = tokens[1];
        if (figuresList.loadFromFile(filename)) {
            std::cout << "✅ Загружено из " << filename << std::endl;
        } else {
            std::cout << "❌ Ошибка загрузки" << std::endl;
        }
    }
    else if (command == "line" && tokens.size() >= 5) {
        try {
            int x1 = toInt(tokens[1]);
            int y1 = toInt(tokens[2]);
            int x2 = toInt(tokens[3]);
            int y2 = toInt(tokens[4]);
            figuresList.addFigure(std::make_unique<Line>(x1, y1, x2, y2, getNextColor(colorIndex++)));
            std::cout << "✅ Линия создана!" << std::endl;
        } catch (...) {
            std::cout << "❌ Ошибка в координатах" << std::endl;
        }
    }
    else if (command == "rect" && tokens.size() >= 5) {
        try {
            int x = toInt(tokens[1]);
            int y = toInt(tokens[2]);
            int w = toInt(tokens[3]);
            int h = toInt(tokens[4]);
            figuresList.addFigure(std::make_unique<Rectangle>(x, y, w, h, getNextColor(colorIndex++)));
            std::cout << "✅ Прямоугольник создан!" << std::endl;
        } catch (...) {
            std::cout << "❌ Ошибка в координатах" << std::endl;
        }
    }
    else if (command == "ellipse" && tokens.size() >= 5) {
        try {
            int x = toInt(tokens[1]);
            int y = toInt(tokens[2]);
            int rx = toInt(tokens[3]);
            int ry = toInt(tokens[4]);
            figuresList.addFigure(std::make_unique<Ellipse>(x, y, rx, ry, getNextColor(colorIndex++)));
            std::cout << "✅ Эллипс создан!" << std::endl;
        } catch (...) {
            std::cout << "❌ Ошибка в координатах" << std::endl;
        }
    }
    else if (command == "circle" && tokens.size() >= 4) {
        try {
            int x = toInt(tokens[1]);
            int y = toInt(tokens[2]);
            int r = toInt(tokens[3]);
            figuresList.addFigure(std::make_unique<Circle>(x, y, r, getNextColor(colorIndex++)));
            std::cout << "✅ Круг создан!" << std::endl;
        } catch (...) {
            std::cout << "❌ Ошибка в координатах" << std::endl;
        }
    }
    else if (command == "square" && tokens.size() >= 4) {
        try {
            int x = toInt(tokens[1]);
            int y = toInt(tokens[2]);
            int side = toInt(tokens[3]);
            figuresList.addFigure(std::make_unique<Square>(x, y, side, getNextColor(colorIndex++)));
            std::cout << "✅ Квадрат создан!" << std::endl;
        } catch (...) {
            std::cout << "❌ Ошибка в координатах" << std::endl;
        }
    }
    else if (command == "triangle" && tokens.size() >= 7) {
        try {
            int x1 = toInt(tokens[1]);
            int y1 = toInt(tokens[2]);
            int x2 = toInt(tokens[3]);
            int y2 = toInt(tokens[4]);
            int x3 = toInt(tokens[5]);
            int y3 = toInt(tokens[6]);
            figuresList.addFigure(std::make_unique<Triangle>(x1, y1, x2, y2, x3, y3, getNextColor(colorIndex++)));
            std::cout << "✅ Треугольник создан!" << std::endl;
        } catch (...) {
            std::cout << "❌ Ошибка в координатах" << std::endl;
        }
    }
    else {
        std::cout << "❌ Неизвестная команда. Введите 'help'" << std::endl;
    }
    
    return true;
}

int main() {
    // Создаем пустой список фигур
    FiguresList figuresList;
    
    // Менеджер рисования
    DrawingManager drawingManager;
    
    // Счетчик для цветов
    int colorIndex = 0;
    
    // Создаем окно
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Graphics Editor");
    
    std::cout << "\n🎨 ГРАФИЧЕСКИЙ РЕДАКТОР ЗАПУЩЕН!\n";
    std::cout << "Формат сериализации: TEXT (вариант 3)\n";
    printHelp();
    
    // Переменная для ввода команд
    std::string command;
    bool running = true;
    
    // Создаем поток для ввода команд
    std::thread inputThread([&]() {
        while (running) {
            std::getline(std::cin, command);
            if (!running) break;
            
            // Обрабатываем команду
            if (!processCommand(command, figuresList, colorIndex)) {
                running = false;
            }
        }
    });
    
    // Отсоединяем поток
    inputThread.detach();
    
    // Главный цикл отрисовки
    while (window.isOpen()) {
        // Обработка событий окна
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
                running = false;
            }
        }
        
        // Отрисовка
        window.clear(sf::Color::Black);
        
        // Рисуем все фигуры
        for (size_t i = 0; i < figuresList.getCount(); ++i) {
            const Shape* shape = figuresList.getFigure(i);
            drawingManager.draw(shape, window);
        }
        
        window.display();
        
        // Небольшая задержка
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    std::cout << "👋 Программа завершена\n";
    return 0;
}
