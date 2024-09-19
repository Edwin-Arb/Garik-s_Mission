#pragma once
#include "../Abstract/AActor.h"


class AGameState;

class AGameOver : public AActor
{
public:
    /**
     * @brief Конструктор класса AGameOver.
     * 
     * Инициализирует необходимые ресурсы для экрана "Game Over".
     */
    AGameOver();

    /**
     * @brief Деструктор класса AGameOver.
     * 
     * Используется деструктор по умолчанию.
     */
    ~AGameOver() override = default;

    /**
     * @brief Инициализирует экран "Game Over".
     * 
     * Загружает текстуры и спрайты для экрана "Game Over".
     * 
     * @param SpriteManager Менеджер спрайтов для загрузки текстур.
     */
    void InitGameOver(ASpriteManager& SpriteManager);

    /**
     * @brief Обновляет состояние экрана "Game Over".
     * 
     * Управляет логикой и обновляет позицию спрайтов.
     * 
     * @param DeltaTime Время, прошедшее между кадрами.
     */
    void UpdateGameOver(float DeltaTime);

    /**
     * @brief Устанавливает новую позицию для экрана "Game Over".
     * 
     * @param NewPosition Новая позиция экрана "Game Over".
     */
    void SetGameOverPosition(const sf::Vector2f& NewPosition);

    /**
     * @brief Сбрасывает позицию экрана "Game Over".
     * 
     * Возвращает экран в его исходную позицию.
     */
    void ResetGameOverPosition();

    /**
     * @brief Отрисовывает экран "Game Over".
     * 
     * @param Window Окно, в котором происходит отрисовка.
     */
    void DrawActor(sf::RenderWindow& Window) override;

private:
    sf::Texture GameOverTexture;    // Текстура для экрана "Game Over"
    sf::Sprite GameOverSprite;      // Спрайт для отображения текста "Game Over"
    sf::Vector2f TargetPosition;    // Целевая позиция экрана "Game Over"
};
