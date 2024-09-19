// ReSharper disable All
#pragma once
#include <SFML/Graphics/View.hpp>
#include "../Manager/CollisionManager.h"
#include "../Manager/FpsManager.h"
#include "../Manager/ParticleSystemManager.h"

// Подключаем разные состояния игры
#include "IntroDialogue.h"
#include "GameOver.h"
#include "CreditsScreen.h"

// Подключаем пользовательский интерфейс
#include "../UserInterface/UIText.h"
#include "../UserInterface/Menu/Menu.h"

// Подключаем объекты, с которыми персонаж заимодействует
#include "../GameObjects/FinalGateObject.h"
#include "../GameObjects/KeyObject.h"
#include "../GameObjects/ChestObject.h"
#include "../GameObjects/HealthObject.h"

// Подключаем типов врагов
#include "../Enemy/BossEnemy/BossEnemy.h"
#include "../Enemy/BaseEnemy/BaseRedEnemy.h"
#include "../Enemy/BaseEnemy/BaseBlueEnemy.h"
#include "../Enemy/BaseEnemy/BaseGreenEnemy.h"

/**
 * @brief Перечисление, определяющее различные состояния игры
 */
enum class EGameState
{
    EGS_Menu,
    EGS_IntroDialogue,
    EGS_Playing,
    EGS_GameOver,
    EGS_EscapeRoute,
    EGS_MissionCompleted,
    EGS_Pause,
    EGS_Options,
    EGS_Exit,
};

/**
 * @brief Класс AGameState представляет текущее состояние игры.
 * Он управляет инициализацией игры, обновлением ввода, игрового процесса, камеры и отрисовкой.
 */
class AGameState
{
public:
    /**
     * @brief Конструктор класса AGameState.
     */
    AGameState();

    /**
     * @brief Деструктор класса AGameState.
     */
    ~AGameState();

    /**
     * @brief Инициализирует начальное состояние игры, включая игровые объекты и менеджеры.
     */
    void InitGame();

    /**
     * @brief Устанавливает флаг, показывающий, идет ли игровой процесс.
     * 
     * @param IsPlayState Состояние игрового процесса (true - идет, false - остановлен).
     */
    void SetIsPlayState(bool IsPlayState);

    /**
     * @brief Обрабатывает взаимодействие игрока с кнопками (например, меню или пауза).
     * 
     * @param Event Событие SFML, описывающее взаимодействие (нажатие кнопки).
     * @param Window Окно, в котором происходит игра.
     */
    void HandleButtonInteraction(const sf::Event& Event, sf::RenderWindow& Window);

    /**
     * @brief Устанавливает новое состояние игры.
     * 
     * @param NewGameState Новое состояние игры.
     */
    void SetGameState(const EGameState& NewGameState);

    /**
     * @brief Обновляет ввод пользователя.
     * 
     * @param DeltaTime Время, прошедшее с последнего кадра.
     */
    void UpdateInput(float DeltaTime);

    /**
     * @brief Обновление игрового процесса.
     * 
     * Этот метод обновляет состояние игрового процесса,
     * включая обновление движений персонажей, врагов, пуль и других объектов.
     * Также проверяются столкновения, обновляются текстуры и анимации.
     * 
     * @param DeltaTime Время, прошедшее с последнего обновления.
     */
    void UpdateGameplay(float DeltaTime);

    /**
     * @brief Обновляет камеру игры в зависимости от положения персонажа.
     * 
     * @param Window Окно для отрисовки игры.
     */
    void UpdateCamera(sf::RenderWindow& Window);

    /**
     * @brief Начинает игру с начального состояния.
     */
    void StartGame();

    /**
     * @brief Сбрасывает состояние игры для перезапуска.
     */
    void ResetGame();

    /**
     * @brief Отрисовывает текущее состояние игры в окне.
     * 
     * @param Window Окно для отрисовки игры.
     */
    void DrawGame(sf::RenderWindow& Window) const;
 
    /**
     * @brief Получает текущее состояние игры (игровой процесс активен или нет).
     * 
     * @return true, если игра идет, иначе false.
     */
    bool GetIsPlayState() const;
 
    /**
     * @brief Получает текущее состояние игры.
     * 
     * @return Текущее состояние игры.
     */
    EGameState GetCurrentGameState() const;

    /**
     * @brief Получает предыдущее состояние игры.
     * 
     * @return Предыдущее состояние игры.
     */
    EGameState GetPastGameState() const;

    /**
     * @brief Возвращает таймер состояния Game Over.
     * 
     * @return Таймер для состояния Game Over.
     */
    sf::Clock GetGameOverTimer() const;

    /**
     * @brief Возвращает текущий вид (камеру) игрока.
     * 
     * @return Вид игрока (sf::View).
     */
    sf::View GetViewPlayer() const;

    /**
    * @brief Ссылка на вектор указателей на найденые ключи.
    * 
    * @return Возвращает вектор ключей.
    */
    std::vector<AKeyObject*>& GetFoundKeysVector();

private:
    bool bIsGamePlaying;                                     // Флаг, указывающий, идет ли игровой процесс.

    AIntroDialogue* DialoguePtr;                             // Указатель на диалог при старте игры.
    AGameOver* GameOverPtr;                                  // Указатель на экран Game Over.
    ACreditsScreen* CreditsScreenPtr;                        // Указатель на экран титров после завершения миссии.

    APlayer* PlayerPtr;                                      // Указатель на игрока.
    AGameMap* GameMapPtr;                                    // Указатель на игровую карту.
    std::vector<AEnemy*> EnemyVectorPtr;                     // Вектор врагов.
    std::vector<ABullet*> BulletsVectorPtr;                  // Вектор пуль.

    ASpriteManager* SpriteManagerPtr;                        // Менеджер спрайтов.
    ACollisionManager* CollisionManagerPtr;                  // Менеджер коллизий.
    AParticleSystemManager* ParticleSystemPtr;               // Менеджер частиц.
    ASoundManager* SoundManagerPtr;                          // Менеджер звуков.
    // AFpsManager* FpsManagerPtr;                           // Менеджер FPS. // TODO: Используется только тестов.

    AUIText* GameTextPtr;                                    // Текст для отображения состояния игры.
    AMenu* MenuPtr;                                          // Меню игры.
    AFinalGateObject* FinalGatePtr;                          // Финальные ворота, которые открываются по завершении миссии.
    std::vector<AKeyObject*> KeysVectorPtr;                  // Вектор ключей для выполнения миссий.
    std::vector<AKeyObject*> FoundKeysVectorPtr;             // Вектор указателей на ключи, которые найдены
    std::vector<AChestObject*> ChestVectorPtr;               // Вектор сундуков для выполнения миссий.
    std::vector<AHealthObject*> HealingHamVectorPtr;         // Вектор объектов для восстановления здоровья.

    EGameState CurrentGameState;                             // Текущее состояние игры.
    EGameState PastGameState;                                // Предыдущее состояние игры.

    sf::Clock GameOverTimer;                                 // Таймер для состояния Game Over.
    sf::Clock EscapeTimer;                                   // Таймер для того, чтобы в конце игры фоновая музыка затихала.
    sf::Clock MissionCompletedTimer;                         // Таймер для завершения миссии.
 
    sf::View ViewPlayer;                                     // Камера для отображения игрового мира.
};
