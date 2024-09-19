#include "KeyObject.h"

int AKeyObject::KeysCount = 0;
int AKeyObject::KeysCountFound = 0;

/**
 * @brief Конструктор объекта ключа.
 * 
 * Инициализирует объект ключа с начальной позицией и базовыми параметрами.
 * 
 * @param StartPosition Начальная позиция ключа.
 */
AKeyObject::AKeyObject(const sf::Vector2f& StartPosition)
    : bIsKeyFound(false),
      JumpKey(true),
      Gravity(true),
      KeyID(EKeyType::EKT_Non)
{
    ++KeysCount; // Увеличиваем счетчик ключей
    ActorVelocity = {0.f, 0.f};
    ActorSize = {14, 9};
    ActorCollisionRect = {StartPosition.x, StartPosition.y, ActorSize.x, ActorSize.y};
}

/**
 * @brief Инициализация ключей.
 * 
 * Инициализирует текстуры и спрайты для каждого типа ключа.
 * 
 * @param SpriteManager Менеджер спрайтов для загрузки текстур.
 */
void AKeyObject::InitKeys(ASpriteManager& SpriteManager)
{
    // Инициализация 3 видов ключей
    const std::string KeysTexturePath = ASSETS_PATH + "MainTiles/AllObjects.png";
    const sf::Vector2f KeyOrigin = {0.5f, 0.5f};

    switch (KeysCount)
    {
    case 1:
        {
            // Красный ключ
            const sf::IntRect RedKeyRectTexture = sf::IntRect(1, 20, static_cast<int>(ActorSize.x),
                                                              static_cast<int>(ActorSize.y));
            AActor::InitActorTexture(KeysTexturePath, RedKeyRectTexture, ActorSize, KeyOrigin, SpriteManager);

            KeyID = EKeyType::EKT_Red;
            break;
        }
    case 2:
        {
            // Зеленый ключ
            const sf::IntRect GreenKeyRectTexture = sf::IntRect(33, 20, static_cast<int>(ActorSize.x),
                                                                static_cast<int>(ActorSize.y));
            AActor::InitActorTexture(KeysTexturePath, GreenKeyRectTexture, ActorSize, KeyOrigin, SpriteManager);

            KeyID = EKeyType::EKT_Green;
            break;
        }
    case 3:
        {
            // Желтый ключ
            const sf::IntRect YellowKeyRectTexture = sf::IntRect(17, 20, static_cast<int>(ActorSize.x),
                                                                 static_cast<int>(ActorSize.y));
            AActor::InitActorTexture(KeysTexturePath, YellowKeyRectTexture, ActorSize, KeyOrigin, SpriteManager);

            KeyID = EKeyType::EKT_Yellow;
            break;
        }
    default:
        {
            break;
        }
    }

    ActorSprite.setScale(0.6f, 0.6f);
}

/**
 * @brief Обновление движения ключа.
 * 
 * Обновляет положение ключа в зависимости от времени и гравитации.
 * 
 * @param DeltaTime Время, прошедшее с предыдущего кадра.
 */
void AKeyObject::UpdateMoveKey(float DeltaTime)
{
    if (Gravity)
    {
        // Реализация появления(лёгкого прыжка вверх), после убийсва босса
        if (JumpKey)
        {
            float KeyVelocityJump = 120.f;

            ActorVelocity.y = KeyVelocityJump * DeltaTime;
            JumpKey = false;
        }

        // Применение гравитации для падения персонажа
        ActorVelocity.y += -2.f * DeltaTime;
    }

    // Обновление позиции персонажа на основе скорости
    ActorCollisionRect.left += ActorVelocity.x;
    ActorCollisionRect.top -= ActorVelocity.y;

    // Обновление позиции спрайта персонажа и его коллизии
    if (!bIsKeyFound)
    {
        ActorDrawPosition = {
            ActorCollisionRect.left + (ActorSize.x / 2.f),
            ActorCollisionRect.top + (ActorSize.y / 2.f)
        };
    }

    ActorSprite.setPosition(ActorDrawPosition);
}

/**
 * @brief Установка флага, найден ли ключ.
 * 
 * @param WasKeyFound Логическое значение, указывающее, был ли ключ найден.
 */
void AKeyObject::SetKeyFound(bool WasKeyFound)
{
    bIsKeyFound = WasKeyFound;
    if (WasKeyFound)
    {
        ++KeysCountFound; // Увеличиваем количество найденных ключей
    }
}

/**
 * @brief Установка гравитации для ключа.
 * 
 * @param NewGravity Новое значение гравитации для ключа.
 */
void AKeyObject::SetKeyGravity(bool NewGravity)
{
    Gravity = NewGravity;
}

/**
 * @brief Установка новой позиции ключа.
 * 
 * @param NewPosition Новая позиция ключа.
 */
void AKeyObject::SetActorPosition(const sf::Vector2f& NewPosition)
{
    ActorDrawPosition = NewPosition;
    ActorSprite.setPosition(ActorDrawPosition);
}

/**
 * @brief Поместить ключ в верхний правый угол экрана.
 * 
 * @param Window Вид окна, по которому рассчитывается положение ключа.
 */
void AKeyObject::MakrKeyInTopRightCorner(const sf::View& Window)
{
    float KeyPositionX; // Фиксированное смещение по оси X от правого края
    constexpr float KeySpacingY = 30.f; // Расстояние между ключами по оси Y

    // Определяем вертикальное смещение в зависимости от типа ключа
    constexpr float KeyOffsetX = 10.f;

    switch (GetKeyID())
    {
    case EKeyType::EKT_Red:
        {
            KeyPositionX = KeyOffsetX * 3; // Позиция первого ключа (Красного)
            break;
        }
    case EKeyType::EKT_Green:
        {
            KeyPositionX = KeyOffsetX * 2; // Позиция второго ключа (Зелёного)
            break;
        }
    case EKeyType::EKT_Yellow:
        {
            KeyPositionX = KeyOffsetX * 1; // Позиция третьего ключа (Жёлтого)
            break;
        }
    default:
        {
            return; // Если тип ключа не определен, ничего не делать
        }
    }

    // Рассчитываем новую позицию ключа
    sf::Vector2f NewPosition(
        Window.getCenter().x + (Window.getSize().x / 2.f) - KeyPositionX, // Смещение от правого края
        Window.getCenter().y - (Window.getSize().y / 2.f) + KeySpacingY // Смещение от верхнего края
    );

    // Устанавливаем новую позицию ключа
    AKeyObject::SetActorPosition(NewPosition);
}

/**
 * @brief Сброс состояния ключа.
 * 
 * Сбрасывает все параметры ключа к начальному состоянию.
 */
void AKeyObject::ResetKey()
{
    // Устанавливаем дефолтные значения ключа
    KeysCount = 0;
    KeysCountFound = 0;

    KeyID = EKeyType::EKT_Non;
}

/**
 * @brief Отрисовка ключа на экране.
 * 
 * @param Window Окно, в котором будет отображаться ключ.
 */
void AKeyObject::DrawActor(sf::RenderWindow& Window)
{
    Window.draw(ActorSprite);
}

/**
 * @brief Проверка, был ли ключ найден.
 * 
 * @return Логическое значение, указывающее, найден ли ключ.
 */
bool AKeyObject::GetKeyFound() const
{
    return bIsKeyFound;
}

/**
 * @brief Получение количества ключей.
 * 
 * @return Текущее количество ключей.
 */
int AKeyObject::GetKeyCount() const
{
    return KeysCount;
}

/**
 * @brief Получение ID ключа.
 * 
 * @return Идентификатор типа ключа.
 */
EKeyType AKeyObject::GetKeyID() const
{
    return KeyID;
}
