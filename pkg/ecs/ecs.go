package ecs

import (
	"sync/atomic"
)

type (
	TypeComponent int
	Component     interface{}

	Entities map[uint32]*Entity

	ListSystem   []System
	MapEntity    map[TypeComponent]Entities
	MapComponent map[TypeComponent]Component

	Pool struct {
		idIncEntity            uint32
		idIncEvent             uint32
		containerWithSystem    ListSystem
		containerWithEntity    MapEntity
		containerWithEvent     ListEvent
		containerWithLazyEvent ListEvent
	}

	Entity struct {
		id         uint32
		components MapComponent
	}

	System interface {
		SetPool(pool *Pool)
		Execute()
	}
)

func CreatePool() *Pool {
	return &Pool{
		containerWithEntity:    make(MapEntity),
		containerWithEvent:     make(ListEvent),
		containerWithLazyEvent: make(ListEvent),
	}
}

func (pool *Pool) LenghtData() (int, int, int) {
	s := len(pool.containerWithSystem)
	e := int(pool.idIncEntity)
	c := len(pool.containerWithEvent)
	return s, e, c
}

func (pool *Pool) CreateEntity() *Entity {
	newEntity := &Entity{id: atomic.AddUint32(&pool.idIncEntity, 1), components: make(MapComponent)}
	return newEntity
}

func (pool *Pool) RegComponent(typeComponent ...TypeComponent) {
	for _, v := range typeComponent {
		pool.containerWithEntity[v] = map[uint32]*Entity{}
	}
}

func (pool *Pool) AddComponent(e *Entity, typeComponent TypeComponent, component Component) {
	e.components[typeComponent] = component
	pool.containerWithEntity[typeComponent][e.id] = e
}

func (pool *Pool) GetEntities(typeComponent TypeComponent) map[uint32]*Entity {
	return pool.containerWithEntity[typeComponent]
}

func (pool *Pool) RemoveComponent(e *Entity, typeComponent TypeComponent) {
	delete(e.components, typeComponent)
	delete(pool.containerWithEntity[typeComponent], e.id)
}

func (pool *Pool) RemoveEntity(e *Entity) {
	for i := range e.components {
		delete(pool.containerWithEntity[i], e.id)
	}
}

func (e *Entity) Has(typeComponent TypeComponent) bool {
	return e.components[typeComponent] != nil
}

func (e *Entity) Get(typeComponent TypeComponent) Component {
	return e.components[typeComponent]
}

func (e *Entity) Replace(typeComponent TypeComponent, newComponent Component) {
	e.components[typeComponent] = newComponent
}

func (pool *Pool) SetSystem(syss ...System) {
	pool.containerWithSystem = append(pool.containerWithSystem, syss...)
}

func (pool *Pool) SetPoolSystems() {
	for _, v := range pool.containerWithSystem {
		v.SetPool(pool)
	}
}
