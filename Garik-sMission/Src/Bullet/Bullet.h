#pragma once
#include "../Abstract/AActor.h"
#include "../Manager/SpriteManager.h"
#include "../Manager/AnimationManager.h"

/**
 * @brief Перечисление, определяющее различные типы пуль.
 */
enum class EBulletDestroyEffect
{
    EBDE_BulletDefaultDestroy,
    EBDE_RocketDestroy,
};

/**
 * @brief Перечисление, определяющее в кого летит пуля.
 */
enum class EBulletType
{
    EBT_ShootAtPlayer, // Пуля направлена на игрока.
    EBT_ShootAtEnemy // Пуля направлена на врага.
};

/**
 * @brief Класс ABullet представляет собой сущность пули в игре.
 * Пуля может двигаться с заданной скоростью, проверять коллизии
 * с другими объектами и отрисовываться на экране.
 */
class ABullet : public AActor
{
public:
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
    ABullet(bool NewVelocity,
            const float BulletSpeed,
            const float BulletDamage,
            const sf::IntRect& BulletTextureRect,
            const EBulletType& ShootAt,
            const EBulletDestroyEffect& DestroyEffect,
            const sf::Vector2f& StartPosition,
            ASpriteManager& SpriteManager);

    /**
     * @brief Деструктор для освобождения ресурсов.
     */
    ~ABullet() override = default;

    /**
     * @brief Инициализирует текстуру и создаёт спрайт для пули.
     * 
     * @param NewVelocity Флаг, определяющий, имеет ли пуля новую скорость.
     * @param BulletSpeed Скорость полёта пули.
     * @param BulletTextureRect Прямоугольник текстуры пули.
     * @param StartPosition Начальная позиция пули.
     * @param SpriteManager Ссылка на менеджер спрайтов для отрисовки.
     */
    virtual void InitBullet(const bool NewVelocity,
                            const float BulletSpeed,
                            const sf::IntRect& BulletTextureRect,
                            const sf::Vector2f& StartPosition,
                            ASpriteManager& SpriteManager);

    /**
     * @brief Обновляет позицию пули на основе прошедшего времени.
     * 
     * @param DeltaTime Время, прошедшее с последнего обновления.
     */
    void UpdateBulletPosition(float DeltaTime);

    /**
     * @brief Отрисовывает пулю на указанном окне отрисовки.
     * 
     * @param Window Окно отрисовки, на котором отображается пуля.
     */
    void DrawActor(sf::RenderWindow& Window) override;
 
    /**
     * @brief Возвращает урон, наносимый пулей.
     * 
     * @return Значение урона пули.
     */
    float GetBulletDamage() const;

    /**
     * @brief Получение типа пули.
     * 
     * @return Тип пули (стрелять в игрока или во врага).
     */
    EBulletType GetBulletType() const;

    /**
     * @brief Получение типа эффекта для пули, чтобы узнать .
     * 
     * @return Тип эффекта разрушения пули.
     */
    EBulletDestroyEffect GetBulletDestroyEffectType() const;

private:
    const float BulletDamage;                  // Урон, наносимый пулей.
 
    EBulletType BulletType;                    // Тип пули (игрок или враг).
    EBulletDestroyEffect DestroyEffectType;    // Какой эффекту уничтожения проигрывать
};
