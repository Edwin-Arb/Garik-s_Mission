#include "GameOver.h"

/**
* @brief Конструктор класса AGameOver.
* 
* Инициализирует необходимые ресурсы для экрана "Game Over".
*/
AGameOver::AGameOver()
{
    ActorSize = {50, 19};
    
    ActorVelocity = {0.f, 0.f};

    // Устанавливаем прямоугольник коллизии для отслеживания позиции текста
    ActorCollisionRect.left = 0;
    ActorCollisionRect.top = 0;
    ActorCollisionRect.width = ActorSize.x;
    ActorCollisionRect.height = ActorSize.y;
}

/**
* @brief Инициализирует экран "Game Over".
* 
* Загружает текстуры и спрайты для экрана "Game Over".
* 
* @param SpriteManager Менеджер спрайтов для загрузки текстур.
*/
void AGameOver::InitGameOver(ASpriteManager& SpriteManager)
{
    const std::string GameOverTexturePath = ASSETS_PATH + "MainTiles/GAME_OVER.png";
    const sf::IntRect GameOverRect = {0, 0, 74, 41};

    AActor::InitActorTexture(GameOverTexturePath, GameOverRect, {1.f, 1.f}, {0.5f, 0.5f}, SpriteManager);
    
    ActorSprite.setScale(1.5f, 1.5f);

    // Устанавливаем начальное положение спрайта
    ActorDrawPosition = {
        ActorCollisionRect.left + ActorSize.x / 2.f,
        ActorCollisionRect.top + ActorSize.y / 2.f
    };
    ActorSprite.setPosition(ActorDrawPosition);
}

/**
* @brief Обновляет состояние экрана "Game Over".
* 
* Управляет логикой и обновляет позицию спрайтов.
* 
* @param DeltaTime Время, прошедшее между кадрами.
*/
void AGameOver::UpdateGameOver(float DeltaTime)
{
    // Применение "гравитации" для движения текста вниз, только если он еще не достиг нужной позиции
    if (ActorCollisionRect.top < TargetPosition.y)
    {
        float GravitySprite = 40.f;
        ActorVelocity.y += GravitySprite * DeltaTime;

        // Обновление позиции текста на основе скорости
        ActorCollisionRect.top += ActorVelocity.y;

        // Убедитесь, что текст не опускается ниже целевой позиции
        if (ActorCollisionRect.top >= TargetPosition.y + 120.f)
        {
            ActorCollisionRect.top = TargetPosition.y + 120.f; // Устанавливаем на целевую позицию
            //ActorVelocity.y = 0.f; // Останавливаем движение
        }

        // Обновляем позицию спрайта текста
        ActorDrawPosition = {
            ActorCollisionRect.left + ActorSize.x / 2.f,
            ActorCollisionRect.top + ActorSize.y / 2.f
        };

        // Установка позиции спрайта текста
        ActorSprite.setPosition(ActorDrawPosition);
    }
}

/**
* @brief Устанавливает новую позицию для экрана "Game Over".
* 
* @param NewPosition Новая позиция экрана "Game Over".
*/
void AGameOver::SetGameOverPosition(const sf::Vector2f& NewPosition)
{
    // Сбрасываем позицию и скорость
    TargetPosition = NewPosition;
    ActorCollisionRect.left = NewPosition.x - ActorSize.x / 2.f;
    ActorCollisionRect.top = NewPosition.y - ActorSize.y / 2.f;
}

/**
* @brief Сбрасывает позицию экрана "Game Over".
* 
* Возвращает экран в его исходную позицию.
*/
void AGameOver::ResetGameOverPosition()
{
    // Устанавливаем прямоугольник коллизии для отслеживания позиции текста
    ActorCollisionRect.left = 0;
    ActorCollisionRect.top = 0;
    ActorCollisionRect.width = ActorSize.x;
    ActorCollisionRect.height = ActorSize.y;

    ActorVelocity = {0.f, 0.f};
    
    // Устанавливаем начальное положение спрайта
    ActorDrawPosition = {
        ActorCollisionRect.left + ActorSize.x / 2.f,
        ActorCollisionRect.top + ActorSize.y / 2.f
    };
    ActorSprite.setPosition(ActorDrawPosition);
}

/**
* @brief Отрисовывает экран "Game Over".
* 
* @param Window Окно, в котором происходит отрисовка.
*/
void AGameOver::DrawActor(sf::RenderWindow& Window)
{
    Window.draw(ActorSprite);
}
