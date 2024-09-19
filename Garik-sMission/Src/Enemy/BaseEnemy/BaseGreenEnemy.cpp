#include "BaseGreenEnemy.h"

/**
 * @brief Конструктор для инициализации зеленого врага.
 * 
 * @param MaxPatrolDistance Максимальное расстояние патрулирования врага.
 * @param StartPosition Начальная позиция врага.
 */
ABaseGreenEnemy::ABaseGreenEnemy(float MaxPatrolDistance, const sf::Vector2f& StartPosition)
    : AEnemy(MaxPatrolDistance, StartPosition)
{
}

/**
 * @brief Инициализирует зеленого врага текстурами и создает спрайт для него.
 * 
 * @param SpriteManager Ссылка на менеджер спрайтов для управления текстурами.
 */
void ABaseGreenEnemy::InitEnemy(ASpriteManager& SpriteManager)
{
    AEnemy::InitEnemy(SpriteManager);

    // Инициализация переменных для текстуры врага
    const sf::IntRect EnemyRectTexture = sf::IntRect(36, 1, static_cast<int>(ENEMY_SIZE.x),
                                                     static_cast<int>(ENEMY_SIZE.y));

    // Инициализировать текстуру для врага и создать спрайт для неё.
    InitActorTexture(EnemyTexturePath, EnemyRectTexture, ActorSize, EnemyOrigin, SpriteManager);

    WalkAnimation.AnimTexture.loadFromFile(EnemyTexturePath);
    WalkAnimation.FrameSpeed = 5.f;
    WalkAnimation.FrameRect.emplace_back(EnemyRectTexture);
    WalkAnimation.FrameRect.emplace_back(52, 1, static_cast<int>(ENEMY_SIZE.x), static_cast<int>(ENEMY_SIZE.y));
}
