#include "BaseBlueEnemy.h"

/**
 * @brief Конструктор для инициализации синего врага.
 * 
 * @param MaxPatrolDistance Максимальное расстояние патрулирования врага.
 * @param StartPosition Начальная позиция врага.
 */
ABaseBlueEnemy::ABaseBlueEnemy(float MaxPatrolDistance, const sf::Vector2f& StartPosition)
    : AEnemy(MaxPatrolDistance, StartPosition)
{
    ActorSize = {ENEMY_SIZE.x + 1, ENEMY_SIZE.y - 1};

    ActorCollisionRect = {
        StartPosition.x, StartPosition.y,
        ActorSize.x, ActorSize.y
    };
}

/**
 * @brief Инициализирует врага текстурами и создаёт спрайт для него.
 * 
 * @param SpriteManager Ссылка на менеджер спрайтов для управления текстурами.
 */
void ABaseBlueEnemy::InitEnemy(ASpriteManager& SpriteManager)
{
    AEnemy::InitEnemy(SpriteManager);

    // Инициализация переменных для текстуры врага
    const sf::IntRect EnemyRectTexture = sf::IntRect(3, 18, static_cast<int>(ActorSize.x),
                                                     static_cast<int>(ActorSize.y));

    // Инициализировать текстуру для врага и создать спрайт для неё.
    InitActorTexture(EnemyTexturePath, EnemyRectTexture, ActorSize, EnemyOrigin, SpriteManager);

    WalkAnimation.AnimTexture.loadFromFile(EnemyTexturePath);
    WalkAnimation.FrameSpeed = 5.f;
    WalkAnimation.FrameRect.emplace_back(EnemyRectTexture);
    WalkAnimation.FrameRect.emplace_back(19, 18, static_cast<int>(ActorSize.x), static_cast<int>(ActorSize.y));
}

/**
 * @brief Выполняет действие выстрела для синего врага.
 * 
 * @param SpawnBulletOffsetY Вертикальное смещение для спауна пуль.
 * @param BulletsVectorPtr Ссылка на вектор пуль для управления ими.
 * @param SpriteManager Ссылка на менеджер спрайтов для управления текстурами.
 */
void ABaseBlueEnemy::EnemyShoot(const float SpawnBulletOffsetY, std::vector<ABullet*>& BulletsVectorPtr,
                                ASpriteManager& SpriteManager)
{
    AEnemy::EnemyShoot(8.f, BulletsVectorPtr, SpriteManager);
}
