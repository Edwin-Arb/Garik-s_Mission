#include "IntroDialogue.h"
#include "GameState.h"

/**
* @brief Конструктор класса AIntroDialogue.
* 
* Инициализирует начальные параметры для вступительного диалога.
*/
AIntroDialogue::AIntroDialogue()
    : bDialogueFinished(false)
{
    ActorSize = {1356.f, 841.f};
}

/**
* @brief Инициализация диалога.
* 
* Устанавливает начальные параметры и состояние анимации диалога.
*/
void AIntroDialogue::InitDialogue()
{
    if (!DialogueAnimation.FrameRect.empty())
    {
        bDialogueFinished = false;
        DialogueAnimation.ResetAnimation();
    }

    // Загружаем текстуру картинки
    if (!DialogueAnimation.AnimTexture.loadFromFile(ASSETS_PATH + "MainTiles/IntroDialogue.png"))
    {
        throw std::runtime_error("Error: Failed to load texture: " + ASSETS_PATH + "MainTiles/IntroDialogue.png");
    }

    ActorSprite.setTexture(DialogueAnimation.AnimTexture);

    // Добавляем картинки в вектор
    //DialogueAnimation.FrameSpeed = 0.2f;
    DialogueAnimation.FrameRect.emplace_back(sf::IntRect(0, -2, static_cast<int>(ActorSize.x),
                                                         static_cast<int>(ActorSize.y)));

    DialogueAnimation.FrameRect.emplace_back(sf::IntRect(0, static_cast<int>(ActorSize.y),
                                                         static_cast<int>(ActorSize.x), static_cast<int>(ActorSize.y)));

    DialogueAnimation.FrameRect.emplace_back(sf::IntRect(0, static_cast<int>(ActorSize.y * 2),
                                                         static_cast<int>(ActorSize.x), static_cast<int>(ActorSize.y)));

    DialogueAnimation.FrameRect.emplace_back(sf::IntRect(0, static_cast<int>(ActorSize.y * 3),
                                                         static_cast<int>(ActorSize.x), static_cast<int>(ActorSize.y)));

    DialogueAnimation.FrameRect.emplace_back(sf::IntRect(0, static_cast<int>(ActorSize.y * 4),
                                                         static_cast<int>(ActorSize.x), static_cast<int>(ActorSize.y)));

    DialogueAnimation.FrameRect.emplace_back(sf::IntRect(0, static_cast<int>(ActorSize.y * 5),
                                                         static_cast<int>(ActorSize.x), static_cast<int>(ActorSize.y)));

    DialogueAnimation.SetStopAtLastFrame(true);

    ActorSprite.setScale(0.18f, 0.172f);
    ActorSprite.setPosition(1000.f, 0.f);
    
    if (!MouseTexture.loadFromFile(ASSETS_PATH + "MainTiles/Mouse.png"))
    {
        throw std::runtime_error("Error: Failed to load texture: " + ASSETS_PATH + "MainTiles/Mouse.png");
    }
    MouseSprite.setTexture(MouseTexture);
}

/**
* @brief Проверяет завершен ли диалог.
* 
* @return Возвращает true, если диалог завершен, иначе false.
*/
bool AIntroDialogue::isDialogueFinished() const
{
    return bDialogueFinished;
}

/**
* @brief Обновление состояния диалога.
* 
* Обновляет текущий кадр анимации и обработку логики диалога.
* 
* @param DeltaTime Время, прошедшее между кадрами.
*/
void AIntroDialogue::UpdateDialogue(float DeltaTime)
{
    ActorSprite.setTextureRect(DialogueAnimation.GetCurrentFrame());
    DialogueAnimation.AnimationUpdate(DeltaTime);
}

/**
* @brief Переключает кадр анимации диалога.
* 
* Обрабатывает событие ввода и управляет переключением кадров диалога.
* 
* @param Event Событие ввода, содержащее информацию о действиях пользователя.
* @param SoundManager Менеджер звука для управления звуковыми эффектами.
*/
void AIntroDialogue::SwitchNextFrame(const sf::Event& Event, ASoundManager& SoundManager)
{
    float ElapsedSeconds = DelayBetweenMessages.getElapsedTime().asSeconds();

    // Переходим к следующему кадру, если это не последний кадр
    if (DialogueAnimation.GetCurrentFrameIndex() < DialogueAnimation.FrameRect.size() - 1)
    {
        if (Event.type == sf::Event::MouseButtonPressed && Event.mouseButton.button == sf::Mouse::Left)
        {
            if (DialogueAnimation.GetCurrentFrameIndex() < 4)
            {
                SoundManager.PlaySound("SMSDialogue");
            }
            else
            {
                // Если последний кадр, то считаем диалог завершенным и проигрывает звук выключения устройства
                SoundManager.PlaySound("OffDialogueDevice");
            }

            // Переключаем на следующий кадр диалога
            DialogueAnimation.SetCurrentFrame(DialogueAnimation.GetCurrentFrameIndex() + 1);

            // Сбрасываем и запускаем таймер, который отсчитывает 2 секунды
            DelayBetweenMessages.restart();
        }
    }
    else
    {
        if (ElapsedSeconds >= 2.f)
        {
            // Если последний кадр, то считаем диалог завершенным
            bDialogueFinished = true;
        }
    }
}

/**
* @brief Устанавливает новую позицию для диалога.
* 
* Перемещает анимацию диалога в указанную позицию.
* 
* @param NewPosition Новая позиция диалога.
*/
void AIntroDialogue::SetDialoguePosition(const sf::Vector2f& NewPosition)
{
    ActorSprite.setPosition(NewPosition);
    MouseSprite.setPosition(NewPosition + sf::Vector2f(207.f, 114.f));
}

/**
* @brief Отрисовывает диалог.
* 
* Рендерит анимацию и элементы диалога на экране.
* 
* @param Window Окно, в котором происходит отрисовка.
*/
void AIntroDialogue::DrawActor(sf::RenderWindow& Window)
{
    Window.draw(ActorSprite);
    Window.draw(MouseSprite);
}

/**
* @brief Получает анимацию диалога.
* 
* @return Возвращает объект анимации диалога.
*/
AAnimationManager AIntroDialogue::GetDialogueAnimation() const
{
    return DialogueAnimation;
}