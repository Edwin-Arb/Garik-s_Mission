#include "Enemy.h"
#include "../Manager/CollisionManager.h"
#include "BossEnemy/BossEnemy.h"

/**
 * @brief Конструктор врага.
 * 
 * @param MaxPatrolDistance Максимальная дистанция, на которую может двигаться враг.
 * @param StartPosition Начальная позиция врага.
 */
AEnemy::AEnemy(const float MaxPatrolDistance, const sf::Vector2f& StartPosition)
    : bIsMoveRight(true),
      bIsPlayerDetected(false),
      EnemyScale(1.f),
      MinMoveDistance(MaxPatrolDistance / 2.f),
      MaxMoveDistance(MaxPatrolDistance / 2.f),
      TimeAfterDetection(0.f),
      DetectionTimeout(2.0f), // Время задержки после потери игрока из виду
      ShootingDelay(0.5f), // Задержка между выстрелами для врагов
      EnemyTexturePath(ASSETS_PATH + "MainTiles/Enemy.png"),
      EnemyOrigin(0.5f, 0.5f),
      EnemyHealthBarSize(10.f, 2.f),
      EnemyStartPosition(StartPosition),
      LineTraceDetectionArea(sf::Vector2f{230.f, 10.f}) // Ширина и высота detection area
{
    PawnSpeed = ENEMY_SPEED;

    PawnMaxHealth = BASE_ENEMY_MAX_HEALTH;
    PawnCurrentHealth = BASE_ENEMY_MAX_HEALTH;

    ActorSize = {ENEMY_SIZE.x * DRAW_SCALE.x, ENEMY_SIZE.y * DRAW_SCALE.y};

    ActorCollisionRect = {
        StartPosition.x, StartPosition.y,
        ActorSize.x, ActorSize.y
    };
}

/**
 * @brief Инициализация врага.
 * 
 * @param SpriteManager Менеджер спрайтов для загрузки ресурсов.
 */
void AEnemy::InitEnemy(ASpriteManager& SpriteManager)
{
    if (!WalkAnimation.FrameRect.empty())
    {
        WalkAnimation.ResetAnimation();
    }

    // Установить центр спрайта и коллизии для ЛайнТрейса
    SpriteManager.SetShapeRelativeOrigin(LineTraceDetectionArea, 0.5f, 0.5f);

    // TODO: Отрисовать прямоугольник коллизии для визуализации (отключить после проверки)
    // LineTraceDetectionArea.setFillColor(sf::Color(155, 0, 0, 128));

    // Инициализация шкалы здоровья врага
    InitPawnHealthBar(EnemyHealthBarSize,
                      sf::Color::Red, sf::Color(181, 184, 177),
                      SpriteManager);
}

/**
 * @brief Враг открывает огонь в сторону персонажа.
 *
 * @param SpawnBulletOffsetY Вертикальное смещение для спауна пуль.
 * @param BulletsVectorPtr Указатель на вектор пуль, в который добавляются выстрелы.
 * @param SpriteManager Менеджер спрайтов для управления спрайтами.
 */
void AEnemy::EnemyShoot(const float SpawnBulletOffsetY,
                        std::vector<ABullet*>& BulletsVectorPtr,
                        ASpriteManager& SpriteManager)
{
    // Определения текстуры для пули в зависимости от типа врага
    sf::IntRect BulletTextureRect;

    if (dynamic_cast<ABossEnemy*>(this))
    {
        BulletTextureRect = sf::IntRect(3, 7, static_cast<int>(BULLET_SIZE.x) + 8,
                                        static_cast<int>(BULLET_SIZE.y) + 2);
    }
    else
    {
        BulletTextureRect = sf::IntRect(7, 72, static_cast<int>(BULLET_SIZE.x),
                                        static_cast<int>(BULLET_SIZE.y));
    }

    // Выстрел из оружия, если персонаж обнаружен
    if (bIsPlayerDetected)
    {
        const float SpawnBulletOffsetX = bIsMoveRight ? ActorSize.x : 0.f;

        // Добавляем пулю в вектор, если враг сделал выстрел
        BulletsVectorPtr.emplace_back(new ABullet(bIsMoveRight, BULLET_SPEED, BULLET_DAMAGE, BulletTextureRect,
                                                  EBulletType::EBT_ShootAtPlayer,
                                                  EBulletDestroyEffect::EBDE_BulletDefaultDestroy,
                                                  sf::Vector2f(ActorCollisionRect.left + SpawnBulletOffsetX,
                                                               ActorCollisionRect.top + SpawnBulletOffsetY),
                                                  SpriteManager));
    }
}

/**
 * @brief Изменения направления врага.
 */
void AEnemy::ChangeDirection()
{
    bIsMoveRight = !bIsMoveRight;
    ActorVelocity.x = bIsMoveRight ? PawnSpeed : -PawnSpeed;
}

/**
 * @brief Вычисление позиции отрисовки врага.
 */
void AEnemy::CalculateEnemyDrawPosition()
{
    ActorDrawPosition =
    {
        ActorCollisionRect.left + (ActorSize.x * DRAW_SCALE.x) / 2.f,
        ActorCollisionRect.top + (ActorSize.y * DRAW_SCALE.y) / 2.f
    };
}

/**
 * @brief Обнаружение персонажа в окрестности.
 * 
 * @param DeltaTime Время, прошедшее с последнего обновления.
 * @param Player Ссылка на объект персонажа.
 * @param GameMap Ссылка на объект игровой карты.
 * @param CollisionManager Менеджер коллизий в игре.
 */
void AEnemy::DetectPlayer(float DeltaTime,
                          APlayer& Player,
                          AGameMap& GameMap,
                          ACollisionManager& CollisionManager)
{
    // Изначально предполагаем, что персонаж не обнаружен
    bool bPlayerVisible = false;

    // Проверка, находится ли персонаж в зоне обнаружения врага
    if (LineTraceDetectionArea.getGlobalBounds().intersects(Player.GetActorCollisionRect()))
    {
        sf::Vector2f EnemyPosition = this->GetActorPosition();
        sf::Vector2f PlayerPosition = Player.GetActorPosition();

        // Направление от врага к игроку
        sf::Vector2f Direction = PlayerPosition - EnemyPosition;
        float DistanceToPlayer = std::sqrt(Direction.x * Direction.x + Direction.y * Direction.y);

        // Нормализуем направление
        Direction /= DistanceToPlayer;

        // Шаг для трассировки (размер шага, с которым будем проверять препятствия на пути)
        constexpr float StepSize = 5.0f; // Увеличивайте для производительности, уменьшайте для точности

        // Флаг, указывающий, что видимость блокируется
        bool bIsViewBlocked = false;

        // Шаг за шагом проверяем наличие препятствий на пути от врага к игроку
        for (float Step = 0.0f; Step < DistanceToPlayer; Step += StepSize)
        {
            // Вычисляем текущую точку на пути трассировки
            sf::Vector2f CurrentPosition = EnemyPosition + Direction * Step;

            // Создаем небольшой прямоугольник вокруг текущей точки для проверки коллизии
            sf::FloatRect TracePointRect(CurrentPosition.x, CurrentPosition.y, StepSize, StepSize);

            // Проверяем коллизию с каждым препятствием
            for (const auto& Obstacle : GameMap.GetGameMapCollisionVector())
            {
                if (CollisionManager.CheckCollision(TracePointRect, Obstacle))
                {
                    // Если обнаружено препятствие, блокирующее видимость, прерываем цикл
                    bIsViewBlocked = true;
                    break;
                }
            }

            // Если видимость заблокирована, прерываем основной цикл
            if (bIsViewBlocked)
            {
                break;
            }
        }

        // Если видимость не блокируется препятствием, враг обнаружил игрока
        if (!bIsViewBlocked)
        {
            bPlayerVisible = true;
        }
    }

    // Если игрок виден, сбросить таймер ожидания
    if (bPlayerVisible)
    {
        bIsPlayerDetected = true;
        TimeAfterDetection = 0.0f;
    }
    else
    {
        // Если игрок не виден, запускаем таймер ожидания перед тем как враг вернется к обычному патрулированию
        if (TimeAfterDetection < DetectionTimeout)
        {
            TimeAfterDetection += DeltaTime; // Добавляем реальное время между кадрами
        }
        else
        {
            // После истечения таймера враг перестаёт считать, что игрок обнаружен
            bIsPlayerDetected = false;
        }
    }
}

/**
 * @brief Обновление направления и скорости врага.
 * 
 * @param DeltaTime Время, прошедшее с последнего обновления.
 * @param Player Ссылка на объект персонажа.
 */
void AEnemy::UpdateDirectionAndVelocity(float DeltaTime, APlayer& Player)
{
    ActorVelocity.y += GRAVITY * DeltaTime;

    // Если персонаж обнаружен и не блокируется препятствием, разворачиваем врага в сторону персонажа
    if (bIsPlayerDetected)
    {
        ActorVelocity.x = 0.f;

        // Определяем направление персонажа относительно врага
        if (Player.GetActorCollisionRect().left < ActorCollisionRect.left)
        {
            // персонаж слева от врага
            bIsMoveRight = false;
            ActorSprite.setScale(-EnemyScale * DRAW_SCALE.x, EnemyScale * DRAW_SCALE.y);
        }
        else
        {
            // персонаж справа от врага
            bIsMoveRight = true;
            ActorSprite.setScale(EnemyScale * DRAW_SCALE.x, EnemyScale * DRAW_SCALE.y);
        }
    }
    else
    {
        // Движение влево или вправо в зависимости от текущего направления    
        ActorVelocity.x = bIsMoveRight ? PawnSpeed * DeltaTime : -PawnSpeed * DeltaTime;
        ActorSprite.setScale(bIsMoveRight ? EnemyScale * DRAW_SCALE.x : -EnemyScale * DRAW_SCALE.x,
                             EnemyScale * DRAW_SCALE.y);

        // Обновляем анимацию спрайта врага
        ActorSprite.setTextureRect(WalkAnimation.GetCurrentFrame());
        WalkAnimation.AnimationUpdate(DeltaTime);
    }
}

/**
 * @brief Обновление позиции врага на основе текущей скорости.
 */
void AEnemy::UpdatePosition()
{
    ActorCollisionRect.left += ActorVelocity.x;
    ActorCollisionRect.top -= ActorVelocity.y;
}

/**
 * @brief Обновление позиции зоны обнаружения персонажа.
 */
void AEnemy::UpdateDetectionAreaPosition()
{
    sf::Vector2f DetectionPosition = ActorDrawPosition;
    if (bIsMoveRight)
    {
        DetectionPosition.x += (ActorSize.x * DRAW_SCALE.x) / 2.f;
    }
    else
    {
        DetectionPosition.x -= (ActorSize.x * DRAW_SCALE.x) / 2.f + LineTraceDetectionArea.getSize().x;
    }
    LineTraceDetectionArea.setPosition(DetectionPosition);
}

/**
 * @brief Обновление дистанции движения врага.
 */
void AEnemy::UpdateMoveDistance()
{
    // Проверка достижения максимальной или минимальной дистанции
    const float DistanceMoved = ActorCollisionRect.left - EnemyStartPosition.x;
    if (DistanceMoved >= MaxMoveDistance || DistanceMoved <= -MinMoveDistance)
    {
        // Меняем направление движения только если персонаж не обнаружен
        if (!bIsPlayerDetected)
        {
            ChangeDirection();
        }
    }
}

/**
 * @brief Обновление движения врага.
 * 
 * @param DeltaTime Время, прошедшее с последнего обновления.
 * @param Player Ссылка на объект персонажа.
 * @param GameMap Ссылка на объект игровой карты.
 * @param CollisionManager Mенеджер коллизий.
 */
void AEnemy::UpdateEnemyMove(float DeltaTime,
                             APlayer& Player,
                             AGameMap& GameMap,
                             ACollisionManager& CollisionManager)
{
    DetectPlayer(DeltaTime, Player, GameMap, CollisionManager);
    UpdateDirectionAndVelocity(DeltaTime, Player);
    UpdatePosition();
    UpdateDetectionAreaPosition();
    UpdateMoveDistance();

    // Проверка столкновений врага с коллизиями карты
    CollisionManager.CheckCollisionWithGameMap(ActorCollisionRect,
                                               ActorVelocity,
                                               this);

    // Вычисление позиции отрисовки врага.
    CalculateEnemyDrawPosition();

    // Установить положение спрайта и прямоугольника коллизии с положением врага в игре
    ActorSprite.setPosition(ActorDrawPosition);
    LineTraceDetectionArea.setPosition(ActorDrawPosition);

    // Устанавливаем положение шкалы здоровья чуть выше врага
    UpdatePawnHealthBar(PawnCurrentHealth, PawnMaxHealth,
                        sf::Vector2f(ActorCollisionRect.left + (ActorSize.x / 2.f),
                                     ActorCollisionRect.top - 4.f));
}

/**
* @brief Отрисовка врага на экране.
* 
* @param Window Окно, в котором происходит отрисовка.
*/
void AEnemy::DrawActor(sf::RenderWindow& Window)
{
    // TODO: Не рисовать LineTrace, используется для отладки обнаружения персонажа
    //Window.draw(LineTraceDetectionArea);

    // Отрисовать врага
    Window.draw(ActorSprite);

    // Отрисовать шкалу здоровья врага
    PawnHealthBarPtr->DrawHealthBar(Window);
}

/**
* @brief Задержка между выстрелами для врагов.
* 
* @return Возвращаем значение задержки.
*/
float AEnemy::GetShootingDelay() const
{
    return ShootingDelay;
}
