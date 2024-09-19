#include "APawn.h"

/**
 * @brief Конструктор класса APawn.
 * Инициализирует флаги и параметры здоровья, скорости, а также создает объекты для шкалы здоровья и её текстуры.
 */
APawn::APawn()
    : bIsPawnJump(false),
      PawnSpeed(0.f),
      PawnMaxHealth(0.f),
      PawnCurrentHealth(0.f),
      PawnHealthBarTexturePtr(std::make_unique<sf::Texture>()),
      PawnHealthBarPtr(std::make_unique<AHealthBar>())
{
}

/**
 * @brief Инициализирует шкалу здоровья пешки с текстурой.
 * 
 * @param TexturePath Путь к текстуре шкалы здоровья.
 * @param Size Размер шкалы здоровья.
 * @param Scale Масштаб шкалы здоровья.
 * @param FillColor Цвет заполнения шкалы здоровья.
 * @param BackgroundColor Цвет фона шкалы здоровья.
 * @param SpriteManager Менеджер спрайтов для обработки спрайтов объекта.
 */
void APawn::InitPawnHealthBar(const std::string& TexturePath, const sf::Vector2f& Size, const sf::Vector2f& Scale,
                              const sf::Color& FillColor, const sf::Color& BackgroundColor,
                              ASpriteManager& SpriteManager)
{
    // Загрузка текстуры шкалы здоровья
    if (!PawnHealthBarTexturePtr->loadFromFile(TexturePath))
    {
        throw std::runtime_error("Error: Failed to load texture: " + ASSETS_PATH + TexturePath);
    }
    
    // Инициализация шкалы здоровья
    PawnHealthBarPtr->InitHealthBar(Size, FillColor, BackgroundColor, SpriteManager);
    HealthBarSprite.setTexture(*PawnHealthBarTexturePtr);
    HealthBarSprite.setScale(Scale);
}

/**
 * @brief Инициализирует шкалу здоровья пешки без текстуры.
 * 
 * @param Size Размер шкалы здоровья.
 * @param FillColor Цвет заполнения шкалы здоровья.
 * @param BackgroundColor Цвет фона шкалы здоровья.
 * @param SpriteManager Менеджер спрайтов для обработки спрайтов объекта.
 */
void APawn::InitPawnHealthBar(const sf::Vector2f& Size,
                              const sf::Color& FillColor, const sf::Color& BackgroundColor,
                              ASpriteManager& SpriteManager)
{
    // Инициализация шкалы здоровья
    PawnHealthBarPtr->InitHealthBar(Size, FillColor, BackgroundColor, SpriteManager);
    HealthBarSprite.setTexture(*PawnHealthBarTexturePtr);
}

/**
 * @brief Получить по ссылке данные о состоянии прыжка пешки.
 * 
 * @return Текущее состояние прыжка пешки.
 */
bool& APawn::GetPawnCanJump()
{
    return bIsPawnJump;
}

/**
 * @brief Обновляет шкалу здоровья пешки с указанием позиции для отрисовки.
 * 
 * @param CurrentHealth Текущее здоровье.
 * @param MaxHealth Максимальное здоровье.
 * @param HealthBarPosition Позиция шкалы здоровья.
 * @param DrawHealthBarPosition Позиция для отрисовки шкалы здоровья.
 */
void APawn::UpdatePawnHealthBar(float CurrentHealth, float MaxHealth,
                                const sf::Vector2f& HealthBarPosition,
                                const sf::Vector2f& DrawHealthBarPosition)
{
    // Обновление значений шкалы здоровья
    if (PawnCurrentHealth >= DEATH)
    {
        PawnHealthBarPtr->UpdateHealthBar(CurrentHealth, MaxHealth);
    }

    PawnHealthBarPtr->SetHealthBarPosition(HealthBarPosition);
    HealthBarSprite.setPosition(DrawHealthBarPosition);
}

/**
 * @brief Обновляет шкалу здоровья пешки без изменения позиции для отрисовки.
 * 
 * @param CurrentHealth Текущее здоровье.
 * @param MaxHealth Максимальное здоровье.
 * @param HealthBarPosition Позиция шкалы здоровья.
 */
void APawn::UpdatePawnHealthBar(float CurrentHealth, const float MaxHealth,
                                const sf::Vector2f& HealthBarPosition) const
{
    // Обновление значений шкалы здоровья
    PawnHealthBarPtr->UpdateHealthBar(CurrentHealth, MaxHealth);
    PawnHealthBarPtr->SetHealthBarPosition(HealthBarPosition);
}

/**
 * @brief Отнять текущее здоровье пешки. Используется для изменения шкалы здоровья.
 * 
 * @param TakeAwayCurrentHealth Отнять текущее здоровье пешки.
 */
void APawn::TakeAwayPawnCurrentHealth(float TakeAwayCurrentHealth)
{
    PawnCurrentHealth -= TakeAwayCurrentHealth;
}

/**
* @brief Добавить здоровья пешки.
*
* @param AddHealth Сколько добавить здоровье пешки.
*/
void APawn::AddPawnHealth(float AddHealth)
{
    // Сначала увеличиваем здоровье
    PawnCurrentHealth += AddHealth;

    // Ограничиваем здоровье в пределах от 0 до максимума
    PawnCurrentHealth = std::clamp(PawnCurrentHealth, 0.0f, PawnMaxHealth);
}

/**
 * @brief Получение текущего здоровья пешки.
 * 
 * @return Текущее здоровье пешки.
 */
float APawn::GetPawnCurrentHealth() const
{
    return PawnCurrentHealth;
}

/**
 * @brief Получение максимального здоровья пешки.
 * 
 * @return Максимальное здоровье пешки.
 */
float APawn::GetPawnMaxHealth() const
{
    return PawnMaxHealth;
}

/**
* @brief Таймер для задержки между выстрелами
* 
* @return Возвращает таймер задежки выстрела
*/
sf::Clock& APawn::GetDelayShotTimerHandle()
{
    return DelayShotTimerHandle;
}
