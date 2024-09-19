#include "AActor.h"

/**
 * @brief Инициализирует текстуру объекта.
 * 
 * @param TexturePath Путь к текстуре.
 * @param TextureRect Прямоугольник, ограничивающий текстуру.
 * @param Size Размер объекта.
 * @param Origin Установить центр объекта.
 * @param SpriteManager Менеджер спрайтов для обработки спрайтов объекта.
 */
void AActor::InitActorTexture(const std::string& TexturePath, const sf::IntRect& TextureRect,
                              const sf::Vector2f& Size, const sf::Vector2f& Origin,
                              ASpriteManager& SpriteManager)
{
    ActorTexturePtr = std::make_unique<sf::Texture>();

    // Подгрузить текстуру из файла
    if (!ActorTexturePtr->loadFromFile(TexturePath))
    {
        throw std::runtime_error("Error: Failed to load texture: " + ASSETS_PATH + TexturePath);
    }

    InitActorSprite(ActorTexturePtr, TextureRect, Size, Origin, SpriteManager);
}

/**
 * @brief Инициализирует текстуру объекта.
 * 
 * @param TextureRect Прямоугольник, ограничивающий текстуру.
 * @param Size Размер объекта.
 * @param Origin Установить центр объекта.
 * @param SpriteManager Менеджер спрайтов для обработки спрайтов объекта.
 */
void AActor::InitActorTexture(const sf::IntRect& TextureRect,
                              const sf::Vector2f& Size, const sf::Vector2f& Origin,
                              ASpriteManager& SpriteManager)
{
    ActorTexturePtr = std::make_unique<sf::Texture>();

    InitActorSprite(ActorTexturePtr, TextureRect, Size, Origin, SpriteManager);
}

/**
 * @brief Инициализирует спрайт объекта.
 * 
 * @param TexturePtr Указатель на текстуру.
 * @param TextureRect Прямоугольник, ограничивающий текстуру.
 * @param DesiredSize Размер объекта.
 * @param Origin Установить центр объекта.
 * @param SpriteManager Менеджер спрайтов для обработки спрайтов объекта.
 */
void AActor::InitActorSprite(const std::unique_ptr<sf::Texture>& TexturePtr, const sf::IntRect& TextureRect,
                             const sf::Vector2f& DesiredSize, const sf::Vector2f& Origin, ASpriteManager& SpriteManager)
{
    // Создать спрайт объекта и установить его текстуру и прямоугольник
    ActorSprite.setTexture(*TexturePtr);
    ActorSprite.setTextureRect(TextureRect);

    // Установить масштаб объекта
    SpriteManager.SetSpriteSize(ActorSprite, DesiredSize);

    // Установить центр спрайта и коллизии
    SpriteManager.SetSpriteRelativeOrigin(ActorSprite, Origin.x, Origin.y);
}

/**
 * @brief Устанавливает новую скорость объекта.
 * 
 * @param NewVelocity Новая скорость объекта.
 */
void AActor::SetActorVelocity(const sf::Vector2f& NewVelocity)
{
    ActorVelocity = NewVelocity;
}

/**
 * @brief Устанавливает текущую позицию объекта.
 *
 * @param NewPosition Новая позиция объекта.
 */
void AActor::SetActorPosition(const sf::Vector2f& NewPosition)
{
    ActorCollisionRect.left = NewPosition.x;
    ActorCollisionRect.top = NewPosition.y;
    
    ActorDrawPosition = {
        ActorCollisionRect.left + (ActorSize.x / 2.f),
        ActorCollisionRect.top + (ActorSize.y / 2.f)
    };

    ActorSprite.setPosition(ActorDrawPosition);
}

/**
 * @brief Устанавливает прямоугольник объекта.
 *
 * @param NewRect Новый прямоугольник, ограничивающий объект.
 */
void AActor::SetActorCollisionRect(const sf::FloatRect& NewRect)
{
    ActorCollisionRect = NewRect;
}

/**
 * @brief Устанавливает спрайт объекта.
 * 
 * @param NewActorSprite Новый спрайт объекта.
 */
void AActor::SetActorSprite(const sf::Sprite& NewActorSprite)
{
    ActorSprite = NewActorSprite;
}

/**
 * @brief Получает по ссылке текущую скорость объекта.
 *
 * @return Текущая скорость объекта.
 */
sf::Vector2f& AActor::GetActorVelocity()
{
    return ActorVelocity;
}

/**
 * @brief Получает по ссылке текущую позицию объекта.
 *
 * @return Текущая позиция объекта.
 */
sf::Vector2f& AActor::GetActorPosition()
{
    return ActorDrawPosition;
}

/**
 * @brief Получает по ссылке прямоугольник, ограничивающий объект.
 *
 * @return Прямоугольник, ограничивающий объект.
 */
sf::FloatRect& AActor::GetActorCollisionRect()
{
    return ActorCollisionRect;
}

/**
 * @brief Получает текущий спрайт объекта.
 *
 * @return Текущий спрайт объекта.
 */
sf::Sprite AActor::GetActorSprite() const
{
    return ActorSprite;
}
