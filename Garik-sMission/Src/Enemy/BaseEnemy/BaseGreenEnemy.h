#pragma once

#include "../Enemy.h"

/**
 * @brief Класс ABaseGreenEnemy представляет собой конкретный тип зеленого врага в игре.
 */
class ABaseGreenEnemy : public AEnemy
{
public:
    /**
     * @brief Конструктор для инициализации зеленого врага.
     * 
     * @param MaxPatrolDistance Максимальное расстояние патрулирования врага.
     * @param StartPosition Начальная позиция врага.
     */
    ABaseGreenEnemy(float MaxPatrolDistance, const sf::Vector2f& StartPosition);

    /**
     * @brief Деструктор для освобождения ресурсов.
     */
    ~ABaseGreenEnemy() override = default;

    /**
     * @brief Инициализирует зеленого врага текстурами и анимациями.
     * 
     * @param SpriteManager Ссылка на менеджер спрайтов для управления текстурами.
     */
    void InitEnemy(ASpriteManager& SpriteManager) override;
};
