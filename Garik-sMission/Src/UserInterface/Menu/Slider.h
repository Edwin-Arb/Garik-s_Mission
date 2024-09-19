#pragma once
#include "../../Constants.h"

/**
 * @brief Класс для создания и управления ползунком.
 */
class ASlider
{
public:
    /**
     * @brief Конструктор для класса ASlider.
     *
     * @param Min Минимальное значение ползунка.
     * @param Max Максимальное значение ползунка.
     * @param BackgroundTexture Текстура фона ползунка.
     * @param KnobTexture Текстура ручки ползунка.
     * @param Margin Отступ между ручкой и краем ползунка.
     */
    ASlider(float Min, float Max, const sf::Texture& BackgroundTexture, const sf::Texture& KnobTexture, float Margin = 8.0f);

    /**
     * @brief Деструктор класса ASlider.
     */
    ~ASlider() = default;

    /**
     * @brief Обрабатывает событие для ползунка, например, нажатие мыши.
     *
     * @param Event Событие, которое нужно обработать.
     * @param MousePosition Позиция мыши на экране.
     */
    void HandleEvent(const sf::Event& Event, const sf::Vector2f& MousePosition);

    /**
     * @brief Устанавливает позицию ползунка на экране.
     *
     * @param NewPosition Новая позиция ползунка.
     */
    void SetSliderPosition(const sf::Vector2f& NewPosition);

    /**
     * @brief Устанавливает текущее значение ползунка.
     *
     * @param Value Новое значение ползунка.
     */
    void SetValue(float Value);

    /**
     * @brief Отрисовывает ползунок на экране.
     *
     * @param Window Окно, в котором происходит отрисовка.
     */
    void DrawSlider(sf::RenderWindow& Window) const;
 
    /**
     * @brief Получает текущее значение ползунка.
     *
     * @return Текущее значение ползунка.
     */
    float GetValue() const;

private:
    bool bIsDragging;            // Флаг, указывающий, что ползунок перетаскивается
    float MinValue;              // Минимальное значение ползунка
    float MaxValue;              // Максимальное значение ползунка
    float CurrentValue;          // Текущее значение ползунка
    float Margin;                // Отступ между ручкой и краем ползунка
 
    sf::Sprite Background;       // Фон слайдера
    sf::Sprite Knob;             // Ручка слайдера
};
