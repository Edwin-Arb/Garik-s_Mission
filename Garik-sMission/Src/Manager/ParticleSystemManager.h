#pragma once
#include <unordered_map>
#include "AnimationManager.h"

/**
 * @brief Структура для хранения информации о частицах.
 * 
 * Хранит анимацию, позицию и масштаб эффекта частиц.
 */
struct ParticleEffect
{
    AAnimationManager Animation;    // Менеджер анимации для эффекта частиц.
    sf::Vector2f Position;          // Позиция эффекта частиц на экране.
    sf::Vector2f Scale;             // Масштаб эффекта частиц.
};

/**
 * @brief Менеджер системы частиц.
 * 
 * Класс AParticleSystemManager управляет эффектами частиц, включая их добавление, воспроизведение,
 * обновление и отрисовку.
 */
class AParticleSystemManager
{
public:
    /**
     * @brief Конструктор класса AParticleSystemManager.
     */
    AParticleSystemManager() = default;

    /**
     * @brief Деструктор класса AParticleSystemManager.
     * Нужен для очистки памяти.
     */
    ~AParticleSystemManager();

    /**
     * @brief Инициализирует систему частиц.
     * 
     * Этот метод должен быть вызван перед использованием системы частиц.
     */
    void InitParticleSystem();

    /**
     * @brief Добавляет новый эффект частиц в систему.
     * 
     * @param EffectName Название эффекта частиц.
     * @param TexturePath Путь к текстуре эффекта частиц.
     * @param FrameSpeed Скорость смены кадров анимации.
     * @param Frames Вектор прямоугольников, определяющих кадры анимации.
     */
    void AddParticleEffect(const std::string& EffectName, const std::string& TexturePath, float FrameSpeed,
                           const std::vector<sf::IntRect>& Frames);

    /**
     * @brief Запускает воспроизведение эффекта частиц.
     * 
     * @param EffectName Название эффекта частиц для воспроизведения.
     */
    void PlayEffect(const std::string& EffectName);

    /**
     * @brief Запускает воспроизведение эффекта частиц с заданной позицией и параметрами.
     * 
     * @param EffectName Название эффекта частиц для воспроизведения.
     * @param ObjectPosition Позиция объекта, на который нужно отобразить эффект частиц.
     * @param Offset Смещение эффекта частиц относительно позиции объекта.
     * @param EffectScale Масштаб эффекта частиц.
     * @param bIsMoveRight Флаг, указывающий, движется ли эффект частиц вправо.
     */
    void PlayEffectWithPosition(const std::string& EffectName,
                                const sf::Vector2f& ObjectPosition,
                                const sf::Vector2f& Offset = {0.f, 0.f},
                                const sf::Vector2f& EffectScale = {1.f, 1.f},
                                bool bIsMoveRight = true);

    /**
     * @brief Обновляет систему частиц.
     * 
     * @param DeltaTime Время, прошедшее с последнего обновления.
     */
    void UpdateParticleSystem(float DeltaTime);

    /**
     * @brief Отрисовывает эффекты частиц на экране.
     * 
     * @param Window Окно, на котором будут отрисованы эффекты частиц.
     */
    void DrawParticleSystem(sf::RenderWindow& Window);

    /**
     * @brief Проверяет, завершен ли эффект частиц.
     * 
     * @param EffectName Название эффекта частиц для проверки.
     * @return true, если эффект завершен, иначе false.
     */
    bool IsEffectFinished(const std::string& EffectName) const;

private:
    std::unordered_map<std::string, ParticleEffect> ParticleEffectsMap; // Хранение анимаций по именам.
};
