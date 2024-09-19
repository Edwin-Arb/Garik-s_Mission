#pragma once
#include "../Abstract/AActor.h"
#include "../Manager/AnimationManager.h"

class AGameState;
class ASoundManager;

class AIntroDialogue : public AActor
{
public:
    /**
     * @brief Конструктор класса AIntroDialogue.
     * 
     * Инициализирует начальные параметры для вступительного диалога.
     */
    AIntroDialogue();

    /**
     * @brief Деструктор класса AIntroDialogue.
     * 
     * Используется деструктор по умолчанию.
     */
    ~AIntroDialogue() override = default;

    /**
     * @brief Инициализация диалога.
     * 
     * Устанавливает начальные параметры и состояние анимации диалога.
     */
    void InitDialogue();

    /**
     * @brief Проверяет завершен ли диалог.
     * 
     * @return Возвращает true, если диалог завершен, иначе false.
     */
    bool isDialogueFinished() const;
 
    /**
     * @brief Обновление состояния диалога.
     * 
     * Обновляет текущий кадр анимации и обработку логики диалога.
     * 
     * @param DeltaTime Время, прошедшее между кадрами.
     */
    void UpdateDialogue(float DeltaTime);

    /**
     * @brief Переключает кадр анимации диалога.
     * 
     * Обрабатывает событие ввода и управляет переключением кадров диалога.
     * 
     * @param Event Событие ввода, содержащее информацию о действиях пользователя.
     * @param SoundManager Менеджер звука для управления звуковыми эффектами.
     */
    void SwitchNextFrame(const sf::Event& Event, ASoundManager& SoundManager);

    /**
     * @brief Устанавливает новую позицию для диалога.
     * 
     * Перемещает анимацию диалога в указанную позицию.
     * 
     * @param NewPosition Новая позиция диалога.
     */
    void SetDialoguePosition(const sf::Vector2f& NewPosition);

    /**
     * @brief Отрисовывает диалог.
     * 
     * Рендерит анимацию и элементы диалога на экране.
     * 
     * @param Window Окно, в котором происходит отрисовка.
     */
    void DrawActor(sf::RenderWindow& Window) override;

    /**
     * @brief Получает анимацию диалога.
     * 
     * @return Возвращает объект анимации диалога.
     */
    AAnimationManager GetDialogueAnimation() const;

private:
    bool bDialogueFinished;                   // Флаг завершения диалога

    sf::Texture MouseTexture;                 // Текстура для указателя мыши
    sf::Sprite MouseSprite;                   // Спрайт для отображения указателя мыши

    AAnimationManager DialogueAnimation;      // Анимация для отображения диалога

    sf::Clock DelayBetweenMessages;           // Таймер для задержки между сообщениями диалога
};
