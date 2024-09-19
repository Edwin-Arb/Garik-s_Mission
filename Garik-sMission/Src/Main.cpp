#include <iostream>
#include "GameMain/GameState.h"


int WinMain()
{
    // Создаем главное окно игры с заданными размерами
    sf::RenderWindow Window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Garik's Mission");

    // Создаем таймер игры
    sf::Clock GameClock;

    // Инициализируем игровое состояние
    AGameState* GameStatePtr = new AGameState;
    GameStatePtr->StartGame();

    // Главный цикл игры
    while (Window.isOpen())
    {
        // Задаём фиксированное количество кадров в секунду(FPS),
        // для обеспечения одинаковой скорости на всех устройствах
        sleep(sf::milliseconds(2));
        
        // Получаем время прошедшее между кадрами
        float DeltaTime = GameClock.getElapsedTime().asSeconds();

        // Сбрасываем таймер для следующего кадра
        GameClock.restart();

        // Обрабатываем события окна
        sf::Event Event;
        while (Window.pollEvent(Event))
        {
            if (Event.type == sf::Event::Closed)
            {
                // Закрываем окно, если пользователь нажал крестик
                Window.close();
            }

            // Обработка кнопок в зависимости от состояния игры
            GameStatePtr->HandleButtonInteraction(Event, Window);
        }

        // Обновляем ввод пользователя
        GameStatePtr->UpdateInput(DeltaTime);

        // Обновляем игровое состояние
        GameStatePtr->UpdateGameplay(DeltaTime);

        // Обновляем положение камеры, следя за игроком
        GameStatePtr->UpdateCamera(Window);

        // Очищаем экран от предыдущего кадра
        Window.clear();

        // Отрисовываем игровые объекты
        GameStatePtr->DrawGame(Window);

        // Показываем отрисованные объекты в окне
        Window.display();
    }

    // Освобождаем память, выделенную под игровое состояние
    delete GameStatePtr;

    return 0;
}
