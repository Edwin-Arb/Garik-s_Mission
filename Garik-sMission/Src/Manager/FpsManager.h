#pragma once
#include "../Constants.h"


// TODO: Класс используется для тестирования производительно, при релизе игры - отключить
/**
 * @brief Класс Менеджер для отображения FPS (количество кадров в секунду) в игре.
 */
class AFpsManager
{
public:
    /**
     * @brief Конструктор по умолчанию.
     */
    AFpsManager() = default;

    /**
     * @brief Деструктор по умолчанию.
     */
    ~AFpsManager() = default;

    /**
     * @brief Инициализирует текст FPS.
     */
    void InitFpsText();

    /**
     * @brief Обновляет текст FPS на основе текущего времени между кадрами.
     * 
     * @param DeltaTime Время между текущим и предыдущим кадром.
     */
    void UpdateFpsText(float DeltaTime);

    /**
     * @brief Устанавливает позицию текста FPS.
     * 
     * @param NewPosition Новая позиция текста.
     */
    void SetPositionFpsText(const sf::Vector2f& NewPosition);

    /**
     * @brief Отрисовывает текст FPS на указанном окне.
     * 
     * @param Window Окно для отрисовки текста.
     */
    void DrawFpsText(sf::RenderWindow& Window) const;

private:
    // Делаем отображение количества кадров в секунду(FPS)
    sf::Text FPSText;    // Текст, отображающий количество FPS.
    sf::Font FPSFont;    // Шрифт для текста FPS.
};