#pragma once
#include "../Menu/Button.h"
#include <unordered_map>
#include "Slider.h"

// Предварительное объявление (Forward declaration)
class AGameState;
enum class EGameState;

/**
 * @brief Класс, представляющий меню игры с кнопками и ползунками.
 */
class AMenu
{
public:
    /**
     * @brief Конструктор для класса AMenu.
     */
    AMenu();

    /**
     * @brief Деструктор класса AMenu.
     * Особождаем память.
     */
    ~AMenu();

    /**
     * @brief Инициализирует меню.
     */
    void InitMenu();

    /**
     * @brief Устанавливает позицию меню на экране.
     *
     * @param NewPosition Новая позиция меню.
     */
    void SetMenuPosition(const sf::Vector2f& NewPosition);

    /**
     * @brief Устанавливает позицию размытого экрана.
     *
     * @param NewPosition Новая позиция размытого экрана.
     */
    void SetBlurScreenPosition(const sf::Vector2f& NewPosition);

    /**
     * @brief Устанавливает начальные позиции ползунков громкости.
     *
     * @param StartPosition Начальная позиция ползунков.
     * @param VerticalOffset Вертикальное смещение ползунков.
     */
    void SetVolumeSliderDefaultPositions(const sf::Vector2f& StartPosition, float VerticalOffset);

    /**
     * @brief Устанавливает позицию кнопки по её имени.
     *
     * @param ButtonName Имя кнопки.
     * @param NewPosition Новая позиция кнопки.
     */
    void SetButtonPosition(const std::string& ButtonName, const sf::Vector2f& NewPosition);

    /**
     * @brief Обрабатывает нажатие кнопки.
     *
     * @param ButtonName Имя кнопки, на которую нажали.
     * @param GameState Состояние игры для возможного обновления.
     */
    void HandleButtonPress(const std::string& ButtonName, AGameState& GameState) const;

    /**
     * @brief Обновляет видимость элементов меню в зависимости от состояния игры.
     *
     * @param GameState Состояние игры для определения видимости элементов.
     */
    void UpdateVisibility(const AGameState& GameState);

    /**
     * @brief Обновляет состояние меню на основе позиции мыши, звукового менеджера и события.
     *
     * @param MousePosition Позиция мыши на экране.
     * @param SoundManager Менеджер звуков для обработки звуковых эффектов.
     * @param GameState Состояние игры для возможного обновления.
     * @param Event Событие, связанное с взаимодействием с меню.
     */
    void UpdateMenu(const sf::Vector2f& MousePosition,
                    ASoundManager& SoundManager,
                    AGameState& GameState,
                    const sf::Event& Event);

    /**
     * @brief Обновляет ползунки громкости на основе позиции мыши и события.
     *
     * @param MousePosition Позиция мыши на экране.
     * @param SoundManager Менеджер звуков для обработки звуковых эффектов.
     * @param Event Событие, связанное с взаимодействием с ползунками.
     */
    void UpdateVolumeSlider(const sf::Vector2f& MousePosition,
                            ASoundManager& SoundManager,
                            const sf::Event& Event) const;

    /**
     * @brief Отрисовывает меню на экране.
     *
     * @param Window Окно, в котором происходит отрисовка меню.
     * @param GameState Состояние игры для определения отображаемых элементов.
     */
    void DrawMenu(sf::RenderWindow& Window, const AGameState& GameState) const;
     
    /**
     * @brief Возвращает карту кнопок меню.
     *
     * @return Неизменяемая карта кнопок, где ключи — имена кнопок, а значения — объекты кнопок.
     */
    std::unordered_map<std::string, AButton> GetButtonMap() const;

    /**
     * @brief Возвращает карту ползунков громкости и соответствующих ключей звуков.
     *
     * @return Неизменяемая карта, где ключи — указатели на ползунки, а значения — векторы строк с ключами звуков.
     */
    std::unordered_map<ASlider*, std::vector<std::string>> GetVolumeSliderMap();

private:
    sf::Texture SoundLabelsTexture;                                              // Текстура для меток звука
                                             
    sf::Texture MenuTexture;                                                     // Текстура для меню
    sf::Sprite MenuSprite;                                                       // Спрайт меню

    sf::RenderTexture PauseBlurTexture;                                          // Текстура для размытого фона при паузе
    sf::Sprite PauseBlurSprite;                                                  // Спрайт размытого фона

    AButton PlayButton;                                                          // Кнопка "Играть"
    AButton Options;                                                             // Кнопка "Настройки"
    AButton ExitButton;                                                          // Кнопка "Выход"
    AButton ContinueButton;                                                      // Кнопка "Продолжить"
    AButton Cross;                                                               // Кнопка "Закрыть"
    AButton MenuButton;                                                          // Кнопка "Меню"

    std::vector<ASlider*> SliderListVector;                                      // Вектор ползунков громкости

    std::unordered_map<std::string, AButton> ButtonMap;                          // Карта кнопок меню

    std::unordered_map<std::string, sf::Sprite> SoundLabelsMap;                  // Карта для хранения меток звука
    std::unordered_map<ASlider*, std::vector<std::string>> VolumeSliderMap;      // Карта ползунков и звуковых ключей
};
