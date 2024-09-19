#pragma once
#include "../Constants.h"
#include <unordered_map>

/**
 * @brief Управляет звуковыми эффектами в игре.
 * 
 * Класс ASoundManager отвечает за загрузку, воспроизведение и управление звуками, 
 * включая плавные переходы громкости и установку уровня громкости.
 */
class ASoundManager
{
public:
    /**
     * @brief Конструктор по умолчанию.
     * 
     * Инициализирует менеджер звуков.
     */
    ASoundManager();

    /**
     * @brief Деструктор по умолчанию.
     * 
     * Освобождает ресурсы, связанные со звуками.
     */
    ~ASoundManager();

    /**
     * @brief Инициализирует звуки для игры.
     * 
     * Загружает звуковые файлы и подготавливает их к воспроизведению.
     */
    void InitSoundGame();

private:
    /**
     * @brief Загружает звук и добавляет его в контейнер.
     * 
     * @param SoundName Имя звука.
     * @param FilePath Путь к файлу со звуком.
     */
    void LoadSound(const std::string& SoundName, const std::string& FilePath);

public:
    /**
     * @brief Устанавливает уровень громкости для всех звуков.
     * 
     * @param Volume Уровень громкости (от 0.0 до 50.0).
     */
    void SetMasterVolume(float Volume);

    /**
     * @brief Плавный переход между звуками.
     * 
     * Выполняет плавный переход громкости для текущих звуков.
     */
    void SmoothTransition();

    /**
     * @brief Получает ссылку на звук по его имени.
     * 
     * @param SoundName Имя звука.
     * 
     * @return sf::Sound& Ссылка на объект звука.
     */
    sf::Sound& GetSound(const std::string& SoundName);

    /**
     * @brief Устанавливает уровень громкости для конкретного звука.
     * 
     * @param SoundName Имя звука.
     * @param Volume Уровень громкости (от 0.0 до 50.0).
     * 
     * @return sf::Sound const& Константная ссылка на объект звука.
     */
    sf::Sound const& SetVolume(const std::string& SoundName, float Volume);

    /**
     * @brief Воспроизводит звук по его имени.
     * 
     * @param SoundName Имя звука.
     * 
     * @return sf::Sound const& Константная ссылка на объект звука.
     */
    sf::Sound const& PlaySound(const std::string& SoundName);

    /**
     * @brief Останавливает воспроизведение звука по его имени.
     * 
     * @param SoundName Имя звука.
     * 
     * @return sf::Sound const& Константная ссылка на объект звука.
     */
    sf::Sound const& StopSound(const std::string& SoundName);

private:
    bool isHelicopterStarted;                                       // Флаг, указывающий, начался ли звук вертолета
    float MasterVolumeLevel;                                        // Уровень громкости для всех звуков
    float PreviousMasterVolume;                                     // Предыдущий уровень громкости
    float BackgroundVolume;                                         // Уровень громкости фоновой музыки
    float HelicopterVolume;                                         // Уровень громкости звука вертолета
 
    std::unordered_map<sf::Sound*, float> IndividualVolumeLevel;    // Уровень громкости для каждого звука
    std::unordered_map<std::string, sf::SoundBuffer> SoundBuffers;  // Буферы звуков
    std::unordered_map<std::string, sf::Sound> Sounds;              // Звуковые объекты
 
    sf::Clock TransitionClock;                                      // Часы для отслеживания времени перехода громкости
};
