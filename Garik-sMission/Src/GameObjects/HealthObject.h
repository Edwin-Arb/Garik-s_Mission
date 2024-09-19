#pragma once
#include "../Abstract/AActor.h"

class AHealthObject : public AActor
{
public:
    /**
     * @brief Конструктор для создания объекта лечения с указанием начальной позиции.
     * 
     * @param StartPosition Начальная позиция объекта лечения.
     */
    explicit AHealthObject(const sf::Vector2f& StartPosition);

    /**
     * @brief Деструктор по умолчанию для объекта лечения.
     */
    ~AHealthObject() override = default;

    /**
     * @brief Инициализация объекта лечения.
     * 
     * Загружает необходимые спрайты и текстуры для объекта лечения.
     * 
     * @param SpriteManager Менеджер спрайтов для загрузки текстур.
     */
    void InitHealthObject(ASpriteManager& SpriteManager);

    /**
     * @brief Установка состояния, был ли объект лечения использован.
     * 
     * @param EatenHeal Логическое значение, указывающее, был ли объект лечения использован.
     */
    void SetIsEatenHeal(bool EatenHeal);

    /**
     * @brief Сброс состояния объекта лечения.
     * 
     * Возвращает объект лечения в начальное состояние.
     */
    void ResetHealObjectState();

    /**
     * @brief Отрисовка объекта лечения.
     * 
     * @param Window Окно, в котором будет отображаться объект лечения.
     */
    void DrawActor(sf::RenderWindow& Window) override;

private:
    bool bIsEatenHeal;               // Флаг, указывает, был ли объект лечения использован
    sf::Vector2f StartPosition;      // Начальная позиция объекта лечения
};
