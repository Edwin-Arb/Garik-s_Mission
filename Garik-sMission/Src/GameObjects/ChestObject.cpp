#include "ChestObject.h"

int AChestObject::ChestCount = 1;
int AChestObject::ChestOpenCountStatic = 0;

/**
* @brief Конструктор для сундука с указанием начальной позиции.
* 
* @param StartPositionChest Начальная позиция сундука.
*/
AChestObject::AChestObject(const sf::Vector2f& StartPositionChest)
    : bIsChestOpen(false),
      ChestID(EChestType::ECT_Non),
      CurrentChestAnimation(nullptr)
{
    ActorSize = {16.f, 13.f};

    ActorCollisionRect = {StartPositionChest.x, StartPositionChest.y, ActorSize.x, ActorSize.y};
}

/**
* @brief Инициализация сундука.
* 
* Загружает анимации и текстуры сундука.
* 
* @param SpriteManager Менеджер спрайтов для загрузки текстур.
*/
void AChestObject::InitChest(ASpriteManager& SpriteManager)
{
    const std::string ChestTexturePath = ASSETS_PATH + "MainTiles/Chests.png";
    const sf::Vector2f ChestOrigin = {0.5f, 0.5f};

    switch (ChestCount)
    {
    case 1:
        {
            // Красный сундук
            const sf::IntRect RedChestRectTexture = sf::IntRect(0, 1, static_cast<int>(ActorSize.x),
                                                                static_cast<int>(ActorSize.y));
            AActor::InitActorTexture(ChestTexturePath, RedChestRectTexture, ActorSize, ChestOrigin, SpriteManager);

            // Анимация открытия сундука
            RedChestOpenAnimation.AnimTexture.loadFromFile(ChestTexturePath);
            RedChestOpenAnimation.FrameSpeed = 5.f;
            RedChestOpenAnimation.FrameRect.emplace_back(sf::IntRect(RedChestRectTexture));
            RedChestOpenAnimation.FrameRect.emplace_back(
                sf::IntRect(16, 1, static_cast<int>(ActorSize.x), static_cast<int>(ActorSize.y)));
            RedChestOpenAnimation.FrameRect.emplace_back(
                sf::IntRect(48, 1, static_cast<int>(ActorSize.x), static_cast<int>(ActorSize.y)));
            RedChestOpenAnimation.FrameRect.emplace_back(
                sf::IntRect(64, 1, static_cast<int>(ActorSize.x), static_cast<int>(ActorSize.y)));

            RedChestOpenAnimation.SetStopAtLastFrame(true); // Остановить на последнем кадре

            CurrentChestAnimation = &RedChestOpenAnimation; // Назначаем текущую анимацию

            ChestID = EChestType::ECT_Red;

            break;
        }
    case 2:
        {
            // Зеленый сундук
            const sf::IntRect GreenChestRectTexture = sf::IntRect(0, 16, 16, 13);
            AActor::InitActorTexture(ChestTexturePath, GreenChestRectTexture, ActorSize, ChestOrigin, SpriteManager);

            // Анимация открытия сундука
            GreenChestOpenAnimation.AnimTexture.loadFromFile(ChestTexturePath);
            GreenChestOpenAnimation.FrameSpeed = 5.f;
            GreenChestOpenAnimation.FrameRect.emplace_back(sf::IntRect(GreenChestRectTexture));
            GreenChestOpenAnimation.FrameRect.emplace_back(
                sf::IntRect(16, 16, static_cast<int>(ActorSize.x), static_cast<int>(ActorSize.y)));
            GreenChestOpenAnimation.FrameRect.emplace_back(
                sf::IntRect(48, 16, static_cast<int>(ActorSize.x), static_cast<int>(ActorSize.y)));
            GreenChestOpenAnimation.FrameRect.emplace_back(
                sf::IntRect(64, 16, static_cast<int>(ActorSize.x), static_cast<int>(ActorSize.y)));

            GreenChestOpenAnimation.SetStopAtLastFrame(true); // Остановить на последнем кадре

            CurrentChestAnimation = &GreenChestOpenAnimation; // Назначаем текущую анимацию

            ChestID = EChestType::ECT_Green;

            break;
        }
    case 3:
        {
            // Желтый сундук
            const sf::IntRect YellowChestRectTexture = sf::IntRect(0, 31, 16, 13);
            AActor::InitActorTexture(ChestTexturePath, YellowChestRectTexture, ActorSize, ChestOrigin, SpriteManager);

            // Анимация открытия сундука
            YellowChestOpenAnimation.AnimTexture.loadFromFile(ChestTexturePath);
            YellowChestOpenAnimation.FrameSpeed = 3.f;
            YellowChestOpenAnimation.FrameRect.emplace_back(sf::IntRect(YellowChestRectTexture));
            YellowChestOpenAnimation.FrameRect.emplace_back(
                sf::IntRect(16, 31, static_cast<int>(ActorSize.x), static_cast<int>(ActorSize.y)));
            YellowChestOpenAnimation.FrameRect.emplace_back(
                sf::IntRect(48, 31, static_cast<int>(ActorSize.x), static_cast<int>(ActorSize.y)));
            YellowChestOpenAnimation.FrameRect.emplace_back(
                sf::IntRect(64, 31, static_cast<int>(ActorSize.x), static_cast<int>(ActorSize.y)));

            YellowChestOpenAnimation.SetStopAtLastFrame(true); // Остановить на последнем кадре

            CurrentChestAnimation = &YellowChestOpenAnimation; // Назначаем текущую анимацию

            ChestID = EChestType::ECT_Yellow;

            break;
        }
    default:
        {
            // Если не совпадает ни один сундук
            break;
        }
    }

    ActorSprite.setScale(1.f, 1.f);
}

/**
* @brief Обновляет анимацию сундука.
* 
* @param DeltaTime Время, прошедшее с последнего кадра.
*/
void AChestObject::ChestUpdateAnimation(float DeltaTime)
{
    if (bIsChestOpen && CurrentChestAnimation)
    {
        if (!CurrentChestAnimation->IsAnimationFinished())
        {
            ActorSprite.setTextureRect(CurrentChestAnimation->GetCurrentFrame());
            CurrentChestAnimation->AnimationUpdate(DeltaTime);
        }
    }

    // Обновление позиции спрайта персонажа и его коллизии
    ActorDrawPosition = {
        ActorCollisionRect.left + static_cast<int>(ActorSize.x) / 2.f,
        ActorCollisionRect.top + static_cast<int>(ActorSize.y) / 2.f
    };

    ActorSprite.setPosition(ActorDrawPosition);
}

/**
* @brief Открывает сундук.
*/
void AChestObject::OpenChest()
{
    bIsChestOpen = true;
}

/**
* @brief Увеличивает счетчик открытых сундуков.
*/
void AChestObject::IncreaseOpenChestCounter()
{
    ++ChestOpenCountStatic;
}

/**
* @brief Сбрасывает состояние сундука.
*/
void AChestObject::ResetChestObject()
{
    ChestCount = 1;
    ChestOpenCountStatic = 0;

    bIsChestOpen = false;
    ChestID = EChestType::ECT_Non;
    CurrentChestAnimation = nullptr;

    RedChestOpenAnimation.ResetAnimation();
    GreenChestOpenAnimation.ResetAnimation();
    YellowChestOpenAnimation.ResetAnimation();
}

/**
* @brief Отрисовывает сундук.
* 
* @param Window Окно, в котором будет отображаться сундук.
*/
void AChestObject::DrawActor(sf::RenderWindow& Window)
{
    Window.draw(ActorSprite);
}

/**
* @brief Проверка, открыт ли сундук.
* 
* @return true, если сундук открыт, иначе false.
*/
bool AChestObject::GetOpenChest() const
{
    return bIsChestOpen;
}

/**
* @brief Проверяет, можно ли открыть сундук с помощью данного ключа.
* 
* @param KeyID Идентификатор ключа, которым пытаются открыть сундук.
* 
* @return true, если ключ подходит для сундука, иначе false.
*/
bool AChestObject::CanOpenWithKey(EKeyType KeyID) const
{
    return ChestID == static_cast<EChestType>(KeyID);
}