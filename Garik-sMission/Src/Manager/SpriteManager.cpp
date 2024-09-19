// ReSharper disable CppMemberFunctionMayBeStatic
#include "SpriteManager.h"

/**
 * @brief Устанавливает размер спрайта с заданными шириной и высотой.
 *
 * @param Sprite Ссылка на объект спрайта SFML.
 * @param DesiredSize Желаемый размер спрайта в виде вектора (ширина, высота).
 */
void ASpriteManager::SetSpriteSize(sf::Sprite& Sprite, const sf::Vector2f& DesiredSize)
{
    // Получаем локальные границы спрайта
    const sf::FloatRect SpriteRect = Sprite.getLocalBounds();

    // Вычисляем масштаб по осям X и Y для достижения желаемых размеров
    const sf::Vector2f Scale = {DesiredSize.x / SpriteRect.width, DesiredSize.y / SpriteRect.height};

    // Устанавливаем масштаб спрайта
    Sprite.setScale(Scale);
}

/**
 * @brief Устанавливает размер формы с заданными шириной и высотой.
 *
 * @param Shape Ссылка на объект формы SFML.
 * @param DesiredWidth Желаемая ширина формы.
 * @param DesiredHeight Желаемая высота формы.
 */
void ASpriteManager::SetShapeSize(sf::Shape& Shape, const float DesiredWidth, const float DesiredHeight)
{
    // Получаем локальные границы формы
    const sf::FloatRect ShapeRect = Shape.getLocalBounds();

    // Вычисляем масштаб по осям X и Y для достижения желаемых размеров
    const sf::Vector2f Scale = {DesiredWidth / ShapeRect.width, DesiredHeight / ShapeRect.height};

    // Устанавливаем масштаб формы
    Shape.setScale(Scale);
}

/**
 * @brief Устанавливает относительный центр происхождения спрайта.
 *
 * @param Sprite Ссылка на объект спрайта SFML.
 * @param OriginX Относительная координата X центра происхождения спрайта (0.0 - 1.0).
 * @param OriginY Относительная координата Y центра происхождения спрайта (0.0 - 1.0).
 */
void ASpriteManager::SetSpriteRelativeOrigin(sf::Sprite& Sprite, const float OriginX, const float OriginY)
{
    // Получаем локальные границы спрайта
    const sf::FloatRect SpriteRect = Sprite.getLocalBounds();

    // Вычисляем относительные координаты центра происхождения спрайта
    const sf::Vector2f Origin = {OriginX * SpriteRect.width, OriginY * SpriteRect.height};

    // Устанавливаем центр происхождения спрайта
    Sprite.setOrigin(Origin);
}

/**
 * @brief Устанавливает относительный центр происхождения формы.
 *
 * @param Shape Ссылка на объект формы SFML.
 * @param OriginX Относительная координата X центра происхождения формы (0.0 - 1.0).
 * @param OriginY Относительная координата Y центра происхождения формы (0.0 - 1.0).
 */
void ASpriteManager::SetShapeRelativeOrigin(sf::Shape& Shape, const float OriginX, const float OriginY)
{
    // Получаем локальные границы формы
    const sf::FloatRect ShapeRect = Shape.getLocalBounds();

    // Вычисляем относительные координаты центра происхождения формы
    const sf::Vector2f Origin = {OriginX * ShapeRect.width, OriginY * ShapeRect.height};

    // Устанавливаем центр происхождения формы
    Shape.setOrigin(Origin);
}
