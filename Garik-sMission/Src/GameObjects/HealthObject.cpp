#include "HealthObject.h"


/**
* @brief Конструктор для создания объекта лечения с указанием начальной позиции.
* 
* @param StartPosition Начальная позиция объекта лечения.
*/
AHealthObject::AHealthObject(const sf::Vector2f& StartPosition)
    : bIsEatenHeal(false),
      StartPosition(StartPosition) // Сохраняем начальную позицию
{
    ActorSize = {14.f, 10.f};
    ActorCollisionRect = {StartPosition, ActorSize};
}

/**
* @brief Инициализация объекта лечения.
* 
* Загружает необходимые спрайты и текстуры для объекта лечения.
* 
* @param SpriteManager Менеджер спрайтов для загрузки текстур.
*/
void AHealthObject::InitHealthObject(ASpriteManager& SpriteManager)
{
    const std::string HealthTexturePath = ASSETS_PATH + "MainTiles/AllObjects.png";
    const sf::IntRect HamRect = {49, 22, static_cast<int>(ActorSize.x), static_cast<int>(ActorSize.y)};

    AActor::InitActorTexture(HealthTexturePath, HamRect, {1.f, 1.f}, {0.5f, 0.5f}, SpriteManager);

    ActorSprite.setScale(1.5f, 1.5f);
    
    // Устанавливаем позицию при инициализации
    SetActorPosition(StartPosition);
}


/**
* @brief Установка состояния, был ли объект лечения использован.
* 
* @param EatenHeal Логическое значение, указывающее, был ли объект лечения использован.
*/
void AHealthObject::SetIsEatenHeal(bool EatenHeal)
{
    bIsEatenHeal = EatenHeal;

    // Сбрасываем коллизию, если объект съеден
    if (bIsEatenHeal)
    {
        ActorCollisionRect.left = 0;
        ActorCollisionRect.top = 0;
    }

    ActorDrawPosition = {
        ActorCollisionRect.left + (ActorSize.x / 2.f),
        ActorCollisionRect.top + (ActorSize.y / 2.f)
    };

    ActorSprite.setPosition(ActorDrawPosition);
}

/**
* @brief Сброс состояния объекта лечения.
* 
* Возвращает объект лечения в начальное состояние.
*/
void AHealthObject::ResetHealObjectState()
{
    bIsEatenHeal = false;

    // Возвращаем объект на начальную позицию
    SetActorPosition(StartPosition);
}

/**
* @brief Отрисовка объекта лечения.
* 
* @param Window Окно, в котором будет отображаться объект лечения.
*/
void AHealthObject::DrawActor(sf::RenderWindow& Window)
{
    if (!bIsEatenHeal)
    {
        Window.draw(ActorSprite);
    }
}
