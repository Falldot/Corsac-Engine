package ECS

type Entity interface {

	// PUBLIC:

	ID() uint64
	Destroy()

	Add(ComponentType, Component) Entity
	Get(ComponentType) Component
	Has(...ComponentType) bool
	Remove(...ComponentType) Entity

	// PRIVATE:

	removeAllComponents()
}

type entity struct {
	id uint64

	components []Component
}

// Конструктор "Сущности"
func createEntity(id uint64) Entity {
	return &entity{
		id:         id,
		components: make([]Component, maxLenghtComponentInEntity),
	}
}

// Получить "Уникальный индитификатор" (ID) "Сущности"
//
// id := player.ID()
func (e *entity) ID() uint64 {
	return e.id
}

// Удалить "Сущность"
//
// 	player.Destroy()
func (e *entity) Destroy() {
	destroyEntity(e)
}

// Добавить "Компонент" в "Сущность"
//
// 	player.Add(TRANSFORM, &Transform{20, 30}).
//		Add(VELOCITY, &Velocity{70, 10}).
//		Add(DIRECTION, &Direction{500, 0})
func (e *entity) Add(componentType ComponentType, component Component) Entity {
	e.components = append(e.components, component)
	// e.pool.componentUpdate(e, typeComponent, e.components[typeComponent])
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
func (e *entity) Has(componentTypes ...ComponentType) bool {
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
func (e *entity) Remove(typeComponent ...ComponentType) Entity {
	for _, t := range typeComponent {
		e.components[t] = nil
		// e.pool.componentUpdate(e, typeComponent, e.components[typeComponent])
	}
	return e
}

// Удалить ВСЕ "Компоненты" из "Сущности"
func (e *entity) removeAllComponents() {
	e.components = make([]Component, maxLenghtComponentInEntity)
}
