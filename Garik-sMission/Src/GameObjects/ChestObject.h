#pragma once
#include "KeyObject.h"
#include "../Manager/AnimationManager.h"

// Предварительное объявление перечисления EKeyType
enum class EKeyType;

enum class EChestType
{
    ECT_Red,
    ECT_Yellow,
    ECT_Green,
    ECT_Non
};

class AChestObject : public AActor
{
public:
    /**
     * @brief Конструктор по умолчанию для сундука.
     */
    AChestObject() = default;

    /**
     * @brief Конструктор для сундука с указанием начальной позиции.
     * 
     * @param StartPositionChest Начальная позиция сундука.
     */
    AChestObject(const sf::Vector2f& StartPositionChest);

    /**
     * @brief Деструктор по умолчанию для сундука.
     */
    ~AChestObject() override = default;

    /**
     * @brief Инициализация сундука.
     * 
     * Загружает анимации и текстуры сундука.
     * 
     * @param SpriteManager Менеджер спрайтов для загрузки текстур.
     */
    void InitChest(ASpriteManager& SpriteManager);

    /**
     * @brief Обновляет анимацию сундука.
     * 
     * @param DeltaTime Время, прошедшее с последнего кадра.
     */
    void ChestUpdateAnimation(float DeltaTime);

    /**
     * @brief Открывает сундук.
     */
    void OpenChest();

    /**
     * @brief Увеличивает счетчик открытых сундуков.
     */
    void IncreaseOpenChestCounter();

    /**
     * @brief Сбрасывает состояние сундука.
     */
    void ResetChestObject();

    /**
     * @brief Отрисовывает сундук.
     * 
     * @param Window Окно, в котором будет отображаться сундук.
     */
    void DrawActor(sf::RenderWindow& Window) override;
 
    /**
     * @brief Проверка, открыт ли сундук.
     * 
     * @return true, если сундук открыт, иначе false.
     */
    bool GetOpenChest() const;
   
    /**
     * @brief Проверяет, можно ли открыть сундук с помощью данного ключа.
     * 
     * @param KeyID Идентификатор ключа, которым пытаются открыть сундук.
     * 
     * @return true, если ключ подходит для сундука, иначе false.
     */
    bool CanOpenWithKey(EKeyType KeyID) const;

private:
    bool bIsChestOpen;                             // Флаг, указывающий, открыт ли сундук

public:
    static int ChestCount;                         // Общее количество сундуков в игре
    static int ChestOpenCountStatic;               // Количество открытых сундуков

private:
    EChestType ChestID;                            // Идентификатор типа сундука

    AAnimationManager RedChestOpenAnimation;       // Анимация открытия красного сундука
    AAnimationManager GreenChestOpenAnimation;     // Анимация открытия зеленого сундука
    AAnimationManager YellowChestOpenAnimation;    // Анимация открытия желтого сундука
    
    AAnimationManager* CurrentChestAnimation;      // Указатель на текущую анимацию сундука
};
