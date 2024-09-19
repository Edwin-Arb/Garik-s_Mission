#pragma once
#include "../../Constants.h"
#include "../../Manager/AnimationManager.h"
#include "../../Manager/ASoundManager.h"

/**
 * @brief Класс, представляющий кнопку с анимацией и состоянием.
 */
class AButton
{
public:
    /**
     * @brief Конструктор для класса AButton.
     */
    AButton();

    /**
     * @brief Деструктор класса AButton.
     */
    ~AButton() = default;

    /**
     * @brief Инициализирует кнопку с заданными текстурами и кадрами.
     *
     * @param TexturePath Путь к текстуре кнопки.
     * @param DefaultFrame Прямоугольник для нормального состояния кнопки.
     * @param HoverFrame Прямоугольник для состояния наведения на кнопку.
     */
    void InitButton(const std::string& TexturePath,
                    const sf::IntRect& DefaultFrame,
                    const sf::IntRect& HoverFrame);

    /**
     * @brief Устанавливает состояние кнопки (нажата или нет).
     *
     * @param bPressed Флаг, указывающий, нажата ли кнопка.
     */
    void SetPressedState(bool bPressed);

    /**
     * @brief Устанавливает видимость кнопки.
     *
     * @param bVisible Флаг, указывающий, видима ли кнопка.
     */
    void SetVisibility(bool bVisible);

    /**
     * @brief Устанавливает позицию кнопки на экране.
     *
     * @param NewPosition Новая позиция кнопки.
     */
    void SetButtonPosition(const sf::Vector2f& NewPosition);

    /**
     * @brief Обновляет состояние кнопки на основе позиции мыши, события и звукового менеджера.
     *
     * @param MousePosition Позиция мыши на экране.
     * @param SoundManager Менеджер звуков для воспроизведения звуков при взаимодействии с кнопкой.
     * @param Event Событие, связанное с нажатием кнопки.
     */
    void UpdateButton(const sf::Vector2f& MousePosition,
                      ASoundManager& SoundManager,
                      const sf::Event& Event);

    /**
     * @brief Отрисовывает кнопку на экране.
     *
     * @param Window Окно, в котором происходит отрисовка кнопки.
     */
    void DrawButton(sf::RenderWindow &Window) const;
 
    /**
     * @brief Проверяет, нажата ли кнопка.
     * 
     * @return true, если кнопка нажата, иначе false.
     */
    bool GetIsPressedButton() const;
   
    /**
     * @brief Проверяет, видима ли кнопка.
     * 
     * @return true, если кнопка видима, иначе false.
     */
    bool IsVisible() const;
   
    /**
     * @brief Получает состояние анимации кнопки.
     * 
     * @return true, если анимация воспроизводится, иначе false.
     */
    bool GetAnimationState() const;

private:
    bool bIsPressed;                       // Флаг, указывающий, нажата ли кнопка
    bool bIsVisible;                       // Флаг, указывающий, видима ли кнопка
    bool bAnimationPlaying;                // Флаг, указывающий, воспроизводится ли анимация кнопки
 
    sf::Sprite ButtonSprite;               // Спрайт кнопки
    AAnimationManager ButtonAnimation;     // Менеджер анимации кнопки

    sf::Clock AnimationClock;              // Таймер для отслеживания времени анимации
    sf::Time AnimationDuration;            // Продолжительность анимации
};
