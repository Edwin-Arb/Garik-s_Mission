#pragma once

#include <tmxlite/Layer.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include "../Constants.h"
#include "../Player/Player.h"

/**
 * @brief Класс, представляющий игровую карту.
 * 
 * Этот класс отвечает за загрузку, обработку и отрисовку игровой карты,
 * а также за управление слоями коллизий и спавном игровых объектов.
 */
class AGameMap
{
public:
    /**
     * @brief Конструктор с ссылкой на игрока.
     */
    AGameMap() = default;

    /**
     * @brief Деструктор класса.
     * 
     * Очищает используемые ресурсы, такие как текстуры и массивы вершин.
     */
    ~AGameMap();

    /**
     * @brief Инициализация игровой карты.
     * 
     * Загружает карту, текстуры, инициализирует слои и обрабатывает коллизии.
     */
    void InitGameMap();

private:
    /**
     * @brief Загрузка тайлсетов.
     * 
     * Загружает текстуры тайлов, используемые на карте.
     * 
     * @param GameMap Объект карты, содержащий информацию о тайлсетах.
     */
    void LoadTilesets(const tmx::Map& GameMap);

    /**
     * @brief Проверка слоев карты.
     * 
     * Проходит по всем слоям карты и обрабатывает тайловые слои.
     * 
     * @param GameMap Объект карты, содержащий слои.
     */
    void CheckLayers(const tmx::Map& GameMap);

    /**
     * @brief Обработка тайлового слоя.
     * 
     * Создает массивы вершин для каждого тайлового слоя карты, подготавливая их к отрисовке.
     * 
     * @param TileLayerPtr Указатель на объект тайлового слоя.
     */
    void ProcessTileLayer(const tmx::TileLayer* TileLayerPtr);

    /**
     * @brief Обработка слоев коллизий.
     * 
     * Определяет слои, отвечающие за коллизии, лестницы и спавн объектов, и сохраняет соответствующие данные.
     * 
     * @param GameMap Объект карты, содержащий слои с информацией о коллизиях.
     */
    void ProcessCollisionLayers(const tmx::Map& GameMap);

public:
    /**
    * @brief Сброс состояния карты.
    * 
    * Возвращает карту в её исходное состояние для повторного использования.
    */
    void ResetGameMap();

    /**
    * @brief Отрисовка игровой карты.
    * 
    * @param Window Окно, в котором будет отображаться карта.
    */
    void DrawGameMap(sf::RenderWindow& Window) const;

   /**
    * @brief Коллизия для начала затишья фоновой музыки и активации звука вертолёта.
    * 
    * Эта коллизия срабатывает, когда персонаж приближается к точке эвакуации,
    * вызывая уменьшение громкости фоновой музыки и включение звука вертолёта.
    * 
    * @return Прямоугольник, представляющий область коллизии для активации звука вертолёта.
    */
    sf::FloatRect GetSpawnSoundHelicopterLayer() const;

    /**
     * @brief Коллизия, которая обозначает завершение игры и начало проигрывания титров.
     * 
     * Когда персонаж попадает в эту область, игра заканчивается и начинается показ титров.
     * 
     * @return Прямоугольник, представляющий область коллизии завершения игры.
     */
    sf::FloatRect GetMissionAccomplishedLayer() const;

    /**
     * @brief Вектор коллизий игровой карты.
     * 
     * @return Вектор прямоугольников коллизий, определяющие препятствия на карте.
     */
    std::vector<sf::FloatRect>& GetGameMapCollisionVector();

    /**
     * @brief Получение вектора коллизий участков карты, которые наносят урон.
     * 
     * @return Вектор прямоугольников коллизий участков карты, наносящих урон.
     */
    std::vector<sf::FloatRect> GetLavaDamageCollisionVector() const;

    /**
    * @brief Получение вектора с коллизиями, наносящими урон.
    * 
    * @return Вектор прямоугольников, представляющих области с повреждающими коллизиями.
    */
    std::vector<sf::FloatRect> GetPeaksDamageCollisionVector() const;

   /**
    * @brief Получение вектора с коллизиями лестниц.
    * 
    * @return Вектор прямоугольников, представляющих области с коллизиями лестниц.
    */
    std::vector<sf::FloatRect> GetLadderCollisionVector() const;

    /**
     * @brief Вектор позиций базовых врагов.
     * 
     * Хранит позиции для спавна обычных врагов на карте.
     */
    std::vector<sf::Vector2f> GetSpawnBaseEnemyPositionVector() const;

    /**
     * @brief Вектор позиций босса врагов.
     * 
     * Хранит позиции для спавна босса врагов на карте.
     */
    std::vector<sf::Vector2f> GetSpawnBossEnemyPositionVector() const;

    /**
     * @brief Вектор позиций сундуков.
     * 
     * Хранит позиции для спавна сундуков на карте.
     */
    std::vector<sf::Vector2f> GetSpawnChestPositionVector() const;

   /**
    * @brief Вектор позиций лечащих объектов.
    * 
    * Хранит позиции для спавна лечащих объектов на карте.
    */
    std::vector<sf::Vector2f> GetSpawnHealingObjectsPositionVector() const;

private:
    tmx::Map GameMap;                                        // Игровая карта, загруженная из файла
            
    sf::FloatRect SpawnSoundHelicopterLayer;                 // Коллизи, чтобы воспроизвести звук вертолёта,
                                                             // когда будет подходить к точке эвакуации
    sf::FloatRect MissionAccomplishedLayer;                  // Коллизи, где будет проиходит эвакуация персонажа и конец игры
    std::vector<sf::FloatRect> GameMapCollisionLayer;        // Вектор коллизий участков карты, через которые персонаж, враги и пули не могу пройти
    std::vector<sf::FloatRect> LavaDamageCollisionLayer;     // Вектор коллизий лавы, которые наносят урон
    std::vector<sf::FloatRect> PeaksDamageCollisionLayer;    // Вектор коллизий пик, которые наносят урон
    std::vector<sf::FloatRect> LadderCollisionLayer;         // Вектор коллизий лестниц

    std::vector<sf::Vector2f> SpawnBaseEnemyPosition;        // Вектор, который хранит позиции для спавна обычных врагов на карте.
    std::vector<sf::Vector2f> SpawnBossEnemyPosition;        // Вектор, который хранит позиции для спавна босса врагов на карте.
    std::vector<sf::Vector2f> SpawnChestPosition;            // Вектор, который хранит позиции для спавна сундуков на карте.
    std::vector<sf::Vector2f> SpawnHealingObjectsPosition;   // Вектор, который хранит позиции для спавна лечения на карте.

    std::vector<sf::VertexArray> LayersVector;               // Вектор массивов вершин для каждого слоя карты
    std::vector<sf::RenderStates> RenderStatesVector;        // Вектор состояний отрисовки для каждого слоя карты
    std::map<uint32_t, sf::Texture> TilesetTextures;         // Карта, связывающая идентификаторы тайлов с их текстурами.
};
