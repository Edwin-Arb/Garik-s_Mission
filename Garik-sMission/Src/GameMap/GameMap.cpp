#include "GameMap.h"

/**
* @brief Деструктор класса.
* 
* Очищает используемые ресурсы, такие как текстуры и массивы вершин.
*/
AGameMap::~AGameMap()
{
    // Очищаем текстуры тайлов
    TilesetTextures.clear();

    // Очищаем массивы вершин
    LayersVector.clear();

    // Очищаем состояния отрисовки
    RenderStatesVector.clear();

    // Очищаем слои коллизий карты
    GameMapCollisionLayer.clear();
    LadderCollisionLayer.clear();
    LavaDamageCollisionLayer.clear();

    // Очищаем вектора позиций спавна врагов и сундуков
    SpawnBaseEnemyPosition.clear();
    SpawnBossEnemyPosition.clear();
    SpawnChestPosition.clear();
}

/**
* @brief Инициализация игровой карты.
* 
* Загружает карту, текстуры, инициализирует слои и обрабатывает коллизии.
*/
void AGameMap::InitGameMap()
{
    // Резервируем место для коллизий карты
    int ReserveCollisionCapacity = 100;
    GameMapCollisionLayer.reserve(ReserveCollisionCapacity);
    LadderCollisionLayer.reserve(5); // Резервируем место для коллизий лестниц

    // Загружаем карту из файла
    if (!GameMap.load(ASSETS_PATH + "Map/GarikMap.tmx"))
    {
        throw std::runtime_error("Error: Failed to load texture: " + ASSETS_PATH + "Map/GarikMap.tmx");
    }

    // Загружаем текстуры тайлов
    LoadTilesets(GameMap);

    // Проверяем и обрабатываем слои карты
    CheckLayers(GameMap);

    // Обрабатываем слои коллизий
    ProcessCollisionLayers(GameMap);
}

/**
* @brief Загрузка тайлсетов.
* 
* Загружает текстуры тайлов, используемые на карте.
* 
* @param GameMap Объект карты, содержащий информацию о тайлсетах.
*/
void AGameMap::LoadTilesets(const tmx::Map& GameMap)
{
    const auto& Tilesets = GameMap.getTilesets();
    for (const auto& Tileset : Tilesets)
    {
        sf::Texture Texture;
        std::string ImagePath = Tileset.getImagePath();

        // Загружаем текстуру тайлов
        if (Texture.loadFromFile(Tileset.getImagePath()))
        {
            TilesetTextures[Tileset.getFirstGID()] = std::move(Texture);
        }
    }
}

/**
* @brief Проверка слоев карты.
* 
* Проходит по всем слоям карты и обрабатывает тайловые слои.
* 
* @param GameMap Объект карты, содержащий слои.
*/
void AGameMap::CheckLayers(const tmx::Map& GameMap)
{
    for (const auto& Layer : GameMap.getLayers())
    {
        // Проверяем, является ли слой тайловым
        if (Layer->getType() == tmx::Layer::Type::Tile)
        {
            const auto* TileLayerPtr = dynamic_cast<const tmx::TileLayer*>(Layer.get());
            // Обрабатываем тайловый слой
            ProcessTileLayer(TileLayerPtr);
        }
    }
}

/**
* @brief Обработка тайлового слоя.
* 
* Создает массивы вершин для каждого тайлового слоя карты, подготавливая их к отрисовке.
* 
* @param TileLayerPtr Указатель на объект тайлового слоя.
*/
void AGameMap::ProcessTileLayer(const tmx::TileLayer* TileLayerPtr)
{
    const auto& Tiles = TileLayerPtr->getTiles();
    const auto& MapSize = TileLayerPtr->getSize();
    std::map<const sf::Texture*, sf::VertexArray> LayerVertexMap;

    for (uint32_t y = 0; y < MapSize.y; ++y)
    {
        for (uint32_t x = 0; x < MapSize.x; ++x)
        {
            tmx::TileLayer::Tile Tile = Tiles[y * MapSize.x + x];
            if (Tile.ID == 0)
            {
                continue; // Пропускаем пустые тайлы
            }

            const sf::Texture* CurrentTexturePtr = nullptr;
            uint32_t FirstGID = 0;

            // Определяем, к какому тайлсету принадлежит тайл
            for (const auto& Pair : TilesetTextures)
            {
                if (Tile.ID >= Pair.first)
                {
                    CurrentTexturePtr = &Pair.second;
                    FirstGID = Pair.first;
                }
                else
                {
                    break;
                }
            }

            if (!CurrentTexturePtr)
            {
                continue;
            }

            // Рассчитываем координаты текстуры
            uint32_t LocalTileID = Tile.ID - FirstGID;
            uint32_t tilesPerRow = CurrentTexturePtr->getSize().x / 16;
            uint32_t TextureHorizontal = LocalTileID % tilesPerRow;
            uint32_t TextureVertical = LocalTileID / tilesPerRow;

            sf::VertexArray& Vertices = LayerVertexMap[CurrentTexturePtr];
            if (Vertices.getVertexCount() == 0)
            {
                Vertices.setPrimitiveType(sf::Quads);
                Vertices.resize(MapSize.x * MapSize.y * 4);
            }

            // Задаем позиции вершин для каждого тайла
            sf::Vertex* Quad = &Vertices[(x + y * MapSize.x) * 4];

            Quad[0].position = sf::Vector2f(x * 16.f, y * 16.f);
            Quad[1].position = sf::Vector2f((x + 1) * 16.f, y * 16.f);
            Quad[2].position = sf::Vector2f((x + 1) * 16.f, (y + 1) * 16.f);
            Quad[3].position = sf::Vector2f(x * 16.f, (y + 1) * 16.f);

            // Задаем текстурные координаты
            sf::Vector2f texCoords[4] = {
                sf::Vector2f(TextureHorizontal * 16.f, TextureVertical * 16.f),
                sf::Vector2f((TextureHorizontal + 1) * 16.f, TextureVertical * 16.f),
                sf::Vector2f((TextureHorizontal + 1) * 16.f, (TextureVertical + 1) * 16.f),
                sf::Vector2f(TextureHorizontal * 16.f, (TextureVertical + 1) * 16.f)
            };

            Quad[0].texCoords = texCoords[0];
            Quad[1].texCoords = texCoords[1];
            Quad[2].texCoords = texCoords[2];
            Quad[3].texCoords = texCoords[3];
        }
    }

    // Сохраняем массивы вершин и состояния отрисовки
    for (const auto& Pair : LayerVertexMap)
    {
        LayersVector.emplace_back(Pair.second);
        RenderStatesVector.emplace_back();
        RenderStatesVector.back().texture = Pair.first;
    }
}

/**
* @brief Обработка слоев коллизий.
* 
* Определяет слои, отвечающие за коллизии, лестницы и спавн объектов, и сохраняет соответствующие данные.
* 
* @param GameMap Объект карты, содержащий слои с информацией о коллизиях.
*/
void AGameMap::ProcessCollisionLayers(const tmx::Map& GameMap)
{
    for (const auto& Layer : GameMap.getLayers())
    {
        if (Layer->getType() == tmx::Layer::Type::Object)
        {
            // Получаем слой объектов (коллизий, спавн-точек и т.д.)
            const auto* ObjectLayer = dynamic_cast<const tmx::ObjectGroup*>(Layer.get());

            if (!ObjectLayer)
            {
                continue;
            }

            for (const auto& Object : ObjectLayer->getObjects())
            {
                tmx::FloatRect TmxRect = Object.getAABB();
                
                // Добавляем объекты в слой коллизий
                if (ObjectLayer && ObjectLayer->getName() == "SpawnSoundHelicopter")
                {
                    SpawnSoundHelicopterLayer = {TmxRect.left, TmxRect.top, TmxRect.width, TmxRect.height};
                }
                else if (ObjectLayer && ObjectLayer->getName() == "FinishCollision")
                {
                    MissionAccomplishedLayer = {TmxRect.left, TmxRect.top, TmxRect.width, TmxRect.height};
                }
                else if (ObjectLayer && ObjectLayer->getName() == "Obstacles")
                {
                    GameMapCollisionLayer.emplace_back(TmxRect.left, TmxRect.top, TmxRect.width, TmxRect.height);
                }
                // Добавляем объекты в слой повреждающих коллизий лавы
                else if (ObjectLayer && ObjectLayer->getName() == "LavaDamageCollision")
                {
                    LavaDamageCollisionLayer.emplace_back(TmxRect.left, TmxRect.top, TmxRect.width, TmxRect.height);
                }
                // Добавляем объекты в слой повреждающих коллизий пик
                else if (ObjectLayer && ObjectLayer->getName() == "PeaksDamageCollision")
                {
                    PeaksDamageCollisionLayer.emplace_back(TmxRect.left, TmxRect.top, TmxRect.width, TmxRect.height);
                }
                // Добавляем объекты в слой коллизий лестниц
                else if (ObjectLayer && ObjectLayer->getName() == "Ladders")
                {
                    LadderCollisionLayer.emplace_back(TmxRect.left, TmxRect.top, TmxRect.width, TmxRect.height);
                }
                // Устанавливаем позиции спавна врагов
                else if (ObjectLayer && ObjectLayer->getName() == "SpawnEnemy")
                {
                    sf::Vector2f TmxRectEnemySpawn = {Object.getPosition().x, Object.getPosition().y};
                    if (Object.getUID() == 399 || Object.getUID() == 420 || Object.getUID() == 421)
                    {
                        SpawnBossEnemyPosition.emplace_back(TmxRectEnemySpawn);
                    }
                    else
                    {
                        SpawnBaseEnemyPosition.emplace_back(TmxRectEnemySpawn);
                    }
                }
                // Устанавливаем позиции спавна сундуков
                else if (ObjectLayer && ObjectLayer->getName() == "SpawnChest")
                {
                    sf::Vector2f TmxRectSpawnChest = {Object.getPosition().x, Object.getPosition().y};
                    SpawnChestPosition.emplace_back(TmxRectSpawnChest);
                }
                // Устанавливаем позиции спавна лечащих объектов(ветчины)
                else if (ObjectLayer && ObjectLayer->getName() == "SpawnHealingObjects")
                {
                    sf::Vector2f TmxRectSpawnChest = {Object.getPosition().x, Object.getPosition().y};
                    SpawnHealingObjectsPosition.emplace_back(TmxRectSpawnChest);
                }
            }
        }
    }
}

/**
* @brief Сброс состояния карты.
* 
* Возвращает карту в её исходное состояние для повторного использования.
*/
void AGameMap::ResetGameMap()
{
    SpawnBaseEnemyPosition.clear();
    SpawnBossEnemyPosition.clear();
    SpawnChestPosition.clear();
    SpawnHealingObjectsPosition.clear();
    GameMapCollisionLayer.clear();
    LavaDamageCollisionLayer.clear();
    LadderCollisionLayer.clear();
    LayersVector.clear();
    RenderStatesVector.clear();
    TilesetTextures.clear();
}

/**
* @brief Отрисовка игровой карты.
* 
* @param Window Окно, в котором будет отображаться карта.
*/
void AGameMap::DrawGameMap(sf::RenderWindow& Window) const
{
    for (size_t i = 0; i < LayersVector.size(); ++i)
    {
        Window.draw(LayersVector[i], RenderStatesVector[i]);
    }
}

/**
* @brief Коллизия для начала затишья фоновой музыки и активации звука вертолёта.
* 
* Эта коллизия срабатывает, когда персонаж приближается к точке эвакуации,
* вызывая уменьшение громкости фоновой музыки и включение звука вертолёта.
* 
* @return Прямоугольник, представляющий область коллизии для активации звука вертолёта.
*/
sf::FloatRect AGameMap::GetSpawnSoundHelicopterLayer() const
{
    return SpawnSoundHelicopterLayer;
}

/**
* @brief Коллизия, которая обозначает завершение игры и начало проигрывания титров.
* 
* Когда персонаж попадает в эту область, игра заканчивается и начинается показ титров.
* 
* @return Прямоугольник, представляющий область коллизии завершения игры.
*/
sf::FloatRect AGameMap::GetMissionAccomplishedLayer() const
{
    return MissionAccomplishedLayer;
}

/**
* @brief Вектор коллизий игровой карты.
* 
* @return Вектор прямоугольников коллизий, определяющие препятствия на карте.
*/
std::vector<sf::FloatRect>& AGameMap::GetGameMapCollisionVector()
{
    return GameMapCollisionLayer;
}

/**
* @brief Получение вектора коллизий участков карты, которые наносят урон.
* 
* @return Вектор прямоугольников коллизий участков карты, наносящих урон.
*/
std::vector<sf::FloatRect> AGameMap::GetLavaDamageCollisionVector() const
{
    return LavaDamageCollisionLayer;
}

/**
* @brief Получение вектора с коллизиями, наносящими урон.
* 
* @return Вектор прямоугольников, представляющих области с повреждающими коллизиями.
*/
std::vector<sf::FloatRect> AGameMap::GetPeaksDamageCollisionVector() const
{
    return PeaksDamageCollisionLayer;
}

/**
 * @brief Получение вектора с коллизиями лестниц.
 * 
 * @return Вектор прямоугольников, представляющих области с коллизиями лестниц.
 */
std::vector<sf::FloatRect> AGameMap::GetLadderCollisionVector() const
{
    return LadderCollisionLayer;
}

/**
* @brief Вектор позиций базовых врагов.
* 
* Хранит позиции для спавна обычных врагов на карте.
*/
std::vector<sf::Vector2f> AGameMap::GetSpawnBaseEnemyPositionVector() const
{
    return SpawnBaseEnemyPosition;
}

/**
* @brief Вектор позиций босса врагов.
* 
* Хранит позиции для спавна босса врагов на карте.
*/
std::vector<sf::Vector2f> AGameMap::GetSpawnBossEnemyPositionVector() const
{
    return SpawnBossEnemyPosition;
}

/**
* @brief Вектор позиций сундуков.
* 
* Хранит позиции для спавна сундуков на карте.
*/
std::vector<sf::Vector2f> AGameMap::GetSpawnChestPositionVector() const
{
    return SpawnChestPosition;
}

/**
* @brief Вектор позиций лечащих объектов.
* 
* Хранит позиции для спавна лечащих объектов на карте.
*/
std::vector<sf::Vector2f> AGameMap::GetSpawnHealingObjectsPositionVector() const
{
    return SpawnHealingObjectsPosition;
}
