#include "UIText.h"
#include "../GameObjects/ChestObject.h"


/**
* @brief Инициализация текста игры.
*/
void AUIText::InitGameText()
{
    // Инициализация шрифта для текста
    if (!MissionText0.loadFromFile(ASSETS_PATH + "MainTiles/Mission0.png"))
    {
        throw std::runtime_error("Error: Failed to load texture: " + ASSETS_PATH + "MainTiles/Mission0.png");
    }
    if (!MissionText1.loadFromFile(ASSETS_PATH + "MainTiles/Mission1.png"))
    {
        throw std::runtime_error("Error: Failed to load texture: " + ASSETS_PATH + "MainTiles/Mission1.png");
    }
    if (!MissionText2.loadFromFile(ASSETS_PATH + "MainTiles/Mission2.png"))
    {
        throw std::runtime_error("Error: Failed to load texture: " + ASSETS_PATH + "MainTiles/Mission2.png");
    }
    if (!FinalMissionText.loadFromFile(ASSETS_PATH + "MainTiles/FinalMission.png"))
    {
        throw std::runtime_error("Error: Failed to load texture: " + ASSETS_PATH + "MainTiles/FinalMission.png");
    }

    MissionSprite.setScale(0.1f, 0.1f);
}

/**
* @brief Установка позиции текста на экране.
*
* @param NewPosition Новая позиция текста.
*/
void AUIText::SetPositionGameText(const sf::Vector2f& NewPosition)
{
    MissionSprite.setPosition(NewPosition);
}

/**
* @brief Обновление текста миссии в зависимости от текущего состояния.
*/
void AUIText::UpdateMissionText()
{
    switch (AChestObject::ChestOpenCountStatic)
    {
    case 0:
        {
            MissionSprite.setTexture(MissionText0);
            break;
        }
    case 1:
        {
            MissionSprite.setTexture(MissionText1);
            break;
        }
    case 2:
        {
            MissionSprite.setTexture(MissionText2);
            break;
        }
    default:
        {
            MissionSprite.setTexture(FinalMissionText);
            break;
        }
    }
}

/**
* @brief Отрисовка текста на экране.
*
* @param Window Окно, в котором происходит отрисовка.
*/
void AUIText::DrawGameText(sf::RenderWindow& Window) const
{
    Window.draw(MissionSprite);
}
