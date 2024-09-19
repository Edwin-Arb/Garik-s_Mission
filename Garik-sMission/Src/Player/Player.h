#pragma once
#include "../Abstract/APawn.h"
#include "../Bullet/Bullet.h"
#include "../Manager/ASoundManager.h"
#include "../Manager/AnimationManager.h"
#include "../Manager/ParticleSystemManager.h"

// Предварительное объявление (Forward declaration)
class ASoundManager;
class ACollisionManager;

/**
 * @brief Класс персонажа, отвечающий за поведение и состояние персонажа.
 */
class APlayer : public APawn
{
public:
    /**
     * @brief Конструктор для класса APlayer.
     */
    APlayer();

    /**
     * @brief Деструктор класса APlayer.
     */
    ~APlayer() override = default;

   /**
    * @brief Инициализация персонажа.
    * Загружает текстуры и настраивает спрайты и шкалу здоровья.
    * 
    * @param SpriteManager Менеджер спрайтов для управления отображением.
    */
    void InitPlayer(ASpriteManager& SpriteManager);

    /**
     * @brief Обработка выстрелов персонажа.
     *
     * @param BulletsVectorPtr Указатель на вектор пуль для добавления новой пули.
     * @param SoundManager Менеджер звуков для воспроизведения звуков выстрелов.
     * @param SpriteManager Менеджер спрайтов для загрузки текстур.
     * @param ParticleSystem Менеджер систем частиц для создания эффектов выстрелов.
     */
    void HandlePlayerShoots(std::vector<ABullet*>& BulletsVectorPtr,
                            ASoundManager& SoundManager,
                            ASpriteManager& SpriteManager,
                            AParticleSystemManager& ParticleSystem) const;

   /**
    * @brief Обработка движения персонажа.
    * Устанавливает скорость персонажа в зависимости от нажатых клавиш.
    * 
    * @param DeltaTime Время, прошедшее с предыдущего кадра.
    */
    void HandlePlayerMove(float DeltaTime);

    /**
     * @brief Применение отталкивания персонажа.
     *
     * @param bFromRight Флаг, указывающий направление отталкивания (справа или слева).
     * @param PushForce Сила отталкивания.
     * @param DeltaTime Время, прошедшее между последними кадрами.
     */
    void ApplyPushBack(bool bFromRight, float PushForce, float DeltaTime);

   /**
    * @brief Обновление позиции и состояния персонажа.
    * Включает в себя обработку гравитации и проверку столкновений с объектами.
    * 
    * @param DeltaTime Время, прошедшее с предыдущего кадра.
    * @param SoundManager Менеджер, отвечаюсь за воспроизведение звука
    */
    void UpdatePlayerMove(float DeltaTime, ASoundManager& SoundManager);

    /**
     * @brief Обработка события попадания ракеты.
     *
     * @param bFromRight Флаг, указывающий направление попадания (справа или слева).
     */
    void OnRocketHit(bool bFromRight);

    /**
     * @brief Установка состояния персонажа (мёртв или нет).
     *
     * @param IsDeath Флаг, указывающий, мёртв ли персонаж.
     */
    void SetIsDeathPlayer(bool IsDeath);

    /**
     * @brief Сброс состояния персонажа.
     */
    void ResetPlayer();

   /**
    * @brief Отрисовка персонажа на экране.
    * Отображает спрайт персонажа, его коллизию и шкалу здоровья.
    * 
    * @param Window Окно, на котором происходит отрисовка.
    */
    void DrawActor(sf::RenderWindow& Window) override;

    /**
     * @brief Получить по ссылке данные, если персонаж находится рядом с лестницей и может карабкаться по ней.
     * 
     * @return Текущее состояние взаимодействия с лестницей.
     */
    bool& GetIsOnLadder();
   
    /**
     * @brief Проверяет, мёртв ли персонаж.
     * 
     * @return true, если персонаж мёртв, иначе false.
     */
    bool GetIsDeathPlayer() const;

private:
    bool bIsUseController;                       // Флаг, может ли игрок управлять персонажем
    bool bIsMoveRight;                           // Флаг, указывающий направление движения персонажа
    bool bIsOnLadder;                            // Флаг, указывающий, находится ли игрок на лестнице
    bool bIsDeath;                               // Флаг, указывающий, мёртв ли персонаж
    bool bIsDeathSoundPlayed;                    // Флаг, чтобы проиграть звук смерти только 1 раз
    bool bIsBeingPushedBack;                     // Флаг, что персонаж отталкивается
    bool bisBulletFromRight;                     // Флаг, с какой стороны летит пуля

    float PlayerJumpSpeed;                       // Скорость прыжка персонажа
    float MaxPushBackTime;                       // Максимальное время для отталкивания (в секундах)

    AAnimationManager IdleAnimation;             // Анимация покоя
    AAnimationManager WalkAnimation;             // Анимация движения
    AAnimationManager JumpUpAnimation;           // Анимация прыжка вверх
    AAnimationManager JumpDownAnimation;         // Анимация прыжка вниз
    AAnimationManager DeathAnimation;            // Анимация смерти персонажа
       
    AAnimationManager* CurrentAnimation;         // Указатель на текущую анимацию персонажа

    sf::Clock PushingTime;                       // Таймер для отслеживания времени отталкивания
};
