#include "Slider.h"


/**
* @brief Конструктор для класса ASlider.
*
* @param Min Минимальное значение ползунка.
* @param Max Максимальное значение ползунка.
* @param BackgroundTexture Текстура фона ползунка.
* @param KnobTexture Текстура ручки ползунка.
* @param Margin Отступ между ручкой и краем ползунка.
*/
ASlider::ASlider(float Min, float Max, const sf::Texture& BackgroundTexture, const sf::Texture& KnobTexture, float Margin)
    : bIsDragging(false),
      MinValue(Min),
      MaxValue(Max),
      CurrentValue(Min),
      Margin(Margin)
{
    // Устанавливаем текстуры и позиции
    sf::IntRect BackgroundRect(6, 0, 76, 15);
    Background.setTexture(BackgroundTexture);
    Background.setTextureRect(BackgroundRect);
    
    sf::IntRect KnobRect(0, 0, 5, 15);
    Knob.setTexture(KnobTexture);
    Knob.setTextureRect(KnobRect);

    // Устанавливаем начальную позицию ручки
    SetValue(CurrentValue);
}


/**
* @brief Обрабатывает событие для ползунка, например, нажатие мыши.
*
* @param Event Событие, которое нужно обработать.
* @param MousePosition Позиция мыши на экране.
*/
void ASlider::HandleEvent(const sf::Event& Event, const sf::Vector2f& MousePosition)
{
    static sf::Vector2f Offset;

    if (Event.type == sf::Event::MouseButtonPressed)
    {
        if (Knob.getGlobalBounds().contains(MousePosition))
        {
            bIsDragging = true;
            // Вычисляем смещение между курсором и левым верхним углом ручки
            Offset = MousePosition - Knob.getPosition();
        }
    }
    else if (Event.type == sf::Event::MouseButtonReleased)
    {
        bIsDragging = false;
    }

    if (bIsDragging && Event.type == sf::Event::MouseMoved)
    {
        // Перемещаем ручку, учитывая смещение и отступы
        float NewPositionX = std::clamp(
            MousePosition.x - Offset.x,
            Background.getPosition().x + Margin,
            Background.getPosition().x + Background.getGlobalBounds().width - Knob.getGlobalBounds().width - Margin
        );

        Knob.setPosition(NewPositionX, Knob.getPosition().y);

        // Обновляем текущее значение с учетом отступов
        CurrentValue = MinValue + (MaxValue - MinValue) *
        ((NewPositionX - (Background.getPosition().x + Margin)) /
            (Background.getGlobalBounds().width - Knob.getGlobalBounds().width - 2 * Margin));
    }
}

/**
* @brief Устанавливает позицию ползунка на экране.
*
* @param NewPosition Новая позиция ползунка.
*/
void ASlider::SetSliderPosition(const sf::Vector2f& NewPosition)
{
    // Вычисляем текущее смещение ручки относительно фона
    float KnobOffsetX = Knob.getPosition().x - Background.getPosition().x;

    // Обновляем позицию фона
    Background.setPosition(NewPosition);

    // Обновляем позицию ручки с учётом сохранённого смещения
    Knob.setPosition(NewPosition.x + KnobOffsetX, NewPosition.y);
}

/**
* @brief Устанавливает текущее значение ползунка.
*
* @param Value Новое значение ползунка.
*/
void ASlider::SetValue(float Value)
{
    CurrentValue = std::clamp(Value, MinValue, MaxValue);
    
    float PositionX = Background.getPosition().x + Margin + 
                      ((CurrentValue - MinValue) / (MaxValue - MinValue)) *
                      (Background.getGlobalBounds().width - Knob.getGlobalBounds().width - 2 * Margin);
    
    Knob.setPosition(PositionX, Background.getPosition().y);
}

/**
* @brief Отрисовывает ползунок на экране.
*
* @param Window Окно, в котором происходит отрисовка.
*/
void ASlider::DrawSlider(sf::RenderWindow& Window) const
{
    Window.draw(Background);
    Window.draw(Knob);
}

/**
* @brief Получает текущее значение ползунка.
*
* @return Текущее значение ползунка.
*/
float ASlider::GetValue() const
{
    return CurrentValue;
}