// ReSharper disable CppMemberFunctionMayBeStatic
#include "CollisionManager.h"

/**
* @brief Конструктор класса ACollisionManager.
* 
* Инициализирует ссылки на персонажа и игровую карту.
* 
* @param GameState Ссылка на объект состояния игры.
* @param Player Ссылка на объект персонажа.
* @param GameMap Ссылка на объект игровой карты.
*/
ACollisionManager::ACollisionManager(AGameState& GameState, APlayer& Player, AGameMap& GameMap)
    : GameStateRef(GameState),
      PlayerRef(Player),
      GameMapRef(GameMap)
{
}

/**
* @brief Проверяет столкновения всех пуль с врагами и игровой картой.
* 
* @param BulletsVectorPtr Указатель на вектор пуль.
* @param EnemyVectorPtr Указатель на вектор врагов.
* @param KeysVectorPtr Указатель на вектор ключей.
* @param Player Ссылка на персонажа.
* @param SpriteManager Ссылка на менеджер спрайтов.
* @param SoundManager Ссылка на менеджер звуков.
* @param ParticleSystem Ссылка на менеджер частиц.
*/
void ACollisionManager::CheckAllBulletCollisions(std::vector<ABullet*>& BulletsVectorPtr,
                                                 std::vector<AEnemy*>& EnemyVectorPtr,
                                                 std::vector<AKeyObject*>& KeysVectorPtr,
                                                 APlayer& Player,
                                                 ASpriteManager& SpriteManager,
                                                 ASoundManager& SoundManager,
                                                 AParticleSystemManager& ParticleSystem) const
{
    // Векторы для хранения пуль и врагов для удаления из основных векторов
    std::vector<ABullet*> BulletsToRemove;
    std::vector<APawn*> EnemyToRemove;

    for (ABullet* Bullet : BulletsVectorPtr)
    {
        // Проверяем столкновение с игровой картой и её границами
        if (CheckBulletCollisionWithGameMap(*Bullet))
        {
            ParticleSystem.PlayEffectWithPosition("BulletDestroy", Bullet->GetActorPosition(), {-4, -4});
            BulletsToRemove.emplace_back(Bullet);
            continue;
             
        }

        if (CheckBulletCollisionWithPawn(*Bullet, Player.GetActorCollisionRect()) &&
            Bullet->GetBulletType() == EBulletType::EBT_ShootAtPlayer)
        {
            // Определение направления отталкивания: если пуля находится правее игрока(true),
            // то отталкивание будет влево, иначе вправо
            bool bFromRight = Bullet->GetActorPosition().x > Player.GetActorPosition().x;

            // Воспроизводим анимацию на основе типа уничтожения пули
            switch (Bullet->GetBulletDestroyEffectType())
            {
            case EBulletDestroyEffect::EBDE_RocketDestroy:
                {
                    // Отталкиваем персонажа и проигрываем анимацию взрыва рокеты
                    Player.OnRocketHit(bFromRight);
                    SoundManager.PlaySound("RocketExplosion");
                    ParticleSystem.PlayEffectWithPosition("RocketDestroy", Bullet->GetActorPosition(),
                                                          {-6, -7}, {0.8f, 0.8f});
                    break;
                }
            default:
                {
                    ParticleSystem.PlayEffectWithPosition("BulletDestroy", Bullet->GetActorPosition(), {-4, -4});
                    break;
                }
            }

            // Уменьшаем здоровье персонажа, если пуля попала в него
            Player.TakeAwayPawnCurrentHealth(Bullet->GetBulletDamage());

            // Воспроизвести звук потери здоровья, но если здоровья дошло до нуля,
            // то не воспроизводим звук, так как, уже будет воспроизведён другой звук
            if (Player.GetPawnCurrentHealth() > DEATH)
            {
                SoundManager.PlaySound("PlayerDamage");
            }
            
            BulletsToRemove.emplace_back(Bullet);
            continue;
        }

        // Проверяем столкновение с врагом
        for (APawn* Enemy : EnemyVectorPtr)
        {
            if (CheckBulletCollisionWithPawn(*Bullet, Enemy->GetActorCollisionRect()) &&
                Bullet->GetBulletType() == EBulletType::EBT_ShootAtEnemy)
            {
                // Воспроизводим анимацию уничтожения для врагов
                ParticleSystem.PlayEffectWithPosition("BulletDestroy", Bullet->GetActorPosition(), {-4, -4});
                SoundManager.PlaySound("BulletHit");

                BulletsToRemove.emplace_back(Bullet);                

                Enemy->TakeAwayPawnCurrentHealth(Bullet->GetBulletDamage());

                if (Enemy->GetPawnCurrentHealth() <= DEATH)
                {
                    EnemyToRemove.emplace_back(Enemy);
                }

                // Выходим из цикла по врагам, так как пуля должна быть удалена
                break;
            }
        }
    }

    // Удаляем пули, столкнувшиеся с препятствиями или врагами
    for (ABullet* BulletRemove : BulletsToRemove)
    {
        std::erase(BulletsVectorPtr, BulletRemove);
        delete BulletRemove;
    }

    // Удаляем врагов, которых убили пули
    for (APawn* EnemyRemove : EnemyToRemove)
    {
        ABossEnemy* BossEnemy = dynamic_cast<ABossEnemy*>(EnemyRemove);
        if (BossEnemy && BossEnemy->GetPawnCurrentHealth() <= DEATH)
        {
            BossEnemy->OnDeath(KeysVectorPtr, SpriteManager, BossEnemy->GetActorPosition());
        }

        SoundManager.PlaySound("EnemyDeath");
        std::erase(EnemyVectorPtr, EnemyRemove);
        delete EnemyRemove;
    }
}

/**
* @brief Обрабатывает столкновение пешки с игровой картой.
* 
* @param PawnRect Прямоугольник коллизии персонажа.
* @param ObjectVelocity Вектор скорости объекта персонажа.
* @param bCanJump Может ли персонаж прыгать.
* @param bCanClimb Может ли персонаж карабкаться.
* @param SoundManager Указатель на менеджер звуков (может быть nullptr).
*/
void ACollisionManager::HandlePawnCollisionWithGameMap(sf::FloatRect& PawnRect,
                                                       sf::Vector2f& ObjectVelocity,
                                                       bool& bCanJump,
                                                       bool& bCanClimb,
                                                       ASoundManager* SoundManager) const
{
    bCanClimb = false;

    // Проверяем столкнование персонажа с препятствиями игровой карты
    for (auto it = GameMapRef.GetGameMapCollisionVector().begin(); it != GameMapRef.GetGameMapCollisionVector().end();)
    {
        auto& Obstacle = *it;

        if (PawnRect.intersects(Obstacle))
        {
            // Рассчитываем перекрытие
            float OverlapLeft = (PawnRect.left + PawnRect.width) - Obstacle.left;
            float OverlapRight = (Obstacle.left + Obstacle.width) - PawnRect.left;
            float OverlapTop = (PawnRect.top + PawnRect.height) - Obstacle.top;
            float OverlapBottom = (Obstacle.top + Obstacle.height) - PawnRect.top;

            bool FromLeft = std::abs(OverlapLeft) < std::abs(OverlapRight);
            bool FromTop = std::abs(OverlapTop) < std::abs(OverlapBottom);

            float MinOverlapX = FromLeft ? OverlapLeft : OverlapRight;
            float MinOverlapY = FromTop ? OverlapTop : OverlapBottom;

            // Если перекрытие по X меньше, чем по Y, это горизонтальное столкновение
            if (std::abs(MinOverlapX) < std::abs(MinOverlapY))
            {
                // Корректируем положение по X
                PawnRect.left += FromLeft ? -OverlapLeft : OverlapRight;

                // Останавливаем горизонтальное движение
                ObjectVelocity.x = 0.f;
            }
            // В противном случае это вертикальное столкновение
            else
            {
                // Корректируем положение по Y
                PawnRect.top += FromTop ? -OverlapTop : OverlapBottom;

                // Останавливаем вертикальное движение
                ObjectVelocity.y = 0.f;

                if (FromTop)
                {
                    bCanJump = true;
                }
            }
        }

        // Проверка условия для удаления Obstacle (коллизия двери)
        if (AChestObject::ChestOpenCountStatic == OPEN_ALL_CHESTS)
        {
            // Координаты и размер коллизии двери
            sf::FloatRect DoorCollisionRect(5056.33f, 481.08f, 15.33f, 94.58f);

            if (Obstacle.intersects(DoorCollisionRect))
            {
                // Удаление из вектора
                it = GameMapRef.GetGameMapCollisionVector().erase(it);

                // Переход к следующему элементу
                continue;
            }
        }

        // Переход к следующему элементу, если ничего не было удалено
        ++it;
    }

    // Когда мы пересечём ворота, меняем состояние на Путь к эвакуации,
    // будет становится фоновая музыка тише, а вертолёт громче
    if (PawnRect.intersects(GameMapRef.GetSpawnSoundHelicopterLayer()))
    {
        GameStateRef.SetGameState(EGameState::EGS_EscapeRoute);
    }

    // Дойдя до флага, мы устанавливаем состояние игры на миссия выполнена 
    if (PawnRect.intersects(GameMapRef.GetMissionAccomplishedLayer()))
    {
        GameStateRef.SetGameState(EGameState::EGS_MissionCompleted);
    }

    // Проверяем, находится ли персонаж на участки карты лавы, который наносит урон
    for (const auto& DamageLayer : GameMapRef.GetLavaDamageCollisionVector())
    {
        if (PawnRect.intersects(DamageLayer))
        {
            // Проиграть звук лавы
            SoundManager->PlaySound("Lava");

            // Устанавливаем флаг для персонажа(игрока) на "Умер", если пересекли коллизию с уроном
            bool PlayerDeath = true;
            PlayerRef.SetIsDeathPlayer(PlayerDeath);

            break;
        }
    }

    // Проверяем, находится ли персонаж на участки карты с пиками, который наносит урон
    for (const auto& DamageLayer : GameMapRef.GetPeaksDamageCollisionVector())
    {
        if (PawnRect.intersects(DamageLayer))
        {
            // Проиграть звук пронзания пик
            SoundManager->PlaySound("Peaks");

            // Устанавливаем флаг для персонажа(игрока) на "Умер", если пересекли коллизию с уроном
            bool PlayerDeath = true;
            PlayerRef.SetIsDeathPlayer(PlayerDeath);

            break;
        }
    }

    // Проверяем, находится ли персонаж на лестнице
    for (const auto& LadderLayer : GameMapRef.GetLadderCollisionVector())
    {
        if (PawnRect.intersects(LadderLayer))
        {
            bCanClimb = true;
            break;
        }
    }
}

/**
* @brief Проверяет столкновение с ключами.
* 
* @param KeysVectorPtr Указатель на вектор ключей.
* @param SoundManager Ссылка на менеджер звуков.
*/
void ACollisionManager::CheckCollisionWithKey(std::vector<AKeyObject*>& KeysVectorPtr, ASoundManager& SoundManager)
{
    // Вектор для хранения найденных ключей
    std::vector<AKeyObject*> FoundKeys;

    for (auto it = KeysVectorPtr.begin(); it != KeysVectorPtr.end();)
    {
        // Воспроизвести звук, когда персонаж подобрал ключ
        SoundManager.PlaySound("KeyFound");

        if (CheckCollision(PlayerRef.GetActorCollisionRect(), (*it)->GetActorCollisionRect()))
        {
            (*it)->SetKeyFound(false);
            GameStateRef.GetFoundKeysVector().emplace_back(*it); // Добавляем ключ в вектор найденных ключей
            it = KeysVectorPtr.erase(it); // Удаляем ключ из общего вектора
        }
        else
        {
            ++it;
        }
    }
}

/**
* @brief Проверяет столкновение с игровой картой.
* 
* @param ActorRect Прямоугольник коллизии актера.
* @param ObjectVelocity Вектор скорости объекта.
* @param EnemyPtr Указатель на врага.
*/
void ACollisionManager::CheckCollisionWithGameMap(sf::FloatRect& ActorRect,
                                                  sf::Vector2f& ObjectVelocity,
                                                  AEnemy* EnemyPtr) const
{
    // Проверяем столкнование с препятствиями игровой карты
    for (const auto& Obstacle : GameMapRef.GetGameMapCollisionVector())
    {
        if (ActorRect.intersects(Obstacle))
        {
            // Рассчитываем перекрытие по всем сторонам препятствия
            float OverlapLeft = (ActorRect.left + ActorRect.width) - Obstacle.left;
            float OverlapRight = (Obstacle.left + Obstacle.width) - ActorRect.left;
            float OverlapTop = (ActorRect.top + ActorRect.height) - Obstacle.top;
            float OverlapBottom = (Obstacle.top + Obstacle.height) - ActorRect.top;

            // Определяем направление, откуда пришло столкновение
            bool FromLeft = std::abs(OverlapLeft) < std::abs(OverlapRight);
            bool FromTop = std::abs(OverlapTop) < std::abs(OverlapBottom);

            // Выбираем минимальное перекрытие по X и Y
            float MinOverlapX = FromLeft ? OverlapLeft : OverlapRight;
            float MinOverlapY = FromTop ? OverlapTop : OverlapBottom;

            // Если перекрытие по X меньше, чем по Y, это горизонтальное столкновение
            if (std::abs(MinOverlapX) < std::abs(MinOverlapY))
            {
                // Корректируем положение по X
                ActorRect.left += FromLeft ? -OverlapLeft : OverlapRight;

                // Останавливаем горизонтальное движение
                ObjectVelocity.x = 0.f;

                if (EnemyPtr)
                {
                    EnemyPtr->ChangeDirection();
                }
            }
            else // В противном случае это вертикальное столкновение
            {
                // Корректируем положение по Y
                ActorRect.top += FromTop ? -OverlapTop : OverlapBottom;

                // Останавливаем вертикальное движение
                ObjectVelocity.y = 0.f;
            }
        }
    }
}

/**
* @brief Проверяет столкновение пули с игровой картой.
* 
* @param Bullet Пуля для проверки столкновения.
* 
* @return true, если есть столкновение с игровой картой, иначе false.
*/
bool ACollisionManager::CheckBulletCollisionWithGameMap(AActor& Bullet) const
{
    // Получаем границы камеры (вьюпорт)
    sf::FloatRect CameraBounds = GetCameraBounds();

    // Проверяем, выходит ли пуля за границы видимой области
    if (!CameraBounds.intersects(Bullet.GetActorCollisionRect()))
    {
        return true; // Пуля вышла за границы экрана
    }

    // Перебираем все объекты коллизий на игровой карте
    for (const auto& Collision : GameMapRef.GetGameMapCollisionVector())
    {
        if (Bullet.GetActorCollisionRect().intersects(Collision))
        {
            return true; // Столкновение обнаружено
        }
    }

    return false; // Столкновение не обнаружено
}

/**
* @brief Проверяет столкновение пули с объектом (персонажем или врагом).
* 
* @param Bullet Пуля для проверки столкновения.
* @param PawnRect Прямоугольник объекта для проверки.
* 
* @return true, если есть столкновение с объектом, иначе false.
*/
bool ACollisionManager::CheckBulletCollisionWithPawn(AActor& Bullet, const sf::FloatRect& PawnRect) const
{
    // Проверяем, пересекается ли коллайдер пули с прямоугольником пешки
    return Bullet.GetActorCollisionRect().intersects(PawnRect);
}


/**
* @brief Проверяет пересечение двух прямоугольников.
* 
* @param Rect1 Первый прямоугольник.
* @param Rect2 Второй прямоугольник.
* 
* @return true, если прямоугольники пересекаются, иначе false.
*/
bool ACollisionManager::CheckCollision(const sf::FloatRect& Rect1, const sf::FloatRect& Rect2)
{
    return Rect1.intersects(Rect2);
}

/**
* @brief Получает границы камеры.
* 
* @return sf::FloatRect Прямоугольник границ камеры.
*/
sf::FloatRect ACollisionManager::GetCameraBounds() const
{
    // Получаем объект вида камеры
    const sf::View& CameraView = GameStateRef.GetViewPlayer();

    // Вычисляем видимую область на карте
    return sf::FloatRect(CameraView.getCenter() - CameraView.getSize() / 2.f, CameraView.getSize());
}
