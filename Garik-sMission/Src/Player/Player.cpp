#include "Player.h"

/**
 * @brief Конструктор по умолчанию для класса APlayer.
 * Инициализирует начальные значения членов класса.
 */
APlayer::APlayer()
    : bIsUseController(true),
      bIsMoveRight(true),               // Персонаж смотрит вправо при инициализации
      bIsOnLadder(false),               // Персонаж не на лестнице при инициализации
      bIsDeath(false),                  // Персонаж живой при старте игры
      bIsDeathSoundPlayed(false),
      bIsBeingPushedBack(false),
      bisBulletFromRight(false),
      PlayerJumpSpeed(200.f),           // Установка скорости прыжка персонажа
      MaxPushBackTime(0.2f),
      CurrentAnimation(nullptr)         // Инициализация указателя на текущую анимацию
{
    // Персонаж может прыгать при инициализации
    bIsPawnJump = true;

    // Максимальное и текущие здоровья персонажа, чтобы использовать для отслеживания шкалы здоровья.
    PawnMaxHealth = PLAYER_MAX_HEALTH;
    PawnCurrentHealth = PawnMaxHealth;

    // Скорость персонажа
    PawnSpeed = PLAYER_SPEED;

    // Скорость и направление персонажа
    ActorVelocity = {0.f, 0.f};

    // Прямоугольник персонажа с учетом масштабирования
    ActorCollisionRect = {
        0.f,0.f,
        PLAYER_SIZE.x,
        PLAYER_SIZE.y
    };
}

/**
 * @brief Инициализация персонажа.
 * Загружает текстуры и настраивает спрайты и шкалу здоровья.
 * 
 * @param SpriteManager Менеджер спрайтов для управления отображением.
 */
void APlayer::InitPlayer(ASpriteManager& SpriteManager)
{
    // Стартовая позиция персонажа
    ActorCollisionRect = {
        110.f, 520.f,
        PLAYER_SIZE.x,
        PLAYER_SIZE.y
    };
    
    // Инициализация переменных для текстуры персонажа
    const std::string PlayerTexturePath = ASSETS_PATH + "MainTiles/Player.png";
    const sf::IntRect PlayerRectTexture = sf::IntRect(19, 0, static_cast<int>(PLAYER_SIZE.x),
                                                             static_cast<int>(PLAYER_SIZE.y));
    const sf::Vector2f PlayerSize = {PLAYER_SIZE.x * DRAW_SCALE.x, PLAYER_SIZE.y * DRAW_SCALE.y};
    const sf::Vector2f PlayerOrigin = {0.5f, 0.5f};

    // Инициализировать текстуру для персонажа и создать спрайт для него
    AActor::InitActorTexture(PlayerTexturePath, PlayerRectTexture,
                             PlayerSize, PlayerOrigin,
                             SpriteManager);

    // Инициализация переменных текстуры шкалы здоровья для персонажа
    const std::string PlayerTextureHealthBarPath = ASSETS_PATH + "MainTiles/HealthBarPlayer.png";
    const sf::Vector2f HealthBarSize = sf::Vector2f(40.f, 5.f);
    const sf::Vector2f HealthBarScale = sf::Vector2f(0.6f, 0.6f);
    const sf::Color HealthBarFrontColor = sf::Color(135, 38, 38); // Тёмно красный цвет
    const sf::Color HealthBarBackgroundColor = sf::Color::Black;

    // Инициализация шкалы здоровья
    InitPawnHealthBar(PlayerTextureHealthBarPath, HealthBarSize, HealthBarScale,
                      HealthBarFrontColor, HealthBarBackgroundColor,
                      SpriteManager);
    
    // Анимация ожидания(Idle)
    IdleAnimation.AnimTexture.loadFromFile(PlayerTexturePath);
    IdleAnimation.FrameSpeed = 3.f;

    IdleAnimation.FrameRect.emplace_back(sf::IntRect(3, 0, static_cast<int>(PLAYER_SIZE.x),
                                                     static_cast<int>(PLAYER_SIZE.y)));
    IdleAnimation.FrameRect.emplace_back(sf::IntRect(PlayerRectTexture));

    // Анимация бега(Walk)
    WalkAnimation.AnimTexture.loadFromFile(PlayerTexturePath);

    WalkAnimation.FrameSpeed = 10.f;

    WalkAnimation.FrameRect.emplace_back(sf::IntRect(3, 16, static_cast<int>(PLAYER_SIZE.x),
                                                     static_cast<int>(PLAYER_SIZE.y)));

    WalkAnimation.FrameRect.emplace_back(sf::IntRect(17, 16, static_cast<int>(PLAYER_SIZE.x),
                                                     static_cast<int>(PLAYER_SIZE.y)));

    WalkAnimation.FrameRect.emplace_back(sf::IntRect(35, 16, static_cast<int>(PLAYER_SIZE.x),
                                                     static_cast<int>(PLAYER_SIZE.y)));

    // Анимация прыжка(Jump)
    // Анимация прыжка, когда персонаж летит вверх
    JumpUpAnimation.AnimTexture.loadFromFile(PlayerTexturePath);

    JumpUpAnimation.FrameSpeed = 0.f;
    JumpUpAnimation.FrameRect.emplace_back(sf::IntRect(2, 32, static_cast<int>(PLAYER_SIZE.x),
                                                       static_cast<int>(PLAYER_SIZE.y)));

    // Анимация прыжка, когда персонаж уже прыгнул и летит вниз
    JumpDownAnimation.AnimTexture.loadFromFile(PlayerTexturePath);

    JumpDownAnimation.FrameSpeed = 2.f;

    JumpDownAnimation.FrameRect.emplace_back(sf::IntRect(18, 33, static_cast<int>(PLAYER_SIZE.x),
                                                         static_cast<int>(PLAYER_SIZE.y)));

    JumpDownAnimation.FrameRect.emplace_back(sf::IntRect(35, 33, static_cast<int>(PLAYER_SIZE.x),
                                                         static_cast<int>(PLAYER_SIZE.y)));

    JumpDownAnimation.FrameRect.emplace_back(sf::IntRect(51, 33, static_cast<int>(PLAYER_SIZE.x),
                                                         static_cast<int>(PLAYER_SIZE.y)));


    // Анимация смерти персонажа
    DeathAnimation.FrameRect.emplace_back(sf::IntRect(0, 50, 15, 60));
}

/**
* @brief Обработка выстрелов персонажа.
*
* @param BulletsVectorPtr Указатель на вектор пуль для добавления новой пули.
* @param SoundManager Менеджер звуков для воспроизведения звуков выстрелов.
* @param SpriteManager Менеджер спрайтов для загрузки текстур.
* @param ParticleSystem Менеджер систем частиц для создания эффектов выстрелов.
*/
void APlayer::HandlePlayerShoots(std::vector<ABullet*>& BulletsVectorPtr,
                                 ASoundManager& SoundManager,
                                 ASpriteManager& SpriteManager,
                                 AParticleSystemManager& ParticleSystem) const
{
    // Создание нового снаряда и добавление его в вектор снарядов
    const float BulletOffsetX = bIsMoveRight ? 12.f : 0.f;
    constexpr float BulletOffsetY = 11.f;
    const sf::IntRect BulletRectTexture = sf::IntRect(7, 72, static_cast<int>(BULLET_SIZE.x),
                                                      static_cast<int>(BULLET_SIZE.y));

    BulletsVectorPtr.emplace_back(new ABullet(bIsMoveRight, BULLET_SPEED + 10, BULLET_DAMAGE, BulletRectTexture,
                                              EBulletType::EBT_ShootAtEnemy,
                                              EBulletDestroyEffect::EBDE_BulletDefaultDestroy,
                                              sf::Vector2f(ActorCollisionRect.left + BulletOffsetX,
                                                           ActorCollisionRect.top + BulletOffsetY),
                                              SpriteManager));

    // Проигрываем эффект "Пламя из ствола"
    const float EffectOffsetX = bIsMoveRight ? 11.f : 1.f;
    constexpr float EffectOffsetY = 8.f;
    ParticleSystem.PlayEffectWithPosition("MuzzleFlash",
                                          {
                                              ActorCollisionRect.left + EffectOffsetX,
                                              ActorCollisionRect.top + EffectOffsetY
                                          },
                                          {0.f, 0.f}, {0.7f, 0.7f}, bIsMoveRight);

    // Проиграть звук выстрела
    SoundManager.PlaySound("PlayerShot");
}

/**
 * @brief Обработка движения персонажа.
 * Устанавливает скорость персонажа в зависимости от нажатых клавиш.
 * 
 * @param DeltaTime Время, прошедшее с предыдущего кадра.
 */
void APlayer::HandlePlayerMove(float DeltaTime)
{
    // Сброс скорости по оси X
    ActorVelocity.x = 0.f;

    if (!bIsDeath && bIsUseController)
    {
        // Движение влево
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            bIsMoveRight = false;
            ActorVelocity.x = -PawnSpeed * DeltaTime;
            ActorSprite.setScale(-1.f * DRAW_SCALE.x, 1.f * DRAW_SCALE.y);
        }
        // Движение вправо
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            bIsMoveRight = true;
            ActorVelocity.x = PawnSpeed * DeltaTime;
            ActorSprite.setScale(1.f * DRAW_SCALE.x, 1.f * DRAW_SCALE.y);
        }
        // Движение вверх (подъем по лестнице)
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            if (bIsOnLadder)
            {
                ActorVelocity.y = PawnSpeed * DeltaTime;
            }
        }
        
        // Прыжок
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (bIsPawnJump)
            {
                bIsPawnJump = false;
                ActorVelocity.y = PlayerJumpSpeed * DeltaTime;
            }
        }
    }
}

/**
* @brief Применение отталкивания персонажа.
*
* @param bFromRight Флаг, указывающий направление отталкивания (справа или слева).
* @param PushForce Сила отталкивания.
* @param DeltaTime Время, прошедшее между последними кадрами.
*/
void APlayer::ApplyPushBack(bool bFromRight, float PushForce, float DeltaTime)
{
    // Если персонаж движется вправо и пуля прилетела справа, отталкиваем влево
    // Если персонаж движется влево и пуля прилетела слева, отталкиваем вправо
    // Иначе, если пуля прилетела слева, а персонаж движется вправо, отталкиваем вправо
    // И наоборот для других случаев

    // Устанавливаем силу толчка и направление
    // Определяем направление отталкивания
    if (bFromRight)
    {
        if (bIsMoveRight)
        {
            ActorVelocity.x -= PushForce * DeltaTime;
        }
        else
        {
            ActorVelocity.x -= PushForce * DeltaTime;
        }
    }
    else
    {
        if (bIsMoveRight)
        {
            ActorVelocity.x += PushForce * DeltaTime;
        }
        else
        {
            ActorVelocity.x += PushForce * DeltaTime;
        }
    }

    // Задаем вертикальное движение, если нужно
    ActorVelocity.y += 10.f * DeltaTime;

    // Запрещаем прыжки во время отталкивания и использовать контроллер
    bIsPawnJump = false;
    bIsUseController = false;
}

/**
 * @brief Обновление позиции и состояния персонажа.
 * Включает в себя обработку гравитации и проверку столкновений с объектами.
 * 
 * @param DeltaTime Время, прошедшее с предыдущего кадра.
 * @param SoundManager Менеджер, отвечаюсь за воспроизведение звука
 */
void APlayer::UpdatePlayerMove(float DeltaTime, ASoundManager& SoundManager)
{
    // Применение гравитации для падения персонажа
    ActorVelocity.y += GRAVITY * DeltaTime;

    // Проверка отталкивания
    if (bIsBeingPushedBack)
    {
        float ElapsedTime = PushingTime.getElapsedTime().asSeconds();

        // Проверяем, прошло ли время отталкивания
        if (ElapsedTime <= MaxPushBackTime)
        {
            // Пока время не истекло, продолжаем отталкивать персонажа
            ApplyPushBack(bisBulletFromRight, 450.f, DeltaTime);
        }
        else
        {
            // Останавливаем отталкивание после истечения времени и даём использовать контроллер
            bIsBeingPushedBack = false;
            bIsUseController = true;
        }
    }

    // Обновление позиции персонажа на основе скорости
    ActorCollisionRect.left += ActorVelocity.x;
    ActorCollisionRect.top -= ActorVelocity.y;

    // Обновление позиции спрайта персонажа и его коллизии
    ActorDrawPosition = {
        ActorCollisionRect.left + (PLAYER_SIZE.x / 2.f),
        ActorCollisionRect.top + (PLAYER_SIZE.y / 2.f)
    };

    // Анимация персонажа
    if (bIsPawnJump)
    {
        if (std::abs(ActorVelocity.x) > 0.0001f)
        {
            CurrentAnimation = &WalkAnimation;
        }
        else
        {
            CurrentAnimation = &IdleAnimation;
        }
    }
    else
    {
        if (ActorVelocity.y < 0.f)
        {
            CurrentAnimation = &JumpDownAnimation;
        }
        else
        {
            CurrentAnimation = &JumpUpAnimation;
        }
    }

    // Если здоровье меньше или равно 0, то проигрываем анимацию смерти и меняем флаг на "Умер"
    if (PawnCurrentHealth <= DEATH)
    {
        bIsDeath = true;
        bIsUseController = false;
        CurrentAnimation = &DeathAnimation;

        // Проигрываем звук смерти только один раз
        if (!bIsDeathSoundPlayed)
        {
            SoundManager.PlaySound("PlayerDeath");

            // Обновляем флаг, чтобы звук больше не проигрывался
            bIsDeathSoundPlayed = true;
        }
    }

    // Если персонаж мёрт, то от текущего здоровья отнимаем текущее,
    // чтобы всегда шкала здоровья при смерте равнялась 0
    if (bIsDeath)
    {
        // Устанавливаем здоровье персонажу
        TakeAwayPawnCurrentHealth(GetPawnCurrentHealth());
    }

    ActorSprite.setTextureRect(CurrentAnimation->GetCurrentFrame());
    CurrentAnimation->AnimationUpdate(DeltaTime);

    // Установка позиции спрайта персонажа
    ActorSprite.setPosition(ActorDrawPosition);
}

/**
* @brief Обработка события попадания ракеты.
*
* @param bFromRight Флаг, указывающий направление попадания (справа или слева).
*/
void APlayer::OnRocketHit(bool bFromRight)
{
    // Устанавливаем флаг отталкивания
    bIsBeingPushedBack = true;

    // Запускаем таймер для отталкивания
    PushingTime.restart();

    // Сохраняем значение, откуда прилетела пуля, чтобы повтарять несколько раз вызов функции
    bisBulletFromRight = bFromRight;
}

/**
* @brief Установка состояния персонажа (мёртв или нет).
*
* @param IsDeath Флаг, указывающий, мёртв ли персонаж.
*/
void APlayer::SetIsDeathPlayer(bool IsDeath)
{
    bIsDeath = IsDeath;
}

/**
* @brief Сброс состояния персонажа.
*/
void APlayer::ResetPlayer()
{
    // Инициализируйте необходимые параметры
    bIsUseController = true;
    bIsMoveRight = true;
    bIsOnLadder = false;
    bIsDeath = false;
    bIsDeathSoundPlayed = false;
    bIsPawnJump = true;
    bIsBeingPushedBack = false;
    bisBulletFromRight = false;

    // Восстановите здоровье и позицию
    PawnCurrentHealth = PawnMaxHealth;
    ActorVelocity = {0.f, 0.f};

    // Прямоугольник персонажа с учетом масштабирования
    ActorCollisionRect = {
        0.f, 0.f,
        PLAYER_SIZE.x,
        PLAYER_SIZE.y
    };

    IdleAnimation.ResetAnimation();
    WalkAnimation.ResetAnimation();
    JumpUpAnimation.ResetAnimation();
    JumpDownAnimation.ResetAnimation();
    DeathAnimation.ResetAnimation();

    CurrentAnimation = nullptr;
}

/**
 * @brief Отрисовка персонажа на экране.
 * Отображает спрайт персонажа, его коллизию и шкалу здоровья.
 * 
 * @param Window Окно, на котором происходит отрисовка.
 */
void APlayer::DrawActor(sf::RenderWindow& Window)
{
    // Отрисовка спрайта и коллизии персонажа
    Window.draw(ActorSprite);

    PawnHealthBarPtr->DrawHealthBar(Window);
    Window.draw(HealthBarSprite);
}

/**
* @brief Получить по ссылке данные, если персонаж находить рядом с лестницей и может карабкаться по ней.
* 
* @return Текущие состояние заимодействие с лестницей.
*/
bool& APlayer::GetIsOnLadder()
{
    return bIsOnLadder;
}

/**
* @brief Проверяет, мёртв ли персонаж.
* 
* @return true, если персонаж мёртв, иначе false.
*/
bool APlayer::GetIsDeathPlayer() const
{
    return bIsDeath;
}