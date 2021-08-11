# Corsac Engine 2D: Go

## О проекте

Да, мы разрабатываем 2D кроссплатформенный игровой движок с использованием собственной реализации шаблона проектирования Сущность Компонент Система (Entity Component System), мультимедийной библеотеки SDL2 и языка программирования GO.

Этот репозиторий наш полигон для тестирования и реализации идей.
В данный момент пока не предназначен для непосвященных в разработку пользователей.

## Средства разработки

- [Go v1.16.7](https://golang.org/dl/)
- [Go sdl2 v0.4.8](https://github.com/veandco/go-sdl2)
- [SDL2 v2.0.14](https://www.libsdl.org/download-2.0.php)
- [SDL_image v2.0.5](https://www.libsdl.org/projects/SDL_image/)
- [SDL_mixer v2.0.4](https://www.libsdl.org/projects/SDL_mixer/)
- [MinGW v8.1.0](http://mingw-w64.org/doku.php)

## Corsac ECS

Шаблон проектирования ECS, представляет собой удобный инструмент для реализации любого игрового объекта, интуитивно понятен и переиспользует максимум вашего кода. Представляем реализацию шаблона, ипользуемого в Corsac Engine:

```golang
package main

import "github.com/Falldot/ce2d/pkg/ecs"

func main() {
    // Инициализация пула, контейнера всех сущностей, компонентов и систем
	pool := ecs.CreatePool()

    // ... code

    // Метод добавления систем
    // ! группируйте системы по их назначению 
    pool.AddSystem(LoadResourcer, Cleaner, UnLoad)
	pool.AddSystem(Translate_Getter, Translate_Filter, Translate_Trigger, Translate_Executer)

    // ... code

    // Перед игровым циклом срабатывают "Иниторы"
	pool.Init()
    // Игровой цикл!
    for true {
        // В игровым цикле срабатывают "Геттеры", "Фильтры", "Тригеры", "Экзекъютеры"
        pool.Execute(1)

        // ... code

        // В конце игрового цикла срабатывают "Клинеры"
	    pool.Clean()
    }
    // После завершения программы срабатывают "Экзиторы"
	pool.Exit()
}
```

## Текущие результаты

![Alt Text](https://media.giphy.com/media/LBpvP7xT4IzWRnhY3D/giphy.gif)

![Alt Text](https://media.giphy.com/media/PaHC6Fegk4XhZmVBRI/giphy.gif)

![Alt Text](https://media.giphy.com/media/bOMKe3a53uHFPEohuv/giphy.gif)
