#ifndef EDITORUI_H
#define EDITORUI_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../FiguresList.h"
#include "../Factory/ShapeFactory.h"
#include "../Drawers/DrawingManager.h"

// Простой UI для редактора
class EditorUI {
private:
    sf::RenderWindow& window;
    FiguresList& figuresList;
    DrawingManager drawingManager;
    
    // Состояние редактора
    enum class Mode { SELECT, CREATE_LINE, CREATE_RECT, CREATE_ELLIPSE, 
                      CREATE_TRIANGLE, CREATE_CIRCLE, CREATE_SQUARE };
    Mode currentMode = Mode::SELECT;
    
    // Для создания фигур мышью
    bool isDrawing = false;
    Point startPoint;
    Point currentPoint;
    
    // Текстовый ввод
    sf::Font font;
    sf::Text instructionText;  // Будет инициализирован в конструкторе с шрифтом
    std::string inputBuffer;
    bool waitingForInput = false;
    std::string pendingShapeType;
    
public:
    EditorUI(sf::RenderWindow& win, FiguresList& list) : window(win), figuresList(list), 
        instructionText(font)  // Инициализируем Text с шрифтом
    {
        // Настройка текста
        instructionText.setCharacterSize(20);
        instructionText.setFillColor(sf::Color::White);
        instructionText.setPosition(sf::Vector2f(10, 10));
        
        // Пытаемся загрузить шрифт (необязательно)
        if (!font.openFromFile("Arial.ttf")) {
            std::cout << "Font not found, using default" << std::endl;
        }
        instructionText.setFont(font);  // Устанавливаем шрифт после загрузки
    }
    
    void handleEvent(const sf::Event& event) {
        if (event.is<sf::Event::KeyPressed>()) {
            handleKeyPress(event.getIf<sf::Event::KeyPressed>());
        }
        else if (event.is<sf::Event::MouseButtonPressed>()) {
            handleMousePress(event.getIf<sf::Event::MouseButtonPressed>());
        }
        else if (event.is<sf::Event::MouseButtonReleased>()) {
            handleMouseRelease(event.getIf<sf::Event::MouseButtonReleased>());
        }
        else if (event.is<sf::Event::MouseMoved>()) {
            handleMouseMove(event.getIf<sf::Event::MouseMoved>());
        }
        else if (event.is<sf::Event::TextEntered>()) {
            handleTextInput(event.getIf<sf::Event::TextEntered>());
        }
    }
    
    void handleKeyPress(const sf::Event::KeyPressed* key) {
        if (waitingForInput) return;
        
        switch (key->code) {
            case sf::Keyboard::Key::L: currentMode = Mode::CREATE_LINE; break;
            case sf::Keyboard::Key::R: currentMode = Mode::CREATE_RECT; break;
            case sf::Keyboard::Key::E: currentMode = Mode::CREATE_ELLIPSE; break;
            case sf::Keyboard::Key::T: currentMode = Mode::CREATE_TRIANGLE; break;
            case sf::Keyboard::Key::C: currentMode = Mode::CREATE_CIRCLE; break;
            case sf::Keyboard::Key::S: currentMode = Mode::CREATE_SQUARE; break;
            case sf::Keyboard::Key::Escape: currentMode = Mode::SELECT; break;
            case sf::Keyboard::Key::F1: showHelp(); break;
            default: break;
        }
    }
    
    void handleMousePress(const sf::Event::MouseButtonPressed* mouse) {
        if (mouse->button == sf::Mouse::Button::Left) {
            startPoint = Point(mouse->position.x, mouse->position.y);
            currentPoint = startPoint;
            isDrawing = true;
        }
    }
    
    void handleMouseRelease(const sf::Event::MouseButtonReleased* mouse) {
        if (mouse->button == sf::Mouse::Button::Left && isDrawing) {
            createFigureFromMouse();
            isDrawing = false;
        }
    }
    
    void handleMouseMove(const sf::Event::MouseMoved* move) {
        if (isDrawing) {
            currentPoint = Point(move->position.x, move->position.y);
        }
    }
    
    void handleTextInput(const sf::Event::TextEntered* text) {
        if (!waitingForInput) return;
        
        if (text->unicode == 13) { // Enter
            processTextInput();
        }
        else if (text->unicode == 8) { // Backspace
            if (!inputBuffer.empty()) {
                inputBuffer.pop_back();
            }
        }
        else if (text->unicode < 128) {
            inputBuffer += static_cast<char>(text->unicode);
        }
    }
    
    void createFigureFromMouse() {
        std::unique_ptr<Shape> newShape;
        
        switch (currentMode) {
            case Mode::CREATE_LINE:
                newShape = std::make_unique<Line>(
                    startPoint.x, startPoint.y, 
                    currentPoint.x, currentPoint.y,
                    sf::Color::Red);
                break;
                
            case Mode::CREATE_RECT:
                {
                    int x = std::min(startPoint.x, currentPoint.x);
                    int y = std::min(startPoint.y, currentPoint.y);
                    int w = std::abs(currentPoint.x - startPoint.x);
                    int h = std::abs(currentPoint.y - startPoint.y);
                    newShape = std::make_unique<Rectangle>(x, y, w, h, sf::Color::Green);
                }
                break;
                
            case Mode::CREATE_ELLIPSE:
                {
                    int rx = std::abs(currentPoint.x - startPoint.x);
                    int ry = std::abs(currentPoint.y - startPoint.y);
                    newShape = std::make_unique<Ellipse>(
                        startPoint.x, startPoint.y, rx, ry, sf::Color::Blue);
                }
                break;
                
            case Mode::CREATE_TRIANGLE:
                {
                    // Простой треугольник - третья точка вычисляется
                    int x3 = startPoint.x + (currentPoint.x - startPoint.x) / 2;
                    int y3 = startPoint.y - 50;
                    newShape = std::make_unique<Triangle>(
                        startPoint.x, startPoint.y,
                        currentPoint.x, currentPoint.y,
                        x3, y3,
                        sf::Color::Yellow);
                }
                break;
                
            case Mode::CREATE_CIRCLE:
                {
                    int radius = std::abs(currentPoint.x - startPoint.x);
                    newShape = std::make_unique<Circle>(
                        startPoint.x, startPoint.y, radius, sf::Color::Cyan);
                }
                break;
                
            case Mode::CREATE_SQUARE:
                {
                    int side = std::abs(currentPoint.x - startPoint.x);
                    int x = std::min(startPoint.x, currentPoint.x);
                    int y = std::min(startPoint.y, currentPoint.y);
                    newShape = std::make_unique<Square>(x, y, side, sf::Color::Magenta);
                }
                break;
                
            default:
                return;
        }
        
        if (newShape) {
            figuresList.addFigure(std::move(newShape));
            std::cout << "Figure created" << std::endl;
        }
    }
    
    void showHelp() {
        std::cout << "\n=== Graphics Editor Help ===" << std::endl;
        std::cout << "L - Draw Line" << std::endl;
        std::cout << "R - Draw Rectangle" << std::endl;
        std::cout << "E - Draw Ellipse" << std::endl;
        std::cout << "T - Draw Triangle" << std::endl;
        std::cout << "C - Draw Circle" << std::endl;
        std::cout << "S - Draw Square" << std::endl;
        std::cout << "ESC - Select mode" << std::endl;
        std::cout << "F1 - Show this help" << std::endl;
        std::cout << "===========================\n" << std::endl;
    }
    
    void processTextInput() {
        waitingForInput = false;
        std::cout << "Input processed: " << inputBuffer << std::endl;
        inputBuffer.clear();
    }
    
    void draw() {
        // Рисуем все фигуры через DrawingManager
        for (size_t i = 0; i < figuresList.getCount(); ++i) {
            const Shape* shape = figuresList.getFigure(i);
            drawingManager.draw(shape, window);
        }
        
        // Рисуем превью текущей фигуры
        if (isDrawing) {
            sf::Vertex line[2];
            line[0].position = sf::Vector2f(static_cast<float>(startPoint.x), 
                                            static_cast<float>(startPoint.y));
            line[0].color = sf::Color::White;
            line[1].position = sf::Vector2f(static_cast<float>(currentPoint.x), 
                                            static_cast<float>(currentPoint.y));
            line[1].color = sf::Color::White;
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }
        
        // Рисуем инструкцию
        std::string modeText;
        switch (currentMode) {
            case Mode::SELECT: modeText = "Mode: Select"; break;
            case Mode::CREATE_LINE: modeText = "Mode: Draw Line"; break;
            case Mode::CREATE_RECT: modeText = "Mode: Draw Rectangle"; break;
            case Mode::CREATE_ELLIPSE: modeText = "Mode: Draw Ellipse"; break;
            case Mode::CREATE_TRIANGLE: modeText = "Mode: Draw Triangle"; break;
            case Mode::CREATE_CIRCLE: modeText = "Mode: Draw Circle"; break;
            case Mode::CREATE_SQUARE: modeText = "Mode: Draw Square"; break;
        }
        
        instructionText.setString(modeText + " (F1 for help)");
        window.draw(instructionText);
    }
};

#endif
