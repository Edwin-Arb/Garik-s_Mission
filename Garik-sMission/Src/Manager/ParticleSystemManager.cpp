#include "ParticleSystemManager.h"
#include <ranges>

/**
* @brief Деструктор класса AParticleSystemManager.
* Нужен для очистки памяти.
*/
AParticleSystemManager::~AParticleSystemManager()
{
    ParticleEffectsMap.clear();
}

/**
* @brief Инициализирует систему частиц.
* 
* Этот метод должен быть вызван перед использованием системы частиц.
*/
void AParticleSystemManager::InitParticleSystem()
{
    // Инициализация эффекта разрушения пули
    AddParticleEffect("BulletDestroy", ASSETS_PATH + "MainTiles/Bullets.png", 10.f,
                      {
                          sf::IntRect(21, 54, 7, 7),
                          sf::IntRect(37, 54, 7, 7),
                          sf::IntRect(53, 54, 7, 7),
                          sf::IntRect(68, 54, 7, 7),
                          sf::IntRect(0, 0, 0, 0),
                          // Пустой кадр, чтобы была иллюзия, что объект уничтожен, а не останавливался на последнем кадре
                      });

    // Инициализация эффекта взрыва рокеты
    AddParticleEffect("RocketDestroy", ASSETS_PATH + "MainTiles/Bullets.png", 20.f,
                      {
                          sf::IntRect(53, 6, 7, 7),
                          sf::IntRect(67, 4, 10, 10),
                          sf::IntRect(81, 3, 14, 14),
                          sf::IntRect(96, 3, 15, 14),
                          sf::IntRect(114, 3, 11, 13),
                          sf::IntRect(0, 0, 0, 0),
                      });

    // Инициализация эффекта пламя из дула оружия, после выстрела
    AddParticleEffect("MuzzleFlash", ASSETS_PATH + "MainTiles/Bullets.png", 30.f,
                      {
                          sf::IntRect(51, 70, 4, 8),
                          sf::IntRect(0, 0, 0, 0)
                      });
}

/**
* @brief Добавляет новый эффект частиц в систему.
* 
* @param EffectName Название эффекта частиц.
* @param TexturePath Путь к текстуре эффекта частиц.
* @param FrameSpeed Скорость смены кадров анимации.
* @param Frames Вектор прямоугольников, определяющих кадры анимации.
*/
void AParticleSystemManager::AddParticleEffect(const std::string& EffectName, const std::string& TexturePath,
                                               float FrameSpeed, const std::vector<sf::IntRect>& Frames)
{
    AAnimationManager NewEffect;
    NewEffect.AnimTexture.loadFromFile(TexturePath);
    NewEffect.FrameSpeed = FrameSpeed;
    NewEffect.FrameRect = Frames;

    // Добавляем новый эффект в карту с именем EffectName как ключ
    ParticleEffectsMap.emplace(EffectName, std::move(NewEffect));
}

/**
* @brief Запускает воспроизведение эффекта частиц.
* 
* @param EffectName Название эффекта частиц для воспроизведения.
*/
void AParticleSystemManager::PlayEffect(const std::string& EffectName)
{
    if (ParticleEffectsMap.contains(EffectName))
    {
        ParticleEffectsMap[EffectName].Animation.SetCurrentFrame(0);
        ParticleEffectsMap[EffectName].Animation.ResetAnimation();
    }
}

/**
* @brief Запускает воспроизведение эффекта частиц с заданной позицией и параметрами.
* 
* @param EffectName Название эффекта частиц для воспроизведения.
* @param ObjectPosition Позиция объекта, на который нужно отобразить эффект частиц.
* @param Offset Смещение эффекта частиц относительно позиции объекта.
* @param EffectScale Масштаб эффекта частиц.
* @param bIsMoveRight Флаг, указывающий, движется ли эффект частиц вправо.
*/
void AParticleSystemManager::PlayEffectWithPosition(const std::string& EffectName,
                                                    const sf::Vector2f& ObjectPosition,
                                                    const sf::Vector2f& Offset,
                                                    const sf::Vector2f& EffectScale,
                                                    bool bIsMoveRight)
{
    // Проверка на существование эффекта
    if (ParticleEffectsMap.contains(EffectName))
    {
        // Создаем новый эффект
        ParticleEffect NewEffect;

        // Копируем текстуру и устанавливаем кадры вручную
        NewEffect.Animation.AnimTexture = ParticleEffectsMap[EffectName].Animation.AnimTexture;
        NewEffect.Animation.FrameSpeed = ParticleEffectsMap[EffectName].Animation.FrameSpeed;
        NewEffect.Animation.FrameRect = ParticleEffectsMap[EffectName].Animation.FrameRect;

        // Устанавливаем позицию эффекта с возможным смещением
        NewEffect.Position = ObjectPosition + Offset;

        // Устанавливаем масштаб спрайта
        // Если персонаж движется влево, отразить спрайт по оси X
        if (!bIsMoveRight)
        {
            NewEffect.Scale = {-EffectScale.x, EffectScale.y}; // Отражаем по X
        }
        else
        {
            NewEffect.Scale = EffectScale;
        }

        // Добавляем эффект в список активных
        ParticleEffectsMap[EffectName] = std::move(NewEffect);

        // Сбрасываем анимацию
        ParticleEffectsMap[EffectName].Animation.SetStopAtLastFrame(true);
    }
}

/**
* @brief Обновляет систему частиц.
* 
* @param DeltaTime Время, прошедшее с последнего обновления.
*/
void AParticleSystemManager::UpdateParticleSystem(float DeltaTime)
{
    // TODO: Используется для отладки, сколько эффектов находится в MAP
    //std::cout << "Active Particle Effects: " << ParticleEffectsMap.size() << std::endl;

    // Проходим по всем активным эффектам и обновляем их
    for (auto it = ParticleEffectsMap.begin(); it != ParticleEffectsMap.end();)
    {
        it->second.Animation.AnimationUpdate(DeltaTime);
        ++it; // Переходим к следующему элементу
    }

    // TODO: Используется для отладки, сколько эффектов находится в MAP
    //std::cout << "Active Particle Effects after update: " << ParticleEffectsMap.size() << std::endl;
}

/**
* @brief Отрисовывает эффекты частиц на экране.
* 
* @param Window Окно, на котором будут отрисованы эффекты частиц.
*/
void AParticleSystemManager::DrawParticleSystem(sf::RenderWindow& Window)
{
    for (auto& Effect : ParticleEffectsMap | std::views::values)
    {
        // Отрисовываем эффект на экране
        sf::Sprite Sprite;
        Sprite.setTexture(Effect.Animation.AnimTexture);
        Sprite.setTextureRect(Effect.Animation.GetCurrentFrame());
        Sprite.setPosition(Effect.Position);
        Sprite.setScale(Effect.Scale);

        Window.draw(Sprite);
    }
}

/**
* @brief Проверяет, завершен ли эффект частиц.
* 
* @param EffectName Название эффекта частиц для проверки.
* @return true, если эффект завершен, иначе false.
*/
bool AParticleSystemManager::IsEffectFinished(const std::string& EffectName) const
{
    if (ParticleEffectsMap.contains(EffectName))
    {
        return ParticleEffectsMap.at(EffectName).Animation.IsAnimationFinished();
    }
    return true;
}