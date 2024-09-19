#include "HealthBar.h"


/**
* @brief Инициализация полосы здоровья.
*
* @param Size Размер полосы здоровья.
* @param FillColor Цвет заполнения полосы здоровья.
* @param BackgroundColor Цвет фона полосы здоровья.
* @param SpriteManager Менеджер спрайтов для загрузки текстур.
*/
void AHealthBar::InitHealthBar(const sf::Vector2f& Size,
                               const sf::Color& FillColor,
                               const sf::Color& BackgroundColor,
                               ASpriteManager& SpriteManager)
{
    BackgroundBarRect.setSize(Size);
    BackgroundBarRect.setFillColor(BackgroundColor);

    HealthBarRect.setSize(Size);
    HealthBarRect.setFillColor(FillColor);

    SpriteManager.SetShapeRelativeOrigin(BackgroundBarRect, 0.5f, 0.5f);
    SpriteManager.SetShapeRelativeOrigin(HealthBarRect, 0.5f, 0.5f);
}

/**
* @brief Устанавливает позицию полосы здоровья на экране.
*
* @param Position Новая позиция полосы здоровья.
*/
void AHealthBar::SetHealthBarPosition(const sf::Vector2f& Position)
{
    BackgroundBarRect.setPosition(Position);
    HealthBarRect.setPosition(Position);
}

/**
* @brief Обновляет состояние полосы здоровья в зависимости от текущего и максимального здоровья.
*
* @param CurrentHealth Текущее здоровье.
* @param MaxHealth Максимальное здоровье.
*/
void AHealthBar::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
    float HealthPercentage = CurrentHealth / MaxHealth;
    sf::Vector2f Size = BackgroundBarRect.getSize();
    HealthBarRect.setSize(sf::Vector2f(Size.x * HealthPercentage, Size.y));
}

/**
* @brief Отрисовывает полосу здоровья на экране.
*
* @param Window Окно, в котором происходит отрисовка.
*/
void AHealthBar::DrawHealthBar(sf::RenderWindow& Window) const
{
    Window.draw(BackgroundBarRect);
    Window.draw(HealthBarRect);
}
