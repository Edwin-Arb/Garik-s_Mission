#pragma once
#include "../Constants.h"
#include "../Manager/SpriteManager.h"

/**
 * @brief Класс для отображения полосы здоровья.
 */
class AHealthBar
{
public:
    /**
     * @brief Конструктор для класса AHealthBar.
     */
    AHealthBar() = default;

    /**
     * @brief Деструктор класса AHealthBar.
     */
    ~AHealthBar() = default;
    
    /**
     * @brief Инициализация полосы здоровья.
     *
     * @param Size Размер полосы здоровья.
     * @param FillColor Цвет заполнения полосы здоровья.
     * @param BackgroundColor Цвет фона полосы здоровья.
     * @param SpriteManager Менеджер спрайтов для загрузки текстур.
     */
    void InitHealthBar(const sf::Vector2f& Size,
                       const sf::Color& FillColor,
                       const sf::Color& BackgroundColor,
                       ASpriteManager& SpriteManager);

    /**
     * @brief Устанавливает позицию полосы здоровья на экране.
     *
     * @param Position Новая позиция полосы здоровья.
     */
    void SetHealthBarPosition(const sf::Vector2f& Position);

    /**
     * @brief Обновляет состояние полосы здоровья в зависимости от текущего и максимального здоровья.
     *
     * @param CurrentHealth Текущее здоровье.
     * @param MaxHealth Максимальное здоровье.
     */
    void UpdateHealthBar(float CurrentHealth, float MaxHealth);

    /**
     * @brief Отрисовывает полосу здоровья на экране.
     *
     * @param Window Окно, в котором происходит отрисовка.
     */
    void DrawHealthBar(sf::RenderWindow& Window) const;

private:
    sf::RectangleShape BackgroundBarRect;     // Прямоугольник фона полосы здоровья
    sf::RectangleShape HealthBarRect;         // Прямоугольник заполнения полосы здоровья
};
