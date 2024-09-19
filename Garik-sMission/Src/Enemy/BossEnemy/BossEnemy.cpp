#include "BossEnemy.h"

/**
 * @brief Конструктор для инициализации босса.
 * 
 * @param MaxPatrolDistance Максимальная дистанция патрулирования босса.
 * @param StartPosition Начальная позиция босса.
 */
ABossEnemy::ABossEnemy(float MaxPatrolDistance, const sf::Vector2f& StartPosition)
    : AEnemy(MaxPatrolDistance, StartPosition)
{
    // Установка параметров босса
    PawnMaxHealth = BOSS_ENEMY_MAX_HEALTH;
    PawnCurrentHealth = BOSS_ENEMY_MAX_HEALTH;
    PawnSpeed = ENEMY_SPEED;

    EnemyScale = {1.5f};

    ActorSize = {15.f * DRAW_SCALE.x, 22.f * DRAW_SCALE.y};

    ActorCollisionRect = {
        StartPosition.x, StartPosition.y,
        ActorSize.x, ActorSize.y
    };

    EnemyHealthBarSize = sf::Vector2f(17.f, 3.f);

    // Задержка между выстрелами для босса
    ShootingDelay = 0.55f;
}

/**
 * @brief Инициализирует босса текстурами и анимациями.
 * 
 * @param SpriteManager Менеджер спрайтов для загрузки ресурсов.
 */
void ABossEnemy::InitEnemy(ASpriteManager& SpriteManager)
{
    // Инициализация базового класса
    AEnemy::InitEnemy(SpriteManager);

    // Установка текстуры и анимации для босса
    const sf::IntRect BossRectTexture = sf::IntRect(48, 33, static_cast<int>(ENEMY_SIZE.x),
                                                    static_cast<int>(ENEMY_SIZE.y));

    InitActorTexture(EnemyTexturePath, BossRectTexture, ActorSize, EnemyOrigin, SpriteManager);

    WalkAnimation.AnimTexture.loadFromFile(EnemyTexturePath);
    WalkAnimation.FrameSpeed = 5.f;
    WalkAnimation.FrameRect.emplace_back(BossRectTexture);
    WalkAnimation.FrameRect.emplace_back(32, 33, static_cast<int>(ENEMY_SIZE.x), static_cast<int>(ENEMY_SIZE.y));
}

/**
 * @brief Выполняет выстрелы босса.
 * 
 * @param SpawnBulletOffsetY Смещение по оси Y для спавна пули.
 * @param BulletsVectorPtr Указатель на вектор пуль, в который добавляются выстрелы.
 * @param SpriteManager Менеджер спрайтов для управления спрайтами.
 */
void ABossEnemy::EnemyShoot(const float SpawnBulletOffsetY,
                            std::vector<ABullet*>& BulletsVectorPtr,
                            ASpriteManager& SpriteManager)
{
    // Создаем пулю-рокету с особым эффектом "взрыва рокеты" для босса
    if (bIsPlayerDetected)
    {
        BulletsVectorPtr.emplace_back(new ABullet(bIsMoveRight, BULLET_SPEED, ROCKET_DAMAGE,
                                                  sf::IntRect(3, 7, static_cast<int>(BULLET_SIZE.x) + 8,
                                                              static_cast<int>(BULLET_SIZE.y) + 2),
                                                  EBulletType::EBT_ShootAtPlayer,
                                                  EBulletDestroyEffect::EBDE_RocketDestroy,
                                                  sf::Vector2f(
                                                      ActorCollisionRect.left + (bIsMoveRight ? ActorSize.x : 0.f),
                                                      ActorCollisionRect.top + 15.f),
                                                  SpriteManager));
    }
}

/**
* @brief Когда босс умирает, то спавнем на его месте ключ.
* 
* @param KeysVector Вектор ключей.
* @param SpriteManager Спрайт менеджер.
* @param BossPosition Берём текущию позицию босса, чтобы в том месте засправнить ключ
*/
void ABossEnemy::OnDeath(std::vector<AKeyObject*>& KeysVector,
                         ASpriteManager& SpriteManager,
                         const sf::Vector2f& BossPosition)
{
    // Создаем ключ на позиции босса
    AKeyObject* NewKey = new AKeyObject(BossPosition);

    // Ключи создаются по порядку, не зависимо какой босс умрёт первый
    NewKey->InitKeys(SpriteManager);
    ++NewKey->KeysCountFound;
    KeysVector.emplace_back(NewKey);
}
