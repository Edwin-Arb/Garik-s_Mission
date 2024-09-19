#pragma once
#include "../Enemy.h"

/**
 * @brief Класс ABaseRedEnemy представляет собой конкретный тип красного врага в игре.
 */
class ABaseRedEnemy : public AEnemy
{
public:
    /**
     * @brief Конструктор для инициализации красного врага.
     * 
     * @param MaxPatrolDistance Максимальное расстояние патрулирования врага.
     * @param StartPosition Начальная позиция врага.
     */
    ABaseRedEnemy(float MaxPatrolDistance, const sf::Vector2f& StartPosition);

    /**
     * @brief Деструктор для освобождения ресурсов.
     */
    ~ABaseRedEnemy() override = default;

    /**
     * @brief Инициализирует красного врага текстурами и анимациями.
     * 
     * @param SpriteManager Ссылка на менеджер спрайтов для управления текстурами.
     */
    void InitEnemy(ASpriteManager& SpriteManager) override;
};
