#pragma once
#include "../Constants.h"
#include "ChestObject.h"
#include "../Manager/ASoundManager.h"

class AFinalGateObject : public AActor
{
public:
    /**
     * @brief Конструктор для создания финальных ворот с указанием начальной позиции.
     * 
     * @param StartPosition Начальная позиция финальных ворот.
     */
    explicit AFinalGateObject(sf::Vector2f& StartPosition);

    /**
     * @brief Деструктор по умолчанию для объекта финальных ворот.
     */
    ~AFinalGateObject() override = default;

    /**
     * @brief Инициализация финальных ворот.
     * 
     * Загружает необходимые текстуры и спрайты для отображения ворот.
     * 
     * @param SpriteManager Менеджер спрайтов для загрузки текстур.
     */
    void InitFinalGate(ASpriteManager& SpriteManager);

    /**
     * @brief Обновление состояния финальных ворот.
     * 
     * Обрабатывает изменения состояния ворот (например, открытие/закрытие).
     */
    void UpdateStateFinalGate();

    /**
     * @brief Отрисовка финальных ворот.
     * 
     * @param Window Окно, в котором будут отображаться финальные ворота.
     */
    void DrawActor(sf::RenderWindow& Window) override;
};
