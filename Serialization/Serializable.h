#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <string>
#include <vector>

/**
 * Интерфейс для объектов, которые можно сериализовать
 * Любой класс, который хочет сохраняться в файл, должен реализовать эти методы
 */
class Serializable {
public:
    virtual ~Serializable() {}
    
    /**
     * Сохранить объект в строку (текстовое представление)
     */
    virtual std::string serialize() const = 0;
    
    /**
     * Загрузить объект из строки
     * @return true если успешно, false если ошибка
     */
    virtual bool deserialize(const std::string& data) = 0;
    
    /**
     * Получить тип объекта (для создания правильного класса)
     */
    virtual std::string getType() const = 0;
};

#endif
