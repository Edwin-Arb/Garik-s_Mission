#pragma once
#include "../Enemy.h"

/**
 * @brief Класс ABaseBlueEnemy представляет собой конкретный тип врага в игре.
 */
class ABaseBlueEnemy : public AEnemy
{
public:
    /**
     * @brief Конструктор для инициализации синего врага.
     * 
     * @param MaxPatrolDistance Максимальное расстояние патрулирования врага.
     * @param StartPosition Начальная позиция врага.
     */
    ABaseBlueEnemy(float MaxPatrolDistance, const sf::Vector2f& StartPosition);

    /**
     * @brief Деструктор для освобождения ресурсов.
     */
    ~ABaseBlueEnemy() override = default;

    /**
     * @brief Инициализирует врага текстурами и анимациями.
     * 
     * @param SpriteManager Ссылка на менеджер спрайтов для управления текстурами.
     */
    void InitEnemy(ASpriteManager& SpriteManager) override;

    /**
     * @brief Выполняет действие выстрела для синего врага.
     * 
     * @param SpawnBulletOffsetY Вертикальное смещение для спауна пуль.
     * @param BulletsVectorPtr Ссылка на вектор пуль для управления ими. 
     * @param SpriteManager Ссылка на менеджер спрайтов для управления текстурами.
     */
    void EnemyShoot(const float SpawnBulletOffsetY,
                    std::vector<ABullet*>& BulletsVectorPtr,
                    ASpriteManager& SpriteManager) override;
};
