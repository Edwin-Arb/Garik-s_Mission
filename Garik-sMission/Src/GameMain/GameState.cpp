#include "GameState.h"
#include <random>

/**
 * @brief Конструктор класса AGameState.
 * 
 * Инициализирует игровые объекты и менеджеры.
 */
AGameState::AGameState()
    : bIsGamePlaying(false),
      DialoguePtr(new AIntroDialogue),
      GameOverPtr(new AGameOver),
      CreditsScreenPtr(new ACreditsScreen),
      PlayerPtr(new APlayer),
      GameMapPtr(new AGameMap),
      SpriteManagerPtr(new ASpriteManager),
      CollisionManagerPtr(new ACollisionManager(*this, *PlayerPtr, *GameMapPtr)),
      ParticleSystemPtr(new AParticleSystemManager),
      SoundManagerPtr(new ASoundManager),
      GameTextPtr(new AUIText),
      MenuPtr(new AMenu),
      //FpsManagerPtr(new AFpsManager), // TODO: используется для тестирования производительности
      CurrentGameState(EGameState::EGS_Menu),
      PastGameState(EGameState::EGS_Menu)
{
    sf::Vector2f GatePosition(5060.f, 497.5f);
    FinalGatePtr = new AFinalGateObject(GatePosition);
}

/**
 * @brief Деструктор класса AGameState.
 * 
 * Освобождает память, удаляя игровые объекты и очищая вектора.
 */
AGameState::~AGameState()
{
    // Освобождение памяти указателей на игровые объекты
    if (DialoguePtr)
    {
        delete DialoguePtr;
        DialoguePtr = nullptr;
    }
    if (GameOverPtr)
    {
        delete GameOverPtr;
        GameOverPtr = nullptr;
    }
    if (PlayerPtr)
    {
        delete PlayerPtr;
        PlayerPtr = nullptr;
    }
    if (GameMapPtr)
    {
        delete GameMapPtr;
        GameMapPtr = nullptr;
    }
    if (CreditsScreenPtr)
    {
        delete CreditsScreenPtr;
        CreditsScreenPtr = nullptr;
    }
    if (SpriteManagerPtr)
    {
        delete SpriteManagerPtr;
        SpriteManagerPtr = nullptr;
    }
    if (CollisionManagerPtr)
    {
        delete CollisionManagerPtr;
        CollisionManagerPtr = nullptr;
    }
    if (ParticleSystemPtr)
    {
        delete ParticleSystemPtr;
        ParticleSystemPtr = nullptr;
    }
    if (SoundManagerPtr)
    {
        delete SoundManagerPtr;
        SoundManagerPtr = nullptr;
    }
    if (GameTextPtr)
    {
        delete GameTextPtr;
        GameTextPtr = nullptr;
    }
    if (MenuPtr)
    {
        delete MenuPtr;
        MenuPtr = nullptr;
    }
    if (FinalGatePtr)
    {
        delete FinalGatePtr;
        FinalGatePtr = nullptr;
    }

    // Очищаем вектор врагов
    for (auto& Enemy : EnemyVectorPtr)
    {
        delete Enemy;
    }
    EnemyVectorPtr.clear();

    // Очищаем вектор пуль
    for (auto& Bullet : BulletsVectorPtr)
    {
        delete Bullet;
    }
    BulletsVectorPtr.clear();

    // Очищаем вектор ключей
    for (auto& Key : KeysVectorPtr)
    {
        delete Key;
    }
    KeysVectorPtr.clear();
    FoundKeysVectorPtr.clear();

    // Очищаем вектор сундуков
    for (auto& Chest : ChestVectorPtr)
    {
        delete Chest;
    }
    ChestVectorPtr.clear();

    // Очищаем вектор восстанавливающих здоровье объектов
    for (auto& HealingHam : HealingHamVectorPtr)
    {
        delete HealingHam;
    }
    HealingHamVectorPtr.clear();
}

/**
 * @brief Инициализация игры.
 * 
 * Загружает игровую карту, персонажа, врагов и другие объекты.
 */
void AGameState::InitGame()
{
    // Вместимость вектора врагов
    constexpr int CapacityVectorEnemy = 40;

    // Вместимость вектора пуль
    constexpr int CapacityVectorBullet = 20;

    // Вместимость вектора ключей
    constexpr int CapacityVectorKey = 3;

    DialoguePtr->InitDialogue();
    GameOverPtr->InitGameOver(*SpriteManagerPtr);
    CreditsScreenPtr->InitCreditsScreen(*SpriteManagerPtr);
    ParticleSystemPtr->InitParticleSystem();
    PlayerPtr->InitPlayer(*SpriteManagerPtr);
    GameMapPtr->InitGameMap();
    SoundManagerPtr->InitSoundGame();
    // FpsManagerPtr->InitFpsText(); // TODO: используется для тестирования производительности
    GameTextPtr->InitGameText();
    MenuPtr->InitMenu();
    FinalGatePtr->InitFinalGate(*SpriteManagerPtr);

    // Резервирование места для врагов
    EnemyVectorPtr.reserve(CapacityVectorEnemy);

    // Для случайного появления врагов из 3 видов
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 2);

    for (const sf::Vector2f& EnemyPosition : GameMapPtr->GetSpawnBaseEnemyPositionVector())
    {
        int EnemyType = dist(gen);
        float MaxPatrolDistance = 80.f;
        AEnemy* BaseEnemy = nullptr;

        switch (EnemyType)
        {
        case 0:
            // Зеленый враг
            BaseEnemy = new ABaseGreenEnemy(MaxPatrolDistance, EnemyPosition);
            break;

        case 1:
            // Красный враг
            BaseEnemy = new ABaseRedEnemy(MaxPatrolDistance, EnemyPosition);
            break;

        case 2:
            // Синий враг
            BaseEnemy = new ABaseBlueEnemy(MaxPatrolDistance, EnemyPosition);
            break;

        default:
            BaseEnemy = nullptr;
            break;
        }

        if (BaseEnemy)
        {
            BaseEnemy->InitEnemy(*SpriteManagerPtr);
            EnemyVectorPtr.emplace_back(BaseEnemy);
        }
    }

    for (const sf::Vector2f& BossPosition : GameMapPtr->GetSpawnBossEnemyPositionVector())
    {
        float MaxPatrolDistance = 80.f;
        AEnemy* Boss = new ABossEnemy(MaxPatrolDistance, BossPosition); // Указать начальную позицию врага
        Boss->InitEnemy(*SpriteManagerPtr);
        EnemyVectorPtr.emplace_back(Boss);
    }

    // Резервирование места для пуль
    BulletsVectorPtr.reserve(CapacityVectorBullet);

    // Резервирование места для ключей
    KeysVectorPtr.reserve(CapacityVectorKey);

    // Указать начальную позицию сундуков
    for (const sf::Vector2f& ChestPosition : GameMapPtr->GetSpawnChestPositionVector())
    {
        AChestObject* Chest = new AChestObject(ChestPosition);
        Chest->InitChest(*SpriteManagerPtr);
        ChestVectorPtr.emplace_back(std::move(Chest));
        ++AChestObject::ChestCount;
    }

    // Указать начальную позицию лечащих объектов(ветчины)
    for (const sf::Vector2f& HealingPosition : GameMapPtr->GetSpawnHealingObjectsPositionVector())
    {
        AHealthObject* HealingHam = new AHealthObject(HealingPosition);
        HealingHam->InitHealthObject(*SpriteManagerPtr);
        HealingHamVectorPtr.emplace_back(std::move(HealingHam));
    }
}

/**
* @brief Начинает игру с начального состояния.
*/
void AGameState::StartGame()
{
    InitGame();
    SetGameState(EGameState::EGS_Menu);
}

/**
* @brief Сбрасывает состояние игры, например, для перезапуска.
*/
void AGameState::ResetGame()
{
    // Очищаем вектор врагов
    for (auto& Enemy : EnemyVectorPtr)
    {
        delete Enemy;
    }
    EnemyVectorPtr.clear();

    // Очищаем вектор пуль
    for (auto& Bullet : BulletsVectorPtr)
    {
        delete Bullet;
    }
    BulletsVectorPtr.clear();

    // Очищаем вектор ключей
    for (auto& FoundKey : FoundKeysVectorPtr)
    {
        FoundKey->ResetKey();
        delete FoundKey;
    }
    FoundKeysVectorPtr.clear();
    KeysVectorPtr.clear();

    // Очищаем векток сундуков
    for (auto& Chest : ChestVectorPtr)
    {
        Chest->ResetChestObject();
        delete Chest;
    }
    ChestVectorPtr.clear();

    for (auto& HealingHam : HealingHamVectorPtr)
    {
        HealingHam->ResetHealObjectState();
        delete HealingHam;
    }
    HealingHamVectorPtr.clear();

    // Сбрасываем состояние игрока
    if (PlayerPtr)
    {
        PlayerPtr->ResetPlayer();
    }

    if (GameMapPtr)
    {
        GameMapPtr->ResetGameMap();
    }

    if (GameOverPtr)
    {
        GameOverPtr->ResetGameOverPosition();
    }

    if (CreditsScreenPtr)
    {
        CreditsScreenPtr->ResetCreditsScreen();
    }

    // Инициализируем игру заново
    InitGame();
}

/**
* @brief Устанавливает флаг, показывающий, идет ли игровой процесс.
* 
* @param IsPlayState Состояние игрового процесса (true - идет, false - остановлен).
*/
void AGameState::SetIsPlayState(bool IsPlayState)
{
    bIsGamePlaying = IsPlayState;
}

/**
* @brief Обрабатывает взаимодействие игрока с кнопками (например, меню или пауза).
* 
* @param Event Событие SFML, описывающее взаимодействие (нажатие кнопки).
* @param Window Окно, в котором происходит игра.
*/
void AGameState::HandleButtonInteraction(const sf::Event& Event, sf::RenderWindow& Window)
{
    // Обработка нажатия клавиши Escape
    if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Escape)
    {
        if (CurrentGameState == EGameState::EGS_Playing ||
            CurrentGameState == EGameState::EGS_IntroDialogue ||
            CurrentGameState == EGameState::EGS_GameOver)
        {
            // Переход в паузу, сохранение текущего состояния
            SetGameState(EGameState::EGS_Pause);
            // std::cout << "State changed to: Pause" << std::endl;
        }
    }

    if (CurrentGameState != EGameState::EGS_Playing)
    {
        // Обновляем состояние кнопок в меню
        sf::Vector2i MousePosition = sf::Mouse::getPosition(Window);
        sf::Vector2f MouseWorldPosition = Window.mapPixelToCoords(MousePosition);
        MenuPtr->UpdateMenu(MouseWorldPosition, *SoundManagerPtr, *this, Event);

        // Если левая кнопка мыши нажата
        if (Event.type == sf::Event::MouseButtonPressed && Event.mouseButton.button == sf::Mouse::Left)
        {
            if (CurrentGameState == EGameState::EGS_GameOver)
            {
                float ElapsedTime = GameOverTimer.getElapsedTime().asSeconds();

                // Если после показала текста GameOver прошло 3.5 секунды, то перезапускаем игру и переходим в меню
                if (ElapsedTime >= 3.5f)
                {
                    GameOverTimer.restart();

                    ResetGame();
                    SetGameState(EGameState::EGS_Menu);
                    SetIsPlayState(false);
                }
            }
            else if (CurrentGameState == EGameState::EGS_MissionCompleted)
            {
                float ElapsedTime = MissionCompletedTimer.getElapsedTime().asSeconds();

                // Если после показала титров в конце игры прошло 15 секунды, то перезапускаем игру и переходим в меню
                if (ElapsedTime >= 15.f)
                {
                    MissionCompletedTimer.restart();

                    ResetGame();
                    SetGameState(EGameState::EGS_Menu);
                    SetIsPlayState(false);
                }
            }
        }
    }

    if (CurrentGameState == EGameState::EGS_IntroDialogue)
    {
        DialoguePtr->SwitchNextFrame(Event, *SoundManagerPtr);
    }

    // Закрываем окно, если пользователь нажал Exit
    if (CurrentGameState == EGameState::EGS_Exit)
    {
        Window.close();
        // std::cout << "State changed to: Exit" << std::endl;
    }
}

/**
* @brief Устанавливает новое состояние игры.
* 
* @param NewGameState Новое состояние игры.
*/
void AGameState::SetGameState(const EGameState& NewGameState)
{
    // Сохраняем предыдущее состояние игры
    PastGameState = CurrentGameState;

    switch (NewGameState)
    {
    // Если переключаемся в игровое состояние
    case EGameState::EGS_Playing:
        {
            // Останавливаем музыку меню, если она играет
            if (SoundManagerPtr->GetSound("MenuMusic").Playing || SoundManagerPtr->GetSound("Helicopter").Playing)
            {
                SoundManagerPtr->StopSound("MenuMusic");
                SoundManagerPtr->StopSound("Helicopter");
            }

            // Продолжаем воспроизведение фоновой музыки, если она уже играет
            if (SoundManagerPtr->GetSound("BackgroundMusic").getStatus() == sf::SoundSource::Paused)
            {
                SoundManagerPtr->PlaySound("BackgroundMusic"); // Продолжаем воспроизведение если на паузе
            }
            else if (SoundManagerPtr->GetSound("BackgroundMusic").getStatus() == sf::SoundSource::Stopped)
            {
                // Запускаем фоновую музыку если она не играла
                SoundManagerPtr->GetSound("BackgroundMusic").setLoop(true); // Запуск фоновой музыки на повторе
                SoundManagerPtr->PlaySound("BackgroundMusic");
            }
            break;
        }
    // Если переключаемся в меню
    case EGameState::EGS_Menu:
        {
            // Останавливаем фоновую музыку, если она играет
            if (SoundManagerPtr->GetSound("BackgroundMusic").Playing)
            {
                SoundManagerPtr->StopSound("BackgroundMusic");
            }

            // Воспроизводим музыку меню, если она ещё не играет
            if (SoundManagerPtr->GetSound("MenuMusic").getStatus() == sf::SoundSource::Stopped)
            {
                SoundManagerPtr->GetSound("MenuMusic").setLoop(true); // Запуск музыки меню на повторе
                SoundManagerPtr->PlaySound("MenuMusic");
            }
            break;
        }
    // Если переключаемся в опции или паузу, фоновая музыка продолжает играть
    case EGameState::EGS_IntroDialogue:
        {
            // Останавливаем музыку меню, если она играет
            if (SoundManagerPtr->GetSound("MenuMusic").Playing)
            {
                SoundManagerPtr->StopSound("MenuMusic");
            }

            // Запускаем звук Helicopter
            if (SoundManagerPtr->GetSound("Helicopter").getStatus() == sf::SoundSource::Paused)
            {
                SoundManagerPtr->PlaySound("Helicopter"); // Продолжаем воспроизведение если на паузе
            }
            else if (SoundManagerPtr->GetSound("Helicopter").getStatus() == sf::SoundSource::Stopped)
            {
                // Запускаем звук Helicopter, если он не играл
                SoundManagerPtr->GetSound("Helicopter").setLoop(true); // Запуск Helicopter на повторе
                SoundManagerPtr->PlaySound("Helicopter");
            }
            break;
        }
    case EGameState::EGS_GameOver:
        {
            // Запускаем таймер
            GameOverTimer.restart();
            break;
        }
    default:
        {
            break;
        }
    }

    // Изменяем состояние игры
    CurrentGameState = NewGameState;

    // Обновляем видимость меню в зависимости от состояния игры
    MenuPtr->UpdateVisibility(*this);
}

/**
 * @brief Обновление пользовательского ввода.
 * 
 * @param DeltaTime Время, прошедшее с последнего обновления.
 */
void AGameState::UpdateInput(float DeltaTime)
{
    if (CurrentGameState == EGameState::EGS_Playing || CurrentGameState == EGameState::EGS_EscapeRoute)
    {
        // Проверяем нажатие клавиши и обновляем состояние персонажа
        PlayerPtr->HandlePlayerMove(DeltaTime);

        // Стрельба при нажатии левой кнопки мыши
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            // Задержка между выстрелами для персонажа
            float ElapsedSecondsForPlayer = PlayerPtr->GetDelayShotTimerHandle().getElapsedTime().asSeconds();
            if (ElapsedSecondsForPlayer > 0.4f)
            {
                PlayerPtr->HandlePlayerShoots(BulletsVectorPtr,
                                              *SoundManagerPtr,
                                              *SpriteManagerPtr,
                                              *ParticleSystemPtr);

                // Сброс таймера задержки между выстрелами
                PlayerPtr->GetDelayShotTimerHandle().restart();
            }
        }

        // Задержка между выстрелами для врагов
        for (auto& Enemy : EnemyVectorPtr)
        {
            float ElapsedSecondsForEnemy = Enemy->GetDelayShotTimerHandle().getElapsedTime().asSeconds();
            if (ElapsedSecondsForEnemy >= Enemy->GetShootingDelay())
            {
                // Воспроизводим выстрел
                Enemy->EnemyShoot(9.f, BulletsVectorPtr, *SpriteManagerPtr);

                // Сброс таймера задержки между выстрелами
                Enemy->GetDelayShotTimerHandle().restart();
            }
        }
    }
}

/**
 * @brief Обновление игрового процесса.
 * 
 * Этот метод обновляет состояние игрового процесса, включая обновление движений персонажей, врагов, пуль и других объектов.
 * Также проверяются столкновения, обновляются текстуры и анимации.
 * 
 * @param DeltaTime Время, прошедшее с последнего обновления.
 */
void AGameState::UpdateGameplay(float DeltaTime)
{
    switch (CurrentGameState)
    {
    case EGameState::EGS_Playing:
    case EGameState::EGS_GameOver:
    case EGameState::EGS_EscapeRoute:
        {
            // Если персонаж умер, то устанавливаем состояние конец игры
            if (PlayerPtr->GetIsDeathPlayer() && CurrentGameState != EGameState::EGS_GameOver)
            {
                SetGameState(EGameState::EGS_GameOver);
            }

            // Обновляем состояние текста конца игры
            GameOverPtr->UpdateGameOver(DeltaTime);

            // После пересечения ворот фоновая музыка начинает затихать, а после увеличивается звук вертолёта
            if (CurrentGameState == EGameState::EGS_EscapeRoute)
            {
                float ElapsedTime = EscapeTimer.getElapsedTime().asSeconds();
                if (ElapsedTime >= 0.4f)
                {
                    SoundManagerPtr->SmoothTransition();
                    EscapeTimer.restart();
                }
            }

            // Обновление движения персонажа(спрайта)
            PlayerPtr->UpdatePlayerMove(DeltaTime, *SoundManagerPtr);

            // Обновление движения персонажа (если он не мертв)
            if (!PlayerPtr->GetIsDeathPlayer())
            {
                CollisionManagerPtr->HandlePawnCollisionWithGameMap(
                    PlayerPtr->GetActorCollisionRect(),
                    PlayerPtr->GetActorVelocity(),
                    PlayerPtr->GetPawnCanJump(),
                    PlayerPtr->GetIsOnLadder(),
                    SoundManagerPtr
                );
            }

            // Обновление движения врагов
            for (auto Enemy : EnemyVectorPtr)
            {
                Enemy->UpdateEnemyMove(DeltaTime, *PlayerPtr, *GameMapPtr, *CollisionManagerPtr);
            }

            // Обновление движения пуль
            for (auto Bullet : BulletsVectorPtr)
            {
                Bullet->UpdateBulletPosition(DeltaTime);
            }

            // Проверка столкновений пуль
            CollisionManagerPtr->CheckAllBulletCollisions(
                BulletsVectorPtr,
                EnemyVectorPtr,
                KeysVectorPtr,
                *PlayerPtr,
                *SpriteManagerPtr,
                *SoundManagerPtr,
                *ParticleSystemPtr
            );

            // Проверка столкновения персонажа с ключами
            CollisionManagerPtr->CheckCollisionWithKey(KeysVectorPtr, *SoundManagerPtr);

            // TODO: используется для тестирования производительности
            // Обновление отображения FPS
            // FpsManagerPtr->UpdateFpsText(DeltaTime);

            // Обновление отображения текста миссий(заданий)
            GameTextPtr->UpdateMissionText();

            // Обновления состояния эффектов
            ParticleSystemPtr->UpdateParticleSystem(DeltaTime);

            // Если все сундуки открыты, то устанавливаем текстуру открытых ворот
            if (AChestObject::ChestOpenCountStatic == OPEN_ALL_CHESTS)
            {
                FinalGatePtr->UpdateStateFinalGate();
                SoundManagerPtr->PlaySound("OpenGate");

                // Увеличиваем счётчик, чтобы не попадать в условие снова
                ++AChestObject::ChestOpenCountStatic;
            }

            // Обновления гравитации и состояния ключа
            for (auto& Key : KeysVectorPtr)
            {
                Key->UpdateMoveKey(DeltaTime);
                CollisionManagerPtr->CheckCollisionWithGameMap(
                    Key->GetActorCollisionRect(),
                    Key->GetActorVelocity(),
                    nullptr
                );
            }

            // Обновление анимации сундуков, когда персонаж с ними пересекается 
            for (const auto& Chest : ChestVectorPtr)
            {
                // Проверка на взаимодействие персонажа с сундуком
                if (CollisionManagerPtr->CheckCollision(PlayerPtr->GetActorCollisionRect(),
                                                        Chest->GetActorCollisionRect()))
                {
                    for (auto it = FoundKeysVectorPtr.begin(); it != FoundKeysVectorPtr.end();)
                    {
                        AKeyObject* Key = *it;
                        if (!Chest->GetOpenChest() && Chest->CanOpenWithKey(Key->GetKeyID()))
                        {
                            SoundManagerPtr->PlaySound("OpenChest");
                            Chest->OpenChest(); // Открываем сундук
                            Chest->IncreaseOpenChestCounter(); // Увеличение счетчика открытых сундуков

                            delete Key; // Удаляем объект ключа
                            it = FoundKeysVectorPtr.erase(it); // Удаляем элемент из вектора и получаем новый итератор

                            break; // Прерываем цикл после открытия сундука и удаления ключа
                        }
                        else
                        {
                            ++it; // Переход к следующему элементу, если текущий ключ не подошел
                        }
                    }
                }
                // Обновляем анимацию сундука, когда его открыли
                Chest->ChestUpdateAnimation(DeltaTime);
            }

            // Проверяем столкновение с ветчиной, которая попалняет здоровье
            for (const auto& HealingHam : HealingHamVectorPtr)
            {
                if (CollisionManagerPtr->CheckCollision(HealingHam->GetActorCollisionRect(),
                                                        PlayerPtr->GetActorCollisionRect()))
                {
                    HealingHam->SetIsEatenHeal(true);
                    PlayerPtr->AddPawnHealth(PLAYER_MAX_HEALTH);
                    SoundManagerPtr->PlaySound("EatenHeal");
                }
            }

            break;
        }
    case EGameState::EGS_IntroDialogue:
        {
            // Обновления диалогов старта игры
            DialoguePtr->UpdateDialogue(DeltaTime);

            if (DialoguePtr->isDialogueFinished())
            {
                SetGameState(EGameState::EGS_Playing);
                // std::cout << "State changed to: Playing" << std::endl;
            }
            break;
        }
    case EGameState::EGS_MissionCompleted:
        {
            CreditsScreenPtr->UpdateCreditsScreen(MissionCompletedTimer);
            break;
        }
    default:
        {
            break;
        }
    }
}

/**
 * @brief Обновление камеры игры.
 * 
 * Этот метод обновляет позицию и размер камеры в зависимости от состояния игры и положения игрока.
 * Устанавливает положение камеры для разных состояний игры, таких как меню, диалог и игровой процесс.
 * 
 * @param Window Окно игры для установки камеры.
 */
void AGameState::UpdateCamera(sf::RenderWindow& Window)
{
    // Фокусировка камеры на игроке
    ViewPlayer = Window.getView();
    ViewPlayer.setSize(Window.getDefaultView().getSize() * ZOOM_FACTOR);

    switch (CurrentGameState)
    {
    case EGameState::EGS_Menu:
        {
            ViewPlayer.setCenter(PlayerPtr->GetActorPosition());

            // Картинка для меню
            MenuPtr->SetMenuPosition(sf::Vector2f(ViewPlayer.getCenter().x - (ViewPlayer.getSize().x / 2.f),
                                                  ViewPlayer.getCenter().y - (ViewPlayer.getSize().y / 2.f)));
            // Кнопка Play
            MenuPtr->SetButtonPosition("Play", sf::Vector2f(ViewPlayer.getCenter().x - 25.f,
                                                            ViewPlayer.getCenter().y - (ViewPlayer.getSize().y / 2.f)
                                                            + 40.f));
            // Кнопка Exit
            MenuPtr->SetButtonPosition("Exit", sf::Vector2f(ViewPlayer.getCenter().x - 25.f,
                                                            ViewPlayer.getCenter().y - (ViewPlayer.getSize().y / 2.f)
                                                            + 80.f));
            // Кнопка Options
            MenuPtr->SetButtonPosition("Options", sf::Vector2f(ViewPlayer.getCenter().x - 25.f,
                                                               ViewPlayer.getCenter().y - (ViewPlayer.getSize().y / 2.f)
                                                               + 60.f));

            break;
        }
    case EGameState::EGS_IntroDialogue:
        {
            // Камера фокусируется на фиксированной позиции диалога
            const sf::Vector2f FixedDialoguePosition(0.f, -500.f);
            ViewPlayer.setCenter(FixedDialoguePosition);

            DialoguePtr->SetDialoguePosition(sf::Vector2f(
                FixedDialoguePosition.x - (ViewPlayer.getSize().x / 2.f) + 25.f,
                FixedDialoguePosition.y - (ViewPlayer.getSize().y / 2.f)));

            break;
        }
    case EGameState::EGS_Playing:
    case EGameState::EGS_EscapeRoute:
        {
            // Размеры карты
            sf::Vector2f MapSize(6680.f, 1760.f); // Ширина и высота карты
            sf::Vector2f PlayerPosition = PlayerPtr->GetActorPosition();

            // Проверяем границы карты и устанавливаем центр камеры в пределах карты
            sf::Vector2f ViewSize = ViewPlayer.getSize();
            sf::Vector2f NewCenter = PlayerPosition;

            // Лимит по оси X
            if (PlayerPosition.x - ViewSize.x / 2.f < 0)
            {
                NewCenter.x = ViewSize.x / 2.f;
            }
            else if (PlayerPosition.x + ViewSize.x / 2.f > MapSize.x)
            {
                NewCenter.x = MapSize.x - ViewSize.x / 2.f;
            }
            // Лимит по оси Y
            if (PlayerPosition.y - ViewSize.y / 2.f < 0)
            {
                NewCenter.y = ViewSize.y / 2.f;
            }
            else if (PlayerPosition.y + ViewSize.y / 2.f > MapSize.y)
            {
                NewCenter.y = MapSize.y - ViewSize.y / 2.f;
            }

            ViewPlayer.setCenter(NewCenter);

            if (!PlayerPtr->GetIsDeathPlayer())
            {
                // Обновление позиции шкалы здоровья
                PlayerPtr->UpdatePawnHealthBar(PlayerPtr->GetPawnCurrentHealth(), PlayerPtr->GetPawnMaxHealth(),
                                               sf::Vector2f( // Установка самой шкалы здоровья
                                                   ViewPlayer.getCenter().x - (ViewPlayer.getSize().x / 2.f) + 34.f,
                                                   ViewPlayer.getCenter().y - (ViewPlayer.getSize().y / 2.f) + 7.f),
                                               sf::Vector2f( // Установка спрайта(в нашем случае спрайт сердца) 
                                                   ViewPlayer.getCenter().x - (ViewPlayer.getSize().x / 2.f) + 5,
                                                   ViewPlayer.getCenter().y - (ViewPlayer.getSize().y / 2.f) + 3.f));

                // TODO: используется для тестирования производительности
                // FpsManagerPtr->SetPositionFpsText(sf::Vector2f(
                //     ViewPlayer.getCenter().x - (ViewPlayer.getSize().x / 2.f) + 10.f,
                //     ViewPlayer.getCenter().y - (ViewPlayer.getSize().y / 2.f) + 10.f));

                // Установка позиций GameText
                GameTextPtr->SetPositionGameText(sf::Vector2f(
                    ViewPlayer.getCenter().x - (ViewPlayer.getSize().x / 2.f) + 195.f,
                    ViewPlayer.getCenter().y - (ViewPlayer.getSize().y / 2.f) - 2.f));

                for (auto& Key : FoundKeysVectorPtr)
                {
                    Key->MakrKeyInTopRightCorner(ViewPlayer);
                }
            }
            break;
        }
    case EGameState::EGS_GameOver:
        {
            if (PlayerPtr->GetIsDeathPlayer())
            {
                GameOverPtr->SetGameOverPosition(sf::Vector2f(ViewPlayer.getCenter().x,
                                                              ViewPlayer.getCenter().y -
                                                              ViewPlayer.getSize().y / 2.f - 60.f));
            }
            break;
        }
    case EGameState::EGS_MissionCompleted:
        {
            CreditsScreenPtr->SetCreditsScreenPosition(sf::Vector2f(
                ViewPlayer.getCenter().x - (ViewPlayer.getSize().x / 2.f),
                ViewPlayer.getCenter().y - (ViewPlayer.getSize().y / 2.f)));
            break;
        }
    case EGameState::EGS_Pause:
        {
            // Отображаем картинку диалога, если был активен EGS_IntroDialogue
            if (PastGameState == EGameState::EGS_IntroDialogue)
            {
                const sf::Vector2f FixedDialoguePosition(0.f, -500.f);

                // Камера фокусируется на фиксированной позиции диалога
                ViewPlayer.setCenter(FixedDialoguePosition);

                DialoguePtr->SetDialoguePosition(sf::Vector2f(
                    FixedDialoguePosition.x - (ViewPlayer.getSize().x / 2.f) + 25.f,
                    FixedDialoguePosition.y - (ViewPlayer.getSize().y / 2.f)));
            }

            // Блюр картинка во время паузы
            MenuPtr->SetBlurScreenPosition(sf::Vector2f(ViewPlayer.getCenter().x - (ViewPlayer.getSize().x / 2.f),
                                                        ViewPlayer.getCenter().y - (ViewPlayer.getSize().y / 2.f)));
            // Кнопка Continue
            MenuPtr->SetButtonPosition("Continue",
                                       sf::Vector2f(ViewPlayer.getCenter().x - 31.f,
                                                    ViewPlayer.getCenter().y -
                                                    (ViewPlayer.getSize().y / 2.f) + 40.f));
            // Кнопка Options
            MenuPtr->SetButtonPosition("Options",
                                       sf::Vector2f(ViewPlayer.getCenter().x - 23.f,
                                                    ViewPlayer.getCenter().y -
                                                    (ViewPlayer.getSize().y / 2.f) + 60.f));
            // Кнопка Menu
            MenuPtr->SetButtonPosition("Menu",
                                       sf::Vector2f(ViewPlayer.getCenter().x - 23.f,
                                                    ViewPlayer.getCenter().y -
                                                    (ViewPlayer.getSize().y / 2.f) + 80.f));

            break;
        }
    case EGameState::EGS_Options:
        {
            // Кнопка Cross (image X)
            MenuPtr->SetButtonPosition("Cross", sf::Vector2f(ViewPlayer.getCenter().x + 109.f,
                                                             ViewPlayer.getCenter().y - 70.f));
            // Устанавливаем позиции для всех ползунков звука
            MenuPtr->SetVolumeSliderDefaultPositions(sf::Vector2f(ViewPlayer.getCenter().x - 15.f,
                                                                  ViewPlayer.getCenter().y - 50.f), 20.f);
        }
    default:
        {
            break;
        }
    }

    if (!PlayerPtr->GetIsDeathPlayer())
    {
        Window.setView(ViewPlayer);
    }
}

/**
 * @brief Отрисовка игровых объектов.
 * 
 * Этот метод отвечает за отрисовку всех игровых объектов на экране, включая карту, персонажа, врагов, пули, ключи, и другие элементы.
 * Также обрабатывает отрисовку элементов пользовательского интерфейса, таких как меню и текст.
 * 
 * @param Window Окно игры для отрисовки объектов.
 */
void AGameState::DrawGame(sf::RenderWindow& Window) const
{
    // Отрисовка карты, объектов и персонажей - общий для всех состояний код
    GameMapPtr->DrawGameMap(Window);

    ParticleSystemPtr->DrawParticleSystem(Window);

    for (const auto& Chest : ChestVectorPtr)
    {
        Chest->DrawActor(Window);
    }

    FinalGatePtr->DrawActor(Window);
    PlayerPtr->DrawActor(Window);

    for (auto& Enemy : EnemyVectorPtr)
    {
        Enemy->DrawActor(Window);
    }

    for (auto& Bullet : BulletsVectorPtr)
    {
        Bullet->DrawActor(Window);
    }

    // TODO: используется для тестирования производительности
    // FpsManagerPtr->DrawFpsText(Window);

    GameTextPtr->DrawGameText(Window);

    for (auto& Key : KeysVectorPtr)
    {
        Key->DrawActor(Window);
    }

    for (auto& Key : FoundKeysVectorPtr)
    {
        Key->DrawActor(Window);
    }
    for (const auto& HealingHam : HealingHamVectorPtr)
    {
        HealingHam->DrawActor(Window);
    }

    if (CurrentGameState != EGameState::EGS_Playing)
    {
        // Проверка состояния игры
        if (CurrentGameState == EGameState::EGS_Options ||
            CurrentGameState == EGameState::EGS_Pause ||
            CurrentGameState == EGameState::EGS_IntroDialogue ||
            CurrentGameState == EGameState::EGS_GameOver)
        {
            DialoguePtr->DrawActor(Window);
            GameOverPtr->DrawActor(Window);
        }

        if (CurrentGameState == EGameState::EGS_MissionCompleted)
        {
            CreditsScreenPtr->DrawActor(Window);
        }

        // Отрисовка меню паузы
        MenuPtr->DrawMenu(Window, *this);

        if (CurrentGameState == EGameState::EGS_Options)
        {
            // Отрисовка шкалы громкости для настроек
            for (auto& Slider : MenuPtr->GetVolumeSliderMap())
            {
                Slider.first->DrawSlider(Window);
            }
        }
    }
}

/**
* @brief Получает текущее состояние игры (игровой процесс активен или нет).
* 
* @return true, если игра идет, иначе false.
*/
bool AGameState::GetIsPlayState() const
{
    return bIsGamePlaying;
}

/**
* @brief Получает текущее состояние игры.
* 
* @return Текущее состояние игры.
*/
EGameState AGameState::GetCurrentGameState() const
{
    return CurrentGameState;
}

/**
* @brief Получает предыдущее состояние игры.
* 
* @return Предыдущее состояние игры.
*/
EGameState AGameState::GetPastGameState() const
{
    return PastGameState;
}

/**
* @brief Возвращает таймер состояния Game Over.
* 
* @return Таймер для состояния Game Over.
*/
sf::Clock AGameState::GetGameOverTimer() const
{
    return GameOverTimer;
}

/**
* @brief Возвращает текущий вид (камеру) игрока.
* 
* @return Вид игрока (sf::View).
*/
sf::View AGameState::GetViewPlayer() const
{
    return ViewPlayer;
}

/**
 * @brief Получение вектора найденных ключей.
 * 
 * Этот метод возвращает ссылку на вектор найденных ключей.
 * Позволяет модифицировать оригинальный вектор, содержащий найденные ключи.
 * 
 * @return Возвращает ссылку на вектор найденных ключей.
 */
std::vector<AKeyObject*>& AGameState::GetFoundKeysVector()
{
    return FoundKeysVectorPtr;
}
