# Corsac ECS

## Component

Объявление компонента

```c++
corsac::Component<uint32_t, uint32_t> Position;
```

Можно задать `кол-во сущностей` и `возможность переполнения заданного значения`

```c++
corsac::ComponentMake<
    32,                     // default: 0
    false,                  // default: true
    uint32_t, uint32_t
> Position;
```

## Group

Объявление группы

```c++
corsac::Group<Position(10, 20), Direction(0, 0)> Unit;
```

Комбинирование групп

```c++
corsac::Group<Unit, Damage(10)> Enemy;
```

Можно задать `кол-во сущностей` и `возможность переполнения заданного значения`

```c++
corsac::GroupMake<
    32,                     // default: 0
    false,                  // default: true
    Position(10, 20), Direction(0, 0)
>;
```

## EntityType

Создать пустую сущность

```c++
corsac::EntityType Player;
```

Создать сущность группы

```c++
corsac::EntityType<Unit> Player;
```

Можно задать `кол-во эффетов` и `возможность переполнения заданного значения`

```c++
corsac::EntityType<
    Unit,
    4,      // default: 0
    false   // default: true
>;
```

Удалить сущность

```c++
Player.destroy();
```

Сменить группу сущности

```c++
Player.move<Enemy>();
```

## Effect

Добавить эффект

```c++
Player.add<Flame>(10);
```

Изменить эффект/компонент

```c++
Player.rep<Flame>(20);
```

Удалить эффект

```c++
Player.del<Flame>();
```

Очистить сущность от всех эффектов

```c++
Player.clear();
```

## System

Объявить систему группы

```c++
inline void Move(auto ent, auto &pos, auto &dir)
{
    // ...
}
```

```c++
corsac::System<Move>(Unit);
```

Объявить систему эффекта/компонента

```c++
inline void Fire(auto ent, auto &fl)
{
    // ...
}
```

```c++
corsac::System<Fire>(Flame);
```
## Пример

```c++

using namespace corsac;

class MainScene
{
    // Components
    Component<uint32_t, uint32_t>   Position;

    Component<uint32_t, uint32_t>   Direction;

    Component<std::string_view>     Sprite;

    Component<> Control;

    Component<> AI;

    // Groups
    Group<Position(0, 0), Direction(0, 0)> Unit;

    Group<Unit, AI, Sprite("enemy.png")> Enemy;

    Group<Unit, Control, Sprite("player.png")> Player;

    void Init()
    {
        EntityType<Player> Hero;
        Hero.rep<Position>(120, 400);

        EntityType<Enemy> Goblin;
        Goblin.rep<Position>(120, 700);
    }

    void Update()
    {
        corsac::System<[](auto ent, auto &pos, auto &dir){

            // ...

        }>(Unit);

        // and more systems...
    }
}


```