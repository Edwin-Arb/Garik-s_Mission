#include "Menu.h"
#include <iostream>
#include <ranges>
#include "../../GameMain/GameState.h"


/**
* @brief Конструктор для класса AMenu.
*/
AMenu::AMenu()
{
    constexpr int CapacityVolumeSlider = 5;
    constexpr float Margin = 8.f; // Указываем отступ в 8 пикселей
    static sf::Texture BackgroundTexture;
    static sf::Texture KnobTexture;

    BackgroundTexture.loadFromFile(ASSETS_PATH + "MainTiles/SoundScale.png");
    KnobTexture.loadFromFile(ASSETS_PATH + "MainTiles/SoundScale.png");

    // Ползунок общего уровня звука
    ASlider* MasterVolumeSlider = new ASlider(0.f, 50.f, BackgroundTexture, KnobTexture, Margin);
    MasterVolumeSlider->SetValue(50.0f); // Устанавливаем начальное значение громкости на 100%

    // Добавляем ползунок, который отвечает за общий звук в игре
    VolumeSliderMap[MasterVolumeSlider] = {"MasterVolume"};
    SliderListVector.emplace_back(MasterVolumeSlider);

    for (int i = 1; i < CapacityVolumeSlider; ++i) // Start from 1 since 0 is the MasterVolumeSlider
    {
        ASlider* VolumeSlider = new ASlider(0.f, 50.f, BackgroundTexture, KnobTexture, Margin);

        // Настройка стандартного значения громкости (например 0 до 50)
        VolumeSlider->SetValue(15.0f);

        // Присваиваем каждому ползунку соответствующий звук
        switch (i)
        {
        case 1:
            {
                // Звук интерфейса
                VolumeSliderMap[VolumeSlider] = {"ClickButton"};
                break;
            }
        case 2:
            {
                // Звуки голосов персонажей
                VolumeSliderMap[VolumeSlider] = {"PlayerDamage", "PlayerDeath", "EnemyDeath", "EatenHeal"};
                break;
            }
        case 3:
            {
                // Звуки окружения и объектов
                VolumeSliderMap[VolumeSlider] =
                {
                    "KeyFound", "OpenChest", "Lava", "Peaks", "PlayerShot",
                    "DialogueDevice", "OffDialogueDevice", "Helicopter", "SMSDialogue",
                    "OpenGate", "BulletHit", "RocketExplosion"
                };
                break;
            }
        case 4:
            {
                // Звуки фоновой музыки
                VolumeSliderMap[VolumeSlider] = {"BackgroundMusic", "MenuMusic"};
                break;
            }
        default:
            {
                break;
            }
        }

        SliderListVector.emplace_back(VolumeSlider);
    }

    // Инициализация текстуры и спрайта для затемнения(блюр эффекта)
    PauseBlurTexture.create(SCREEN_WIDTH, SCREEN_HEIGHT); // Устанавливаем размер в соответствии с размером окна
    PauseBlurTexture.clear(sf::Color(0, 0, 0, 200)); // Полупрозрачный черный цвет
    PauseBlurSprite.setTexture(PauseBlurTexture.getTexture());
    PauseBlurSprite.setPosition(1000.f, 0.f);
}

/**
* @brief Деструктор класса AMenu.
* Особождаем память.
*/
AMenu::~AMenu()
{
    // Освобождаем память для всех ползунков
    for (const auto& Slider : SliderListVector)
    {
        delete Slider;
    }
    SliderListVector.clear();
    ButtonMap.clear();
    SoundLabelsMap.clear();

    // Очищаем Map, не удаляя объекты снова
    VolumeSliderMap.clear();
}

/**
* @brief Инициализирует меню.
*/
void AMenu::InitMenu()
{
    // Размер кнопок
    sf::Vector2i ButtonSize = {50, 19};
    sf::Vector2i LabelSize = {10, 9};

    // Название для каждого ползунка звука
    if (!SoundLabelsTexture.loadFromFile(ASSETS_PATH + "MainTiles/SoundLabels.png"))
    {
        throw std::runtime_error("Error: Failed to load texture: " + ASSETS_PATH + "MainTiles/SoundLabels.png");
    }
    
    SoundLabelsMap["Master"].setTexture(SoundLabelsTexture);
    SoundLabelsMap["Master"].setTextureRect(sf::IntRect(0, 0, 29, LabelSize.y));

    SoundLabelsMap["Interface"].setTexture(SoundLabelsTexture);
    SoundLabelsMap["Interface"].setTextureRect(sf::IntRect(0, LabelSize.x, 39, LabelSize.y));

    SoundLabelsMap["Voices"].setTexture(SoundLabelsTexture);
    SoundLabelsMap["Voices"].setTextureRect(sf::IntRect(0, LabelSize.x * 2, 25, LabelSize.y));

    SoundLabelsMap["Ambience"].setTexture(SoundLabelsTexture);
    SoundLabelsMap["Ambience"].setTextureRect(sf::IntRect(0, LabelSize.x * 3, 35, LabelSize.y));

    SoundLabelsMap["Music"].setTexture(SoundLabelsTexture);
    SoundLabelsMap["Music"].setTextureRect(sf::IntRect(0, LabelSize.x * 4, 25, LabelSize.y));

    // Загрузочный экран для меню
    if (!MenuTexture.loadFromFile(ASSETS_PATH + "MainTiles/Menu.png"))
    {
        throw std::runtime_error("Error: Failed to load texture: " + ASSETS_PATH + "MainTiles/Menu.png");
    }
    MenuSprite.setTexture(MenuTexture);
    MenuSprite.setScale(0.1336f, 0.111f);

    // Кнопка Play
    PlayButton.InitButton(ASSETS_PATH + "MainTiles/Buttons.png",
                          {0, 0, ButtonSize.x, ButtonSize.y},
                          {ButtonSize.x, 0, ButtonSize.x, ButtonSize.y});
    ButtonMap["Play"] = PlayButton;

    // Кнопка Options
    Options.InitButton(ASSETS_PATH + "MainTiles/Buttons.png",
                       {0, ButtonSize.y + 1, ButtonSize.x, ButtonSize.y},
                       {ButtonSize.x, ButtonSize.y + 1, ButtonSize.x, ButtonSize.y});
    ButtonMap["Options"] = Options;

    // Кнопка Exit
    ExitButton.InitButton(ASSETS_PATH + "MainTiles/Buttons.png",
                          {0, 40, ButtonSize.x, ButtonSize.y},
                          {ButtonSize.x, 40, ButtonSize.x, ButtonSize.y});
    ButtonMap["Exit"] = ExitButton;

    // Кнопка Continue
    ContinueButton.InitButton(ASSETS_PATH + "MainTiles/Buttons.png",
                              {0, 60, 66, ButtonSize.y},
                              {0, 79, 66, ButtonSize.y});
    ButtonMap["Continue"] = ContinueButton;

    // Кнопка Крестика(Cross), чтобы вернуться в обратно
    Cross.InitButton(ASSETS_PATH + "MainTiles/Buttons.png", {74, 60, 17, 19}, {74, 79, 17, 19});
    ButtonMap["Cross"] = Cross;

    // Кнопка Menu
    MenuButton.InitButton(ASSETS_PATH + "MainTiles/Buttons.png", {0, 99, ButtonSize.x, ButtonSize.y},
                          {ButtonSize.x, 100, ButtonSize.x, ButtonSize.y});
    ButtonMap["Menu"] = MenuButton;
}

/**
* @brief Устанавливает позицию меню на экране.
*
* @param NewPosition Новая позиция меню.
*/
void AMenu::SetMenuPosition(const sf::Vector2f& NewPosition)
{
    MenuSprite.setPosition(NewPosition);
}

/**
* @brief Устанавливает позицию размытого экрана.
*
* @param NewPosition Новая позиция размытого экрана.
*/
void AMenu::SetBlurScreenPosition(const sf::Vector2f& NewPosition)
{
    PauseBlurSprite.setPosition(NewPosition);
}

/**
* @brief Устанавливает начальные позиции ползунков громкости.
*
* @param StartPosition Начальная позиция ползунков.
* @param VerticalOffset Вертикальное смещение ползунков.
*/
void AMenu::SetVolumeSliderDefaultPositions(const sf::Vector2f& StartPosition, float VerticalOffset)
{
    // Смещение для ползунков звука
    float OffsetY = 0.f;

    for (const auto& Slider : SliderListVector)
    {
        // Рассчитываем новую позицию для ползунка
        sf::Vector2f NewPosition = StartPosition;
        NewPosition.y += OffsetY;

        // Добавляем позицию в карту
        Slider->SetSliderPosition(NewPosition);

        // Смещение для следующего ползунка
        OffsetY += VerticalOffset;
    }

    // Смещение для названий звуков
    OffsetY = 3.f;

    for (auto& Label : SoundLabelsMap | std::views::values)
    {
        float OffsetX = 40.f;

        // Рассчитываем новую позицию для названия звука
        sf::Vector2f NewPosition = StartPosition;
        NewPosition.x -= OffsetX;
        NewPosition.y += OffsetY;

        // Добавляем позицию в карту
        Label.setPosition(NewPosition);

        // Смещение для следующего названия
        OffsetY += VerticalOffset;
    }
}

/**
* @brief Устанавливает позицию кнопки по её имени.
*
* @param ButtonName Имя кнопки.
* @param NewPosition Новая позиция кнопки.
*/
void AMenu::SetButtonPosition(const std::string& ButtonName, const sf::Vector2f& NewPosition)
{
    if (ButtonMap.contains(ButtonName))
    {
        ButtonMap[ButtonName].SetButtonPosition(NewPosition);
    }
    else
    {
        // Логирование или обработка ситуации, когда кнопка с таким именем не найден
        throw std::runtime_error("Button with name " + ButtonName + " not found!");
    }
}

/**
* @brief Обрабатывает нажатие кнопки.
*
* @param ButtonName Имя кнопки, на которую нажали.
* @param GameState Состояние игры для возможного обновления.
*/
void AMenu::HandleButtonPress(const std::string& ButtonName, AGameState& GameState) const
{
    if (ButtonName == "Play")
    {
        GameState.SetGameState(EGameState::EGS_IntroDialogue);
        GameState.SetIsPlayState(true);
        std::cout << "State changed to: IntroDialogue" << std::endl;
    }
    else if (ButtonName == "Options")
    {
        GameState.SetGameState(EGameState::EGS_Options);
        std::cout << "State changed to: Options" << std::endl;
    }
    else if (ButtonName == "Continue")
    {
        if (GameState.GetCurrentGameState() == EGameState::EGS_Pause)
        {
            GameState.SetGameState(EGameState::EGS_IntroDialogue);
            std::cout << "State changed to: IntroDialogue" << std::endl;
        }
        else if (GameState.GetPastGameState() == EGameState::EGS_Playing)
        {
            GameState.SetGameState(EGameState::EGS_Playing);
            std::cout << "State changed to: Playing" << std::endl;
        }
    }
    else if (ButtonName == "Exit")
    {
        GameState.SetGameState(EGameState::EGS_Exit);
        std::cout << "State changed to: Exit" << std::endl;
    }
    else if (ButtonName == "Cross")
    {
        if (GameState.GetPastGameState() == EGameState::EGS_Pause)
        {
            GameState.SetGameState(EGameState::EGS_Pause);
            std::cout << "State changed to: Pause" << std::endl;
        }
        else if (GameState.GetPastGameState() == EGameState::EGS_Menu)
        {
            GameState.SetGameState(EGameState::EGS_Menu);
            std::cout << "State changed to: Menu" << std::endl;
        }
    }
    else if (ButtonName == "Menu")
    {
        GameState.ResetGame();
        GameState.SetGameState(EGameState::EGS_Menu);
        GameState.SetIsPlayState(false);
        std::cout << "State changed to: Menu" << std::endl;
    }
    else
    {
        std::cout << "Unknown button: " << ButtonName << std::endl;
    }
}

/**
* @brief Обновляет видимость элементов меню в зависимости от состояния игры.
*
* @param GameState Состояние игры для определения видимости элементов.
*/
void AMenu::UpdateVisibility(const class AGameState& GameState)
{
    // Скрываем все кнопки
    for (auto& Button : ButtonMap | std::views::values)
    {
        Button.SetVisibility(false);
    }

    // Включаем видимость нужных кнопок в зависимости от состояния
    switch (GameState.GetCurrentGameState())
    {
    case EGameState::EGS_Menu:
        {
            ButtonMap["Play"].SetVisibility(true);
            ButtonMap["Options"].SetVisibility(true);
            ButtonMap["Exit"].SetVisibility(true);
            break;
        }
    case EGameState::EGS_Pause:
        {
            ButtonMap["Continue"].SetVisibility(true);
            ButtonMap["Options"].SetVisibility(true);
            ButtonMap["Menu"].SetVisibility(true);
            break;
        }
    case EGameState::EGS_Options:
        {
            ButtonMap["Cross"].SetVisibility(true);
            break;
        }
    default:
        {
            break;
        }
    }
}

/**
* @brief Обновляет состояние меню на основе позиции мыши, звукового менеджера и события.
*
* @param MousePosition Позиция мыши на экране.
* @param SoundManager Менеджер звуков для обработки звуковых эффектов.
* @param GameState Состояние игры для возможного обновления.
* @param Event Событие, связанное с взаимодействием с меню.
*/
void AMenu::UpdateMenu(const sf::Vector2f& MousePosition,
                       ASoundManager& SoundManager,
                       AGameState& GameState,
                       const sf::Event& Event)
{
    // Обновляем видимость кнопок в зависимости от текущего состояния игры
    UpdateVisibility(GameState);

    for (auto& Button : ButtonMap)
    {
        // Если кнопка невидима, пропускаем её
        if (!Button.second.IsVisible())
        {
            continue;
        }

        // Обновляем состояние кнопки
        Button.second.UpdateButton(MousePosition, SoundManager, Event);

        // Проверяем, нажата ли кнопка
        if (Button.second.GetIsPressedButton())
        {
            // Делаем переход, но после визуального отклика
            if (!Button.second.GetAnimationState())
            {
                HandleButtonPress(Button.first, GameState);
                Button.second.SetPressedState(false); // Сброс состояния нажатия после обработки
            }
        }
    }

    // Обновляет ползунки громкости
    UpdateVolumeSlider(MousePosition, SoundManager, Event);
}

/**
* @brief Обновляет ползунки громкости на основе позиции мыши и события.
*
* @param MousePosition Позиция мыши на экране.
* @param SoundManager Менеджер звуков для обработки звуковых эффектов.
* @param Event Событие, связанное с взаимодействием с ползунками.
*/
void AMenu::UpdateVolumeSlider(const sf::Vector2f& MousePosition,
                               ASoundManager& SoundManager,
                               const sf::Event& Event) const
{
    for (auto& SliderPair : VolumeSliderMap)
    {
        ASlider* Slider = SliderPair.first;
        const std::vector<std::string>& SoundKeys = SliderPair.second;

        Slider->HandleEvent(Event, MousePosition);
        float Volume = Slider->GetValue();

        // Если это ползунок общего уровня звука
        if (SoundKeys[0] == "MasterVolume")
        {
            SoundManager.SetMasterVolume(Volume);
        }
        else
        {
            for (const auto& SoundKey : SoundKeys)
            {
                SoundManager.SetVolume(SoundKey, Volume); // Применяем громкость к каждому звуку
            }
        }
    }
}

/**
* @brief Отрисовывает меню на экране.
*
* @param Window Окно, в котором происходит отрисовка меню.
* @param GameState Состояние игры для определения отображаемых элементов.
*/
void AMenu::DrawMenu(sf::RenderWindow& Window, const AGameState& GameState) const
{
    if (GameState.GetCurrentGameState() == EGameState::EGS_Menu || !GameState.GetIsPlayState())
    {
        // Рисуем картинку для меню
        Window.draw(MenuSprite);
    }

    if (GameState.GetCurrentGameState() == EGameState::EGS_Pause ||
        GameState.GetCurrentGameState() == EGameState::EGS_Options)
    {
        // Рисуем текстуру затемнения (размытия)
        Window.draw(PauseBlurSprite);
    }

    // Отрисовка каждой видимой кнопки из ButtonMap
    for (auto& Button : ButtonMap | std::views::values)
    {
        if (Button.IsVisible())
        {
            Button.DrawButton(Window);
        }
    }

    // Отрисовка каждого названия звука для ползунков звука
    if (GameState.GetCurrentGameState() == EGameState::EGS_Options)
    {
        for (auto& Label : SoundLabelsMap | std::views::values)
        {
            Window.draw(Label);
        }
    }
}

/**
* @brief Возвращает карту кнопок меню.
*
* @return Неизменяемая карта кнопок, где ключи — имена кнопок, а значения — объекты кнопок.
*/
std::unordered_map<std::string, AButton> AMenu::GetButtonMap() const
{
    return ButtonMap;
}

/**
* @brief Возвращает карту ползунков громкости и соответствующих ключей звуков.
*
* @return Неизменяемая карта, где ключи — указатели на ползунки, а значения — векторы строк с ключами звуков.
*/
std::unordered_map<ASlider*, std::vector<std::string>> AMenu::GetVolumeSliderMap()
{
    return VolumeSliderMap;
}
