#pragma once
#include "../Abstract/AActor.h"

class AGameState;

class ACreditsScreen : public AActor
{
public:
    /**
     * @brief Конструктор класса ACreditsScreen.
     * 
     * Инициализирует переменные для экрана с титрами.
     */
    ACreditsScreen();

    /**
     * @brief Деструктор класса ACreditsScreen.
     * 
     * Используется деструктор по умолчанию.
     */
    ~ACreditsScreen() override = default;

    /**
     * @brief Инициализирует экран с титрами.
     * 
     * Загружает спрайты и текстуры для отображения титров.
     * 
     * @param SpriteManager Менеджер спрайтов, используется для загрузки текстур и спрайтов.
     */
    void InitCreditsScreen(ASpriteManager& SpriteManager);

    /**
     * @brief Устанавливает новую позицию для экрана с титрами.
     * 
     * Перемещает экран с титрами в указанное место.
     * 
     * @param NewPosition Новая позиция экрана с титрами.
     */
    void SetCreditsScreenPosition(const sf::Vector2f& NewPosition);

    /**
     * @brief Обновляет состояние экрана с титрами.
     * 
     * Плавно изменяет фон и отслеживает время показа титров.
     * 
     * @param ClockTimer Время, прошедшее с начала отображения титров.
     */
    void UpdateCreditsScreen(const sf::Clock& ClockTimer);

    /**
     * @brief Сбрасывает экран с титрами.
     * 
     * Возвращает экран с титрами в начальное состояние для повторного использования.
     */
    void ResetCreditsScreen();

    /**
     * @brief Отрисовывает объекты на экране.
     * 
     * Рисует спрайты титров и фона на экране.
     * 
     * @param Window Окно, в которое происходит отрисовка объектов.
     */
    void DrawActor(sf::RenderWindow& Window) override;

private:
    bool bIsDrawMouse;                            // Флаг отображения курсора мыши
    bool bIsBackgroundCompletely;                 // Флаг завершенности появления фона
    const float MaxInterpolation;                 // Максимальное значение интерполяции фона
    const float AddInterpolationValue;            // Шаг изменения интерполяции фона
    float InterpolationBackground;                // Текущее значение интерполяции фона
    
    sf::RenderTexture BlackBackgroundTexture;     // Текстура для черного фона
    sf::Sprite BlackBackgroundSprite;             // Спрайт для черного фона
    
    sf::Texture MouseTexture;                     // Текстура для курсора мыши
    sf::Sprite MouseSprite;                       // Спрайт для курсора мыши
};