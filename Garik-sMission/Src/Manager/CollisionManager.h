#pragma once

#include "../Enemy/Enemy.h"
#include "../GameMain/GameState.h"
#include "../GameObjects/KeyObject.h"
#include "../GameMap/GameMap.h"
#include "../Manager/ParticleSystemManager.h"

// Преждевременное объявление(форвард декларейшен)
class AGameState;
class AKeyObject;
class AChestObject;

/**
 * @brief Менеджер коллизий для персонажа и врагов.
 * 
 * Класс ACollisionManager отвечает за проверку коллизий персонажа с игровой картой и пулями,
 * а также врагов с игровой картой.
 */
class ACollisionManager
{
public:
    /**
     * @brief Конструктор класса ACollisionManager.
     * 
     * Инициализирует ссылки на персонажа и игровую карту.
     * 
     * @param GameState Ссылка на объект состояния игры.
     * @param Player Ссылка на объект персонажа.
     * @param GameMap Ссылка на объект игровой карты.
     */
    ACollisionManager(AGameState& GameState, APlayer& Player, AGameMap& GameMap);

    /**
     * @brief Деструктор класса ACollisionManager.
     */
    ~ACollisionManager() = default;
 
    /**
     * @brief Проверяет столкновения всех пуль с врагами и игровой картой.
     * 
     * @param BulletsVectorPtr Указатель на вектор пуль.
     * @param EnemyVectorPtr Указатель на вектор врагов.
     * @param KeysVectorPtr Указатель на вектор ключей.
     * @param Player Ссылка на персонажа.
     * @param SpriteManager Ссылка на менеджер спрайтов.
     * @param SoundManager Ссылка на менеджер звуков.
     * @param ParticleSystem Ссылка на менеджер частиц.
     */
    void CheckAllBulletCollisions(std::vector<ABullet*>& BulletsVectorPtr,
                                  std::vector<AEnemy*>& EnemyVectorPtr,
                                  std::vector<AKeyObject*>& KeysVectorPtr,
                                  APlayer& Player, ASpriteManager& SpriteManager,
                                  ASoundManager& SoundManager, AParticleSystemManager& ParticleSystem) const;

    /**
     * @brief Обрабатывает столкновение пешки с игровой картой.
     * 
     * @param PawnRect Прямоугольник коллизии персонажа.
     * @param ObjectVelocity Вектор скорости объекта персонажа.
     * @param bCanJump Может ли персонаж прыгать.
     * @param bCanClimb Может ли персонаж карабкаться.
     * @param SoundManager Указатель на менеджер звуков (может быть nullptr).
     */
    void HandlePawnCollisionWithGameMap(sf::FloatRect& PawnRect,
                                        sf::Vector2f& ObjectVelocity,
                                        bool& bCanJump,
                                        bool& bCanClimb,
                                        ASoundManager* SoundManager = nullptr) const;

    /**
     * @brief Проверяет столкновение с ключами.
     * 
     * @param KeysVectorPtr Указатель на вектор ключей.
     * @param SoundManager Ссылка на менеджер звуков.
     */
    void CheckCollisionWithKey(std::vector<AKeyObject*>& KeysVectorPtr, ASoundManager& SoundManager);

    /**
     * @brief Проверяет столкновение с игровой картой.
     * 
     * @param ActorRect Прямоугольник коллизии актера.
     * @param ObjectVelocity Вектор скорости объекта.
     * @param EnemyPtr Указатель на врага.
     */
    void CheckCollisionWithGameMap(sf::FloatRect& ActorRect, sf::Vector2f& ObjectVelocity, AEnemy* EnemyPtr) const;
 
private:
    /**
     * @brief Проверяет столкновение пули с игровой картой.
     * 
     * @param Bullet Пуля для проверки столкновения.
     * 
     * @return true, если есть столкновение с игровой картой, иначе false.
     */
    bool CheckBulletCollisionWithGameMap(AActor& Bullet) const;
   
    /**
     * @brief Проверяет столкновение пули с объектом (персонажем или врагом).
     * 
     * @param Bullet Пуля для проверки столкновения.
     * @param PawnRect Прямоугольник объекта для проверки.
     * 
     * @return true, если есть столкновение с объектом, иначе false.
     */
    bool CheckBulletCollisionWithPawn(AActor& Bullet, const sf::FloatRect& PawnRect) const;
 
public:
    /**
     * @brief Проверяет пересечение двух прямоугольников.
     * 
     * @param Rect1 Первый прямоугольник.
     * @param Rect2 Второй прямоугольник.
     * 
     * @return true, если прямоугольники пересекаются, иначе false.
     */
    bool CheckCollision(const sf::FloatRect& Rect1, const sf::FloatRect& Rect2);

    /**
     * @brief Получает границы камеры.
     * 
     * @return sf::FloatRect Прямоугольник границ камеры.
     */
    sf::FloatRect GetCameraBounds() const;

private:
    AGameState& GameStateRef;   // Ссылка на объект состояния игры.
    APlayer& PlayerRef;         // Ссылка на объект персонажа.
    AGameMap& GameMapRef;       // Ссылка на объект игровой карты.
};
