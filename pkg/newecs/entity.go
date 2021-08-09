package ecs

type (
	// Интерфейс реализации Сущности
	Entity interface {

		// Entity methods

		ID() uint64
		Destroy()

		// Components methods

		Add(ComponentType, Component) Entity
		Get(ComponentType) Component
		Remove(ComponentType) Entity
		RemoveAll()

		has(...int) bool
	}

	entity struct {
		id uint64

		components []Component

		componentsCache    []Component
		componentTypeCache []ComponentType

		pool Pool
	}
)

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
	e.pool.componentUpdate(e, typeComponent, e.components[typeComponent])
	e.components[typeComponent] = component
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
	e.pool.componentUpdate(e, typeComponent, e.components[typeComponent])
	e.components[typeComponent] = nil
	return e
}

// Удалить ВСЕ "Компоненты" из "Сущности"
//
// 	player.RemoveAllComponents()
func (e *entity) RemoveAll() {
	e.components = make([]Component, 32)
}

// Удалить "Сущность"
//
// 	player.Destroy()
func (e *entity) Destroy() {
	e.pool.destroyEntity(e)
}
