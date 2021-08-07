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
		idIncEntity uint32
		idIncEvent  uint32

		*listSystem

		countSystems int

		containerWithEntity    MapEntity
		containerWithEvent     ListEvent
		containerWithLazyEvent ListEvent
	}

	Entity struct {
		id         uint32
		components MapComponent
	}
)

func CreatePool(sys ...System) *Pool {

	listSystem, countSystems := newListSystem(sys)

	return &Pool{
		listSystem:             listSystem,
		countSystems:           countSystems,
		containerWithEntity:    make(MapEntity),
		containerWithEvent:     make(ListEvent),
		containerWithLazyEvent: make(ListEvent),
	}
}

func (pool *Pool) LenghtData() (int, int, int) {
	s := pool.countSystems
	e := int(pool.idIncEntity)
	c := len(pool.containerWithEvent)
	return s, e, c
}

func (pool *Pool) CreateEntity() *Entity {
	newEntity := &Entity{id: atomic.AddUint32(&pool.idIncEntity, 1), components: make(MapComponent)}
	return newEntity
}

func (pool *Pool) RegComponent(typeComponent []TypeComponent) {
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

func (pool *Pool) Init() {
	pool.listSystem.Init(pool)
}

func (pool *Pool) Update(dt float64) {
	pool.listSystem.Update(dt)
}

//////////////////////////////////////////////////////////////////
// Односвязный список систем
//////////////////////////////////////////////////////////////////

type (
	listSystem struct {
		next *listSystem
		System
	}

	System interface {
		Init(pool *Pool)
		Update(dt float64)
	}
)

func newSystem(next *listSystem, sys System) *listSystem {
	return &listSystem{
		next:   next,
		System: sys,
	}
}

func newListSystem(sys []System) (*listSystem, int) {
	len := len(sys)

	root := newSystem(nil, sys[0])

	for i := 1; i < len; i++ {
		root.push(sys[i])
	}

	return root, len
}

func (list *listSystem) Next() *listSystem {
	return list.next
}

func (list *listSystem) push(sys System) *listSystem {
	newList := newSystem(list.next, sys)
	list.next = newList
	return newList
}

func (list *listSystem) Init(pool *Pool) {
	for nextList := list; nextList != nil; nextList = nextList.Next() {
		nextList.System.Init(pool)

	}
}

func (list *listSystem) Update(dt float64) {
	for nextList := list; nextList != nil; nextList = nextList.Next() {
		nextList.System.Update(dt)
	}
}
