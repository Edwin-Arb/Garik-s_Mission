#pragma once
#include "../Enemy.h"
#include "../../GameObjects/KeyObject.h"

/**
 * @brief Класс ABossEnemy представляет собой босса в игре.
 */
class ABossEnemy : public AEnemy
{
public:
    /**
     * @brief Конструктор для инициализации босса.
     * 
     * @param MaxPatrolDistance Максимальная дистанция патрулирования босса.
     * @param StartPosition Начальная позиция босса.
     */
    ABossEnemy(float MaxPatrolDistance, const sf::Vector2f& StartPosition);

    /**
     * @brief Деструктор для освобождения ресурсов.
     */
    ~ABossEnemy() override = default;

    /**
     * @brief Инициализирует босса текстурами и анимациями.
     * 
     * @param SpriteManager Менеджер спрайтов для загрузки ресурсов.
     */
    void InitEnemy(ASpriteManager& SpriteManager) override;

    /**
     * @brief Выполняет выстрелы босса.
     * 
     * @param SpawnBulletOffsetY Смещение по оси Y для спавна пули.
     * @param BulletsVectorPtr Указатель на вектор пуль, в который добавляются выстрелы.
     * @param SpriteManager Менеджер спрайтов для управления спрайтами.
     */
    void EnemyShoot(const float SpawnBulletOffsetY,
                    std::vector<ABullet*>& BulletsVectorPtr,
                    ASpriteManager& SpriteManager) override;

    /**
     * @brief Когда босс умирает, то спавнем на его месте ключ.
     * 
     * @param KeysVector Вектор ключей.
     * @param SpriteManager Спрайт менеджер.
     * @param BossPosition Берём текущию позицию босса, чтобы в том месте засправнить ключ
     */
    void OnDeath(std::vector<AKeyObject*>& KeysVector,
                 ASpriteManager& SpriteManager,
                 const sf::Vector2f& BossPosition);
};
