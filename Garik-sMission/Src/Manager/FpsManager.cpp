#include "FpsManager.h"

/**
 * @brief Инициализация текста для отображения FPS.
 *
 * Метод загружает шрифт, устанавливает параметры текста и его начальную позицию.
 * В случае ошибки загрузки шрифта, программа завершает выполнение.
 */
void AFpsManager::InitFpsText()
{
    // Убеждаемся, что шрифт успешно загружен
    if (!FPSFont.loadFromFile(ASSETS_PATH + "Fonts/Roboto-Bold.ttf"))
    {
        throw std::runtime_error("Error: Failed to load texture: " + ASSETS_PATH + "Fonts/Roboto-Bold.ttf");
    }
    
    // Устанавливаем шрифт для текста FPS
    FPSText.setFont(FPSFont);

    // Устанавливаем размер символов текста FPS
    FPSText.setCharacterSize(7);

    // Устанавливаем начальную позицию текста FPS
    FPSText.setPosition(10.f, 10.f);

    // Устанавливаем цвет текста FPS
    FPSText.setFillColor(sf::Color::Yellow);
}

/**
 * @brief Обновление текста, отображающего количество FPS.
 *
 * @param DeltaTime Время, прошедшее с последнего кадра.
 */
void AFpsManager::UpdateFpsText(float DeltaTime)
{
    // Обновляем текст, отображающий количество FPS
    FPSText.setString("FPS: " + std::to_string(static_cast<int>(1 / DeltaTime)));
}

/**
 * @brief Установка новой позиции текста FPS.
 *
 * @param NewPosition Новая позиция текста FPS.
 */
void AFpsManager::SetPositionFpsText(const sf::Vector2f& NewPosition)
{
    FPSText.setPosition(NewPosition);
}

/**
 * @brief Отрисовка текста FPS на окне.
 *
 * @param Window Ссылка на окно SFML для отрисовки.
 */
void AFpsManager::DrawFpsText(sf::RenderWindow& Window) const
{
    // Отрисовываем текст FPS на указанном окне
    Window.draw(FPSText);
}
