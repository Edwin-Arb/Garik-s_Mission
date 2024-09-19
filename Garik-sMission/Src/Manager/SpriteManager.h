#pragma once
#include "../Constants.h"


/**
 * @brief Класс ASpriteManager предоставляет методы для управления размерами и ориентацией спрайтов и форм SFML.
 */
class ASpriteManager
{
public:
    /**
     * @brief Устанавливает размер спрайта.
     * 
     * @param Sprite Ссылка на спрайт, размер которого нужно изменить.
     * @param DesiredSize Желаемый размер спрайта в виде вектора (ширина, высота).
     */
    void SetSpriteSize(sf::Sprite& Sprite, const sf::Vector2f& DesiredSize);

    /**
     * @brief Устанавливает размер формы.
     * 
     * @param Shape Ссылка на форму, размер которой нужно изменить.
     * @param DesiredWidth Желаемая ширина формы.
     * @param DesiredHeight Желаемая высота формы.
     */
    void SetShapeSize(sf::Shape& Shape, float DesiredWidth, float DesiredHeight);

    /**
     * @brief Устанавливает относительную точку происхождения спрайта.
     * 
     * @param Sprite Ссылка на спрайт, для которого устанавливается относительная точка происхождения.
     * @param OriginX Относительная координата X точки происхождения спрайта (0.0 - 1.0).
     * @param OriginY Относительная координата Y точки происхождения спрайта (0.0 - 1.0).
     */
    void SetSpriteRelativeOrigin(sf::Sprite& Sprite, float OriginX, float OriginY);

    /**
     * @brief Устанавливает относительную точку происхождения формы.
     * 
     * @param Shape Ссылка на форму, для которой устанавливается относительная точка происхождения.
     * @param OriginX Относительная координата X точки происхождения формы (0.0 - 1.0).
     * @param OriginY Относительная координата Y точки происхождения формы (0.0 - 1.0).
     */
    void SetShapeRelativeOrigin(sf::Shape& Shape, float OriginX, float OriginY);
};
