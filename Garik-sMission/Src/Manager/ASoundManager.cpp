#include "ASoundManager.h"
#include <ranges>


/**
* @brief Конструктор по умолчанию.
* 
* Инициализирует менеджер звуков.
*/
ASoundManager::ASoundManager()
    : isHelicopterStarted(false),
      MasterVolumeLevel(100.f),
      PreviousMasterVolume(0.f),
      BackgroundVolume(0.f),
      HelicopterVolume(0.f)
{
}

ASoundManager::~ASoundManager()
{
    IndividualVolumeLevel.clear();
    SoundBuffers.clear();
    Sounds.clear();
}

/**
* @brief Инициализирует звуки для игры.
* 
* Загружает звуковые файлы и подготавливает их к воспроизведению.
*/
void ASoundManager::InitSoundGame()
{
    // Загрузка фоновой музыки
    LoadSound("BackgroundMusic", ASSETS_PATH + "Sounds/Background.mp3"); // Фоновая музыка во время игры
    LoadSound("MenuMusic", ASSETS_PATH + "Sounds/MenuMusic.mp3"); // Фоновая музыка в меню

    // Загружаем звуки и добавляем их в контейнеры
    LoadSound("PlayerDamage", ASSETS_PATH + "Sounds/PlayerDamage.mp3"); // Звук, когда персонаж получает урон
    LoadSound("PlayerDeath", ASSETS_PATH + "Sounds/PlayerDeath.mp3"); // Звук смерти персонажа
    LoadSound("PlayerShot", ASSETS_PATH + "Sounds/Shot.mp3"); // Звук выстрела
    LoadSound("EnemyDeath", ASSETS_PATH + "Sounds/EnemyDeath.mp3"); // Звук смерти врагов
    LoadSound("KeyFound", ASSETS_PATH + "Sounds/KeyFound.mp3"); // Звук, когда персонаж подобрал ключ
    LoadSound("OpenChest", ASSETS_PATH + "Sounds/OpenChest.mp3"); // Звук открытия сундука
    LoadSound("EatenHeal", ASSETS_PATH + "Sounds/EatenHeal.mp3"); // Звук съедания лечения
    LoadSound("Lava", ASSETS_PATH + "Sounds/LavaDamage.mp3"); // Звук бурлящей лавы
    LoadSound("Peaks", ASSETS_PATH + "Sounds/PeaksDamage.mp3"); // Звук пронзающих пик персонажа
    LoadSound("ClickButton", ASSETS_PATH + "Sounds/ClickButton.mp3"); // Звук клика по кнопки
    LoadSound("DialogueDevice", ASSETS_PATH + "Sounds/DialogueDevice.mp3"); // Звук старого работающего устройства
    LoadSound("OffDialogueDevice", ASSETS_PATH + "Sounds/OffDialogueDevice.mp3"); // Звук выключение старого устройства
    LoadSound("Helicopter", ASSETS_PATH + "Sounds/Helicopter.mp3"); // Звук верталёта
    LoadSound("SMSDialogue", ASSETS_PATH + "Sounds/SMS8bit.mp3"); // Звук короткого пика, как на старых устройствах
    LoadSound("OpenGate", ASSETS_PATH + "Sounds/OpenGate.mp3"); // Звук открытия ворот
    LoadSound("BulletHit", ASSETS_PATH + "Sounds/BulletHit.mp3"); // Звук попадения пули по врагу
    LoadSound("RocketExplosion", ASSETS_PATH + "Sounds/RocketExplosion.mp3"); // Звук попадения рокеты(дитонация)
}

/**
* @brief Устанавливает уровень громкости для всех звуков.
* 
* @param Volume Уровень громкости (от 0.0 до 50.0).
*/
void ASoundManager::SetMasterVolume(float Volume)
{
    // Устанавливаем общий уровень громкости для всех звуков
    MasterVolumeLevel = Volume;
    for (auto& Val : Sounds | std::views::values)
    {
        float IndividualVolume = IndividualVolumeLevel[&Val];
        Val.setVolume(IndividualVolume * MasterVolumeLevel / 100.f);
    }
    PreviousMasterVolume = Volume;
}

/**
* @brief Плавный переход между звуками.
* 
* Выполняет плавный переход громкости для текущих звуков.
*/
void ASoundManager::SmoothTransition()
{
    // Увеличиваем громкость вертолета
    if (isHelicopterStarted)
    {
        if (HelicopterVolume < 100.f)
        {
            float TransitionSpeedHelicopter = 1.f; // Скорость перехода за секунду

            if (HelicopterVolume == 0.f)
            {
                PlaySound("Helicopter");
            }

            HelicopterVolume += TransitionSpeedHelicopter; // Плавное увеличение с учетом времени
            SetVolume("Helicopter", std::min(100.f, HelicopterVolume));
        }
        else
        {
            isHelicopterStarted = false;
        }
    }

    if (!isHelicopterStarted)
    {
        BackgroundVolume = GetSound("BackgroundMusic").getVolume();
    }

    // Уменьшаем громкость фоновой музыки
    if (BackgroundVolume > 0.f)
    {
        float TransitionSpeedBackground = 0.03f; // Скорость перехода за секунду

        BackgroundVolume -= TransitionSpeedBackground; // Плавное уменьшение с учетом времени
        SetVolume("BackgroundMusic", std::max(0.f, BackgroundVolume));

        if (BackgroundVolume <= 0.f)
        {
            StopSound("BackgroundMusic");
            isHelicopterStarted = true;
        }
    }
}

/**
* @brief Получает ссылку на звук по его имени.
* 
* @param SoundName Имя звука.
* 
* @return sf::Sound& Ссылка на объект звука.
*/
sf::Sound& ASoundManager::GetSound(const std::string& SoundName)
{
    auto It = Sounds.find(SoundName);
    if (It != Sounds.end())
    {
        return It->second;
    }
    throw std::runtime_error("Sound not found: " + SoundName);
}

/**
* @brief Устанавливает уровень громкости для конкретного звука.
* 
* @param SoundName Имя звука.
* @param Volume Уровень громкости (от 0.0 до 50.0).
* 
* @return sf::Sound const& Константная ссылка на объект звука.
*/
sf::Sound const& ASoundManager::SetVolume(const std::string& SoundName, float Volume)
{
    auto It = Sounds.find(SoundName);
    if (It != Sounds.end())
    {
        IndividualVolumeLevel[&It->second] = Volume; // Сохраняем индивидуальный уровень громкости
        It->second.setVolume(Volume * MasterVolumeLevel / 100.f); // Применяем громкость с учётом MasterVolume
        return It->second;
    }
    throw std::runtime_error("Sound not found: " + SoundName);
}

/**
* @brief Загружает звук и добавляет его в контейнер.
* 
* @param SoundName Имя звука.
* @param FilePath Путь к файлу со звуком.
*/
void ASoundManager::LoadSound(const std::string& SoundName, const std::string& FilePath)
{
    sf::SoundBuffer Buffer;
    if (!Buffer.loadFromFile(FilePath))
    {
        throw std::runtime_error("Error: Failed to load texture: " + ASSETS_PATH + FilePath);
    }

    // Сохраняем буфер в контейнер
    SoundBuffers[SoundName] = std::move(Buffer);

    sf::Sound Sound;

    // Привязываем буфер к объекту sf::Sound
    Sound.setBuffer(SoundBuffers[SoundName]);

    // Сохраняем звук в контейнер
    Sounds[SoundName] = std::move(Sound);
}

/**
* @brief Воспроизводит звук по его имени.
* 
* @param SoundName Имя звука.
* 
* @return sf::Sound const& Константная ссылка на объект звука.
*/
sf::Sound const& ASoundManager::PlaySound(const std::string& SoundName)
{
    auto It = Sounds.find(SoundName);
    if (It != Sounds.end())
    {
        It->second.play();
        return It->second;
    }
    throw std::runtime_error("Sound not found: " + SoundName);
}

/**
* @brief Останавливает воспроизведение звука по его имени.
* 
* @param SoundName Имя звука.
* 
* @return sf::Sound const& Константная ссылка на объект звука.
*/
sf::Sound const& ASoundManager::StopSound(const std::string& SoundName)
{
    auto It = Sounds.find(SoundName);
    if (It != Sounds.end())
    {
        It->second.stop();
        return It->second;
    }
    throw std::runtime_error("Sound not found: " + SoundName);
}
