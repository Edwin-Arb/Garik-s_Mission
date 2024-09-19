#include "Bullet.h"

/**
* @brief Конструктор для инициализации пули.
* 
* @param NewVelocity Флаг, определяющий, имеет ли пуля новую скорость.
* @param BulletSpeed Скорость полёта пули.
* @param BulletDamage Урон наносимый от пули.
* @param BulletTextureRect Прямоугольник текстуры пули.
* @param ShootAt Тип цели, на которую направлена пуля.
* @param DestroyEffect Тип эффекта, который будет проигрываться, когда пуля будет уничтожена.
* @param StartPosition Начальная позиция пули.
* @param SpriteManager Ссылка на менеджер спрайтов для отрисовки.
*/
ABullet::ABullet(bool NewVelocity,
                 const float BulletSpeed,
                 const float BulletDamage,
                 const sf::IntRect& BulletTextureRect,
                 const EBulletType& ShootAt,
                 const EBulletDestroyEffect& DestroyEffect,
                 const sf::Vector2f& StartPosition,
                 ASpriteManager& SpriteManager)
    : BulletDamage(BulletDamage),
      BulletType(ShootAt),
      DestroyEffectType(DestroyEffect)
{
    ABullet::InitBullet(NewVelocity, BulletSpeed, BulletTextureRect, StartPosition, SpriteManager);
}

/**
 * @brief Инициализирует текстуру и создаёт спрайт для пули.
 * 
 * @param NewVelocity Флаг, определяющий, имеет ли пуля новую скорость.
 * @param BulletSpeed Скорость полёта пули.
 * @param BulletTextureRect Прямоугольник текстуры пули.
 * @param StartPosition Начальная позиция пули.
 * @param SpriteManager Ссылка на менеджер спрайтов для отрисовки.
 */
void ABullet::InitBullet(const bool NewVelocity,
                         const float BulletSpeed,
                         const sf::IntRect& BulletTextureRect,
                         const sf::Vector2f& StartPosition,
                         ASpriteManager& SpriteManager)
{
    // Инициализация переменных для текстуры пули
    const std::string BulletTexturePath = ASSETS_PATH + "MainTiles/Bullets.png";
    const sf::Vector2f BulletSize = {
        static_cast<float>(BulletTextureRect.getSize().x),
        static_cast<float>(BulletTextureRect.getSize().y)
    };
    const sf::Vector2f BulletOrigin = {0.5f, 0.5f};

    // Установить направление, откуда начать движение
    ActorVelocity.x = NewVelocity ? BulletSpeed : -BulletSpeed;

    // Устанавливаем размер коллизии для пули
    ActorCollisionRect = {StartPosition.x, StartPosition.y, BULLET_SIZE.x, BULLET_SIZE.y};

    // Инициализировать текстуру для пули и создать спрайт для неё
    AActor::InitActorTexture(BulletTexturePath, BulletTextureRect, BulletSize, BulletOrigin, SpriteManager);

    // Задать стартовую позицию пули
    ActorSprite.setPosition(StartPosition);

    sf::Vector2f ScaleBullet = NewVelocity ? sf::Vector2f(0.8f, 0.8f) : sf::Vector2f(-0.8f, 0.8f);
    ActorSprite.setScale(ScaleBullet);
}

/**
* @brief Обновляет позицию пули на основе прошедшего времени.
* 
* @param DeltaTime Время, прошедшее с последнего обновления.
*/
void ABullet::UpdateBulletPosition(float DeltaTime)
{
    // Обновляем положение пули с учетом скорости и времени
    ActorCollisionRect.left += ActorVelocity.x * DeltaTime;
    ActorCollisionRect.top -= ActorVelocity.y * DeltaTime;

    // Устанавливаем положение спрайта с коллизией
    ActorDrawPosition = {ActorCollisionRect.left, ActorCollisionRect.top};

    ActorSprite.setPosition(ActorDrawPosition);
}

/**
 * @brief Отрисовка пули на экране.
 * 
 * @param Window Окно для отрисовки.
 */
void ABullet::DrawActor(sf::RenderWindow& Window)
{
    Window.draw(ActorSprite);
}

/**
* @brief Возвращает урон, наносимый пулей.
* 
* @return Значение урона пули.
*/
float ABullet::GetBulletDamage() const
{
    return BulletDamage;
}

/**
 * @brief Получение типа пули.
 * 
 * @return Тип пули (стрелять в игрока или во врага).
 */
EBulletType ABullet::GetBulletType() const
{
    return BulletType;
}

/**
* @brief Получение типа эффекта для пули, чтобы узнать .
* 
* @return Тип эффекта разрушения пули.
*/
EBulletDestroyEffect ABullet::GetBulletDestroyEffectType() const
{
    return DestroyEffectType;
}
