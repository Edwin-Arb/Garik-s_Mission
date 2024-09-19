#include "FinalGateObject.h"


/**
* @brief Конструктор для создания финальных ворот с указанием начальной позиции.
* 
* @param StartPosition Начальная позиция финальных ворот.
*/
AFinalGateObject::AFinalGateObject(sf::Vector2f& StartPosition)
{
    ActorSize = {6.f, 64.f};

    ActorCollisionRect = {StartPosition.x, StartPosition.y, ActorSize.x, ActorSize.y};
}

/**
* @brief Инициализация финальных ворот.
* 
* Загружает необходимые текстуры и спрайты для отображения ворот.
* 
* @param SpriteManager Менеджер спрайтов для загрузки текстур.
*/
void AFinalGateObject::InitFinalGate(ASpriteManager& SpriteManager)
{
    const std::string GateTexturePath = ASSETS_PATH + "MainTiles/FinalGate.png";
    const sf::IntRect CloseGateRectTexture = {157, 480, static_cast<int>(ActorSize.x), static_cast<int>(ActorSize.y)};
    const sf::Vector2f GateOrigin = {0.5f, 0.5f};

    AActor::InitActorTexture(GateTexturePath, CloseGateRectTexture, ActorSize, GateOrigin, SpriteManager);

    // Установка позиции спрайта закрытых ворот
    ActorDrawPosition = {
        ActorCollisionRect.left + (ActorSize.x / 2.f),
        ActorCollisionRect.top + (ActorSize.y / 2.f)
    };

    ActorSprite.setPosition(ActorDrawPosition);
    ActorSprite.setScale(2.f, 1.45f);
}

/**
* @brief Обновление состояния финальных ворот.
* 
* Обрабатывает изменения состояния ворот (например, открытие/закрытие).
*/
void AFinalGateObject::UpdateStateFinalGate()
{    
    // Установка позиции спрайта открытых ворот
    ActorSize = {32.f, 48.f};
    sf::Vector2f OpenGatePosition = {5048.f, 521.f};
    
    ActorCollisionRect = {OpenGatePosition, ActorSize};
    sf::IntRect OpenGateRectTexture = {160, 288, static_cast<int>(ActorSize.x),
                                                 static_cast<int>(ActorSize.y)};

    ActorSprite.setTextureRect(OpenGateRectTexture);
    ActorSprite.setScale(2.f, 1.94f);

    ActorDrawPosition = {
        ActorCollisionRect.left + (ActorSize.x / 2.f),
        ActorCollisionRect.top + (ActorSize.y / 2.f)
    };

    ActorSprite.setPosition(ActorDrawPosition);
}

/**
* @brief Отрисовка финальных ворот.
* 
* @param Window Окно, в котором будут отображаться финальные ворота.
*/
void AFinalGateObject::DrawActor(sf::RenderWindow& Window)
{
    Window.draw(ActorSprite);
}
