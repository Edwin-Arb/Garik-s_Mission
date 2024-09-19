#pragma once
#include "../Constants.h"

/**
 * @brief Класс для работы с текстом в игре.
 */
class AUIText
{
public:
    /**
     * @brief Конструктор класса AUIText.
     */
     AUIText() = default;
 
    /**
     * @brief Деструктор класса AUIText.
     */
    ~AUIText() = default;

    /**
     * @brief Инициализация текста игры.
     */
    void InitGameText();

    /**
     * @brief Установка позиции текста на экране.
     *
     * @param NewPosition Новая позиция текста.
     */
    void SetPositionGameText(const sf::Vector2f& NewPosition);

    /**
     * @brief Обновление текста миссии в зависимости от текущего состояния.
     */
    void UpdateMissionText();
    
    /**
     * @brief Отрисовка текста на экране.
     *
     * @param Window Окно, в котором происходит отрисовка.
     */
    void DrawGameText(sf::RenderWindow& Window) const;

private:
    sf::Texture MissionText0;         // Текстура для текста миссии 0
    sf::Texture MissionText1;         // Текстура для текста миссии 1
    sf::Texture MissionText2;         // Текстура для текста миссии 2
    sf::Texture FinalMissionText;     // Текстура для финального текста миссии
    sf::Sprite MissionSprite;         // Спрайт для отображения текста миссии
};
