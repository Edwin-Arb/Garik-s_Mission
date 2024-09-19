#pragma once
#include "../Abstract/AActor.h"

/**
 * @brief Перечисление типов ключей.
 */
enum class EKeyType
{
    EKT_Red,    // Красный ключ
    EKT_Yellow, // Желтый ключ
    EKT_Green,  // Зеленый ключ
    EKT_Non     // Неопределенный ключ
};

class AKeyObject : public AActor
{
public:
    /**
     * @brief Конструктор объекта ключа.
     * 
     * Инициализирует объект ключа с начальной позицией и базовыми параметрами.
     * 
     * @param StartPosition Начальная позиция ключа.
     */
    AKeyObject(const sf::Vector2f& StartPosition);

    /**
     * @brief Деструктор объекта ключа.
     */
    ~AKeyObject() override = default;

    /**
     * @brief Инициализация ключей.
     * 
     * Инициализирует текстуры и спрайты для каждого типа ключа.
     * 
     * @param SpriteManager Менеджер спрайтов для загрузки текстур.
     */
    void InitKeys(ASpriteManager& SpriteManager);

    /**
     * @brief Обновление движения ключа.
     * 
     * Обновляет положение ключа в зависимости от времени и гравитации.
     * 
     * @param DeltaTime Время, прошедшее с предыдущего кадра.
     */
    void UpdateMoveKey(float DeltaTime);

    /**
     * @brief Установка флага, найден ли ключ.
     * 
     * @param WasKeyFound Логическое значение, указывающее, был ли ключ найден.
     */
    void SetKeyFound(bool WasKeyFound);

    /**
     * @brief Установка гравитации для ключа.
     * 
     * @param NewGravity Новое значение гравитации для ключа.
     */
    void SetKeyGravity(bool NewGravity);

    /**
     * @brief Установка новой позиции ключа.
     * 
     * @param NewPosition Новая позиция ключа.
     */
    void SetActorPosition(const sf::Vector2f& NewPosition) override;

    /**
     * @brief Поместить ключ в верхний правый угол экрана.
     * 
     * @param Window Вид окна, по которому рассчитывается положение ключа.
     */
    void MakrKeyInTopRightCorner(const sf::View& Window);

    /**
     * @brief Сброс состояния ключа.
     * 
     * Сбрасывает все параметры ключа к начальному состоянию.
     */
    void ResetKey();

    /**
     * @brief Отрисовка ключа на экране.
     * 
     * @param Window Окно, в котором будет отображаться ключ.
     */
    void DrawActor(sf::RenderWindow& Window) override;

    /**
     * @brief Проверка, был ли ключ найден.
     * 
     * @return Логическое значение, указывающее, найден ли ключ.
     */
    bool GetKeyFound() const;

    /**
     * @brief Получение количества ключей.
     * 
     * @return Текущее количество ключей.
     */
    int GetKeyCount() const;

    /**
     * @brief Получение идентификатора ключа.
     * 
     * @return Идентификатор типа ключа.
     */
    EKeyType GetKeyID() const;

private:
    bool bIsKeyFound;            // Флаг, указывающий, был ли ключ найден
    bool JumpKey;                // Флаг, указывающий, должен ли ключ прыгнуть
    bool Gravity;                // Флаг, указывающий, применяется ли гравитация

public:
    static int KeysCount;        // Общее количество ключей
    static int KeysCountFound;   // Количество найденных ключей

private:
    EKeyType KeyID;              // Идентификатор типа ключа
};
