#include "Button.h"


/**
* @brief Конструктор для класса AButton.
*/
AButton::AButton()
    : bIsPressed(false),
      bIsVisible(true),
      bAnimationPlaying(false),
      AnimationDuration(sf::seconds(0.15f))

{
}

/**
* @brief Инициализирует кнопку с заданными текстурами и кадрами.
*
* @param TexturePath Путь к текстуре кнопки.
* @param DefaultFrame Прямоугольник для нормального состояния кнопки.
* @param HoverFrame Прямоугольник для состояния наведения на кнопку.
*/
void AButton::InitButton(const std::string& TexturePath,
                         const sf::IntRect& DefaultFrame,
                         const sf::IntRect& HoverFrame)
{
    if (!ButtonAnimation.FrameRect.empty())
    {
        ButtonAnimation.ResetAnimation();
    }
    
    if (!ButtonAnimation.AnimTexture.loadFromFile(TexturePath))
    {
        throw std::runtime_error("Error: Failed to load texture: " + ASSETS_PATH + TexturePath);
    }
    
    ButtonAnimation.FrameRect.emplace_back(DefaultFrame);
    ButtonAnimation.FrameRect.emplace_back(HoverFrame);

    ButtonSprite.setTexture(ButtonAnimation.AnimTexture);
    ButtonSprite.setTextureRect(ButtonAnimation.GetCurrentFrame());
}

/**
* @brief Устанавливает состояние кнопки (нажата или нет).
*
* @param bPressed Флаг, указывающий, нажата ли кнопка.
*/
void AButton::SetPressedState(bool bPressed)
{
    bIsPressed = bPressed;
}

/**
* @brief Устанавливает видимость кнопки.
*
* @param bVisible Флаг, указывающий, видима ли кнопка.
*/
void AButton::SetVisibility(bool bVisible)
{
    bIsVisible = bVisible;
}

/**
* @brief Устанавливает позицию кнопки на экране.
*
* @param NewPosition Новая позиция кнопки.
*/
void AButton::SetButtonPosition(const sf::Vector2f& NewPosition)
{
    ButtonSprite.setPosition(NewPosition);
}

/**
* @brief Обновляет состояние кнопки на основе позиции мыши, события и звукового менеджера.
*
* @param MousePosition Позиция мыши на экране.
* @param SoundManager Менеджер звуков для воспроизведения звуков при взаимодействии с кнопкой.
* @param Event Событие, связанное с нажатием кнопки.
*/
void AButton::UpdateButton(const sf::Vector2f& MousePosition,
                           ASoundManager& SoundManager,
                           const sf::Event& Event)
{
    sf::FloatRect ButtonBounds = ButtonSprite.getGlobalBounds();

    // Проверяем, наведена ли мышь на кнопку
    if (ButtonBounds.contains(MousePosition))
    {
        // Если левая кнопка мыши нажата
        if (Event.type == sf::Event::MouseButtonPressed && Event.mouseButton.button == sf::Mouse::Left)
        {
            // Если кнопка ещё не была нажата
            if (!bIsPressed)
            {
                SoundManager.PlaySound("ClickButton");
                ButtonAnimation.SetCurrentFrame(1); // Устанавливаем 1 кадр на время нажатия
                bIsPressed = true; // Устанавливаем флаг, что кнопка нажата

                // Запускаем таймер для анимации
                AnimationClock.restart();
                bAnimationPlaying = true;
            }
        }
    }

    // Обработка сброса состояния кнопки
    if (bAnimationPlaying && AnimationClock.getElapsedTime() >= AnimationDuration)
    {
        ButtonAnimation.SetCurrentFrame(0); // Сбрасываем кадр на 0 после завершения анимации
        bAnimationPlaying = false;
    }

    ButtonSprite.setTextureRect(ButtonAnimation.GetCurrentFrame());
}

/**
* @brief Отрисовывает кнопку на экране.
*
* @param Window Окно, в котором происходит отрисовка кнопки.
*/
void AButton::DrawButton(sf::RenderWindow& Window) const
{
    Window.draw(ButtonSprite);
}

/**
* @brief Проверяет, нажата ли кнопка.
* 
* @return true, если кнопка нажата, иначе false.
*/
bool AButton::GetIsPressedButton() const
{
    return bIsPressed;
}

/**
* @brief Проверяет, видима ли кнопка.
* 
* @return true, если кнопка видима, иначе false.
*/
bool AButton::IsVisible() const
{
    return bIsVisible;
}

/**
* @brief Получает состояние анимации кнопки.
* 
* @return true, если анимация воспроизводится, иначе false.
*/
bool AButton::GetAnimationState() const
{
    return bAnimationPlaying;
}
