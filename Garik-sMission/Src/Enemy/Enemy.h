#pragma once
#include "../Bullet/Bullet.h"
#include "../Player/Player.h"
#include "../GameMap/GameMap.h"


/**
 * @brief Класс, представляющий врага в игре.
 */
class AEnemy : public APawn
{
public:
    /**
     * @brief Конструктор врага.
     * 
     * @param MaxPatrolDistance Максимальная дистанция, на которую может двигаться враг.
     * @param StartPosition Начальная позиция врага.
     */
    AEnemy(const float MaxPatrolDistance, const sf::Vector2f& StartPosition);

    /**
     * @brief Деструктор врага.
     */
    ~AEnemy() override = default;

    /**
     * @brief Инициализация врага.
     * 
     * @param SpriteManager Менеджер спрайтов для загрузки ресурсов.
     */
    virtual void InitEnemy(ASpriteManager& SpriteManager);

    /**
     * @brief Враг открывает огонь.
     *
     * @param SpawnBulletOffsetY Вертикальное смещение для спауна пуль.
     * @param BulletsVectorPtr Указатель на вектор пуль, в который добавляются выстрелы.
     * @param SpriteManager Менеджер спрайтов для управления спрайтами.
     */
    virtual void EnemyShoot(const float SpawnBulletOffsetY,
                            std::vector<ABullet*>& BulletsVectorPtr,
                            ASpriteManager& SpriteManager);

    /**
     * @brief Изменения направления врага.
     */
    virtual void ChangeDirection();

protected:
    /**
     * @brief Вычисление позиции отрисовки врага.
     */
    void CalculateEnemyDrawPosition();

    /**
     * @brief Обнаружение персонажа в окрестности.
     * 
     * @param DeltaTime
     * @param Player Ссылка на объект персонажа.
     * @param GameMap Ссылка на объект игровой карты.
     * @param CollisionManager Менеджер отвечающий за коллизии в игре.
     */
    void DetectPlayer(float DeltaTime,
                      APlayer& Player,
                      AGameMap& GameMap,
                      ACollisionManager& CollisionManager);

    /**
     * @brief Обновление направления и скорости врага.
     * 
     * @param DeltaTime Время, прошедшее с последнего обновления.
     * @param Player Ссылка на объект персонажа.
     */
    void UpdateDirectionAndVelocity(float DeltaTime, APlayer& Player);

    /**
     * @brief Обновление позиции врага на основе текущей скорости.
     */
    void UpdatePosition();

    /**
     * @brief Обновление позиции зоны обнаружения персонажа.
     */
    void UpdateDetectionAreaPosition();

    /**
     * @brief Обновление дистанции, на которую может двигаться враг.
     */
    void UpdateMoveDistance();

public:
    /**
     * @brief Обновление движения врага.
     * 
     * @param DeltaTime Время, прошедшее с последнего обновления.
     * @param Player Ссылка на объект персонажа.
     * @param GameMap Ссылка на объект игровой карты.
     * @param CollisionManager Менеджер коллизий. 
     */
    virtual void UpdateEnemyMove(float DeltaTime,
                                 APlayer& Player,
                                 AGameMap& GameMap,
                                 ACollisionManager& CollisionManager);

    /**
     * @brief Отрисовка врага на экране.
     * 
     * @param Window Окно, в котором происходит отрисовка.
     */
    void DrawActor(sf::RenderWindow& Window) override;

    /**
     * @brief Задержка между выстрелами для врагов.
     * 
     * @return Возвращаем значение задержки.
     */
     float GetShootingDelay() const;

protected:
    bool bIsMoveRight;                          // Флаг направления движения вправо.
    bool bIsPlayerDetected;                     // Флаг обнаружения персонажа.

    float EnemyScale;                           // Масштаб врага.

private:
    float MinMoveDistance;                      // Минимальная дистанция движения врага.
    float MaxMoveDistance;                      // Максимальная дистанция движения врага.
    float TimeAfterDetection;                   // Время, в течение которого враг продолжит патрулировать после потери игрока
    const float DetectionTimeout;               // Время задержки после потери игрока из виду

protected:
    float ShootingDelay;                        // Задержка между выстрелами для врагов
    const std::string EnemyTexturePath;         // Путь к текстуре врага.
    const sf::Vector2f EnemyOrigin;             // Начальный ориентир врага.

    sf::Vector2f EnemyHealthBarSize;            // Размер шкалы здоровья врага.

    AAnimationManager WalkAnimation;            // Менеджер анимации ходьбы.

private:
    sf::Vector2f EnemyStartPosition;            // Начальная позиция врага.
 
    sf::RectangleShape LineTraceDetectionArea; // Прямоугольная область для обнаружения.
};
