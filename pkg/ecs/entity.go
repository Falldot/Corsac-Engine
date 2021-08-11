package ecs

type (
	// Интерфейс реализации "Сущности"
	Entity interface {

		// Public
		ID() uint64
		Destroy()

		Add(ComponentType, Component) Entity
		Get(ComponentType) Component
		Remove(ComponentType) Entity

		// Private
		removeAll()

		has(...int) bool
	}

	// Структура реализации "Сущности"
	entity struct {
		// "Уникальный индитификатор" (ID) "Сущности"
		id uint64

		// Набор "Компонентов" "Сущности"
		components []Component

		// Контекст
		pool Pool
	}
)

// Конструктор "Сущности"
func createEntity(pool Pool, id uint64) Entity {
	return &entity{
		id:         id,
		components: make([]Component, 32),

		pool: pool,
	}
}

// Получить "Уникальный индитификатор" (ID) "Сущности"
//
// id := player.ID()
func (e *entity) ID() uint64 {
	return e.id
}

// Добавить "Компонент" в "Сущность"
//
// 	player.Add(TRANSFORM, &Transform{20, 30}).
//		Add(VELOCITY, &Velocity{70, 10}).
//		Add(DIRECTION, &Direction{500, 0})
func (e *entity) Add(typeComponent ComponentType, component Component) Entity {
	e.components[typeComponent] = component
	e.pool.componentUpdate(e, typeComponent, e.components[typeComponent])
	return e
}

// Получить "Компонент" из "Сущности"
//
// 		transform := player.Get(TRANSFORM)
// 		transform.(*Transform).X = 30
func (e *entity) Get(typeComponent ComponentType) Component {
	return e.components[typeComponent]
}

// Проверить наличие "Компонента" в "Сущности"
//
// 		if player.Has(TRANSFORM) {
//			fmt.Println("yes")
// 		} else {
//			fmt.Println("np")
//		}
func (e *entity) has(componentTypes ...int) bool {
	for _, t := range componentTypes {
		if e.components[t] == nil {
			return false
		}
	}
	return true
}

// Удалить "Компонент" из "Сущности"
//
// 	player.Remove(TRANSFORM).
//		Remove(VELOCITY).
//		Remove(DIRECTION)
func (e *entity) Remove(typeComponent ComponentType) Entity {
	e.components[typeComponent] = nil
	e.pool.componentUpdate(e, typeComponent, e.components[typeComponent])
	return e
}

// Удалить "Сущность"
//
// 	player.Destroy()
func (e *entity) Destroy() {
	e.pool.destroyEntity(e)
}

// Удалить ВСЕ "Компоненты" из "Сущности"
func (e *entity) removeAll() {
	e.components = make([]Component, 32)
}
