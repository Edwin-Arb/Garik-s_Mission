#include "CreditsScreen.h"

/**
* @brief Конструктор класса ACreditsScreen.
* 
* Инициализирует переменные для экрана с титрами.
*/
ACreditsScreen::ACreditsScreen()
    : bIsDrawMouse(false),
      bIsBackgroundCompletely(false),
      MaxInterpolation(255.f),
      AddInterpolationValue(0.3f),
      InterpolationBackground(0)
{
    // Инициализация текстуры и спрайта для чёрного экрана
    BlackBackgroundTexture.create(SCREEN_WIDTH, SCREEN_HEIGHT); // Устанавливаем размер в соответствии с размером окна

    BlackBackgroundTexture.clear(sf::Color(0, 0, 0, 0)); // Черный цвет
    BlackBackgroundSprite.setTexture(BlackBackgroundTexture.getTexture());
}

/**
* @brief Инициализирует экран с титрами.
* 
* Загружает спрайты и текстуры для отображения титров.
* 
* @param SpriteManager Менеджер спрайтов, используется для загрузки текстур и спрайтов.
*/
void ACreditsScreen::InitCreditsScreen(ASpriteManager& SpriteManager)
{
    const std::string CreditsTexturePath = ASSETS_PATH + "MainTiles/EndCredits.png";
    const sf::IntRect CreditsRect = {0, 0, 1280, 720};

    AActor::InitActorTexture(CreditsTexturePath, CreditsRect, {1.f, 1.f}, {0.5f, 0.5f}, SpriteManager);
    ActorSprite.setScale(0.2f, 0.2f);

    if (!MouseTexture.loadFromFile(ASSETS_PATH + "MainTiles/Mouse.png"))
    {
        throw std::runtime_error("Error: Failed to load texture: " + ASSETS_PATH + "MainTiles/Mouse.png");
    }

    MouseSprite.setTexture(MouseTexture);
}

/**
 * @brief Устанавливает новую позицию для экрана с титрами.
 * 
 * Перемещает экран с титрами в указанное место.
 * 
 * @param NewPosition Новая позиция экрана с титрами.
 */
void ACreditsScreen::SetCreditsScreenPosition(const sf::Vector2f& NewPosition)
{
    BlackBackgroundSprite.setPosition(NewPosition);
    ActorSprite.setPosition(NewPosition + (sf::Vector2f(130.f, 70.f)));
    MouseSprite.setPosition(NewPosition + (sf::Vector2f(230.f, 110.f)));
}

/**
* @brief Обновляет состояние экрана с титрами.
* 
* Плавно изменяет фон и отслеживает время показа титров.
* 
* @param ClockTimer Время, прошедшее с начала отображения титров.
*/
void ACreditsScreen::UpdateCreditsScreen(const sf::Clock& ClockTimer)
{
    if (InterpolationBackground <= MaxInterpolation)
    {
        BlackBackgroundTexture.clear(sf::Color(0, 0, 0, static_cast<uint8_t>(InterpolationBackground)));
        InterpolationBackground += AddInterpolationValue;
    }
    else
    {
        bIsBackgroundCompletely = true;
    }

    // Если прошло нужное время, то можем рисовать спрайт мыши
    if (ClockTimer.getElapsedTime().asSeconds() >= 15.f)
    {
        bIsDrawMouse = true;
    }
}

/**
* @brief Сбрасывает экран с титрами.
* 
* Возвращает экран с титрами в начальное состояние для повторного использования.
*/
void ACreditsScreen::ResetCreditsScreen()
{
    bIsDrawMouse = false;
    bIsBackgroundCompletely = false;
    InterpolationBackground = 0;
}

/**
* @brief Отрисовывает объекты на экране.
* 
* Рисует спрайты титров и фона на экране.
* 
* @param Window Окно, в которое происходит отрисовка объектов.
*/
void ACreditsScreen::DrawActor(sf::RenderWindow& Window)
{
    if (!bIsBackgroundCompletely)
    {
        // Сначала отрисовываем задний фон(чёрный экран)
        Window.draw(BlackBackgroundSprite);
    }
    else
    {
        // Сначала отрисовываем задний фон(чёрный экран)
        Window.draw(BlackBackgroundSprite);

        // Затем текст с титрами
        Window.draw(ActorSprite);

        // Если прошло нужное время, то можем рисовать спрайт мыши
        if (bIsDrawMouse)
        {
            Window.draw(MouseSprite);
        }
    }
}
