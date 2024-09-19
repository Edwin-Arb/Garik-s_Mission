#pragma once
#include "../Constants.h"

/**
 * @brief Управляет анимацией объектов в игре.
 * 
 * Класс AAnimationManager управляет последовательным отображением кадров анимации 
 * на основе заданной текстуры и скорости смены кадров.
 */
class AAnimationManager
{
public:
    /**
     * @brief Конструктор по умолчанию.
     * 
     * Инициализирует скорость смены кадров и текущий индекс кадра.
     */
    AAnimationManager();

    /**
     * @brief Деструктор по умолчанию.
     * 
     * Освобождает ресурсы, необходимые для управления анимацией.
     */
    ~AAnimationManager();

    /**
     * @brief Обновляет текущий кадр анимации на основе прошедшего времени.
     * 
     * @param DeltaTime Время, прошедшее с последнего обновления кадра.
     */
    void AnimationUpdate(float DeltaTime);

    /**
     * @brief Устанавливает текущий кадр анимации по индексу.
     * 
     * @param Frame Индекс кадра анимации.
     */
    void SetCurrentFrame(const int Frame);

    /**
     * @brief Устанавливает флаг для остановки на последнем кадре.
     * 
     * @param bStop Флаг, указывающий, нужно ли остановить анимацию на последнем кадре.
     */
    void SetStopAtLastFrame(bool bStop);

    /**
     * @brief Сбрасывает состояние анимации.
     */
    void ResetAnimation();

    /**
     * @brief Проверяет, завершена ли анимация.
     *
     * @return Возвращает состояние анимацие, true - значить анимация завершена.
     */
    bool IsAnimationFinished() const;
 
    /**
     * @brief Получает текущий индекс кадра анимации.
     * 
     * @return int Текущий индекс кадра анимации.
     */
    int GetCurrentFrameIndex() const;

    /**
     * @brief Получает прямоугольник текущего кадра анимации на текстуре.
     * 
     * @return sf::IntRect Прямоугольник текущего кадра анимации.
     */
    sf::IntRect GetCurrentFrame() const;

private:
    bool bIsAnimationFinished;           // Флаг завершения анимации
    bool bStopAtLastFrame;               // Флаг для остановки анимации на последнем кадре

public:
    float FrameSpeed;                    // Скорость смены кадров анимации.

private:
   float CurrentFrameIndex;              // Текущий индекс кадра анимации в векторе FrameRect.

public:
    std::vector<sf::IntRect> FrameRect;  // Вектор прямоугольников кадров анимации.
    sf::Texture AnimTexture;             // Текстура анимации.
};
