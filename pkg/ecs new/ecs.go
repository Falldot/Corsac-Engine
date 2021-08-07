package ecs

import "fmt"

type TypeComponent int
type Component interface{}

type Pool struct {
	*system

	mapEntity map[TypeComponent]*Entity
}

func NewPool() *Pool {
	return &Pool{
		mapEntity: make(map[TypeComponent]*Entity),
	}
}

func (pool *Pool) RegComponent(typeComponent TypeComponent) {
	pool.mapEntity[typeComponent] = newListEntity()
}

func (pool *Pool) CreateEntity() *Entity {
	return &Entity{
		components: make(map[TypeComponent]Component),
	}
}

func (pool *Pool) AddComponent(e *Entity, typeComponent TypeComponent, componenent Component) {
	e.components[typeComponent] = componenent
	fmt.Println(e)
	e.Add()
	pool.mapEntity[typeComponent].Add(e)
}

func (pool *Pool) RemComponent(e *Entity, typeComponent TypeComponent) {
	delete(e.components, typeComponent)
	e.next = nil
	e.prev = nil
	pool.mapEntity[typeComponent].Remove()
}

func (pool *Pool) GetEntities(typeComponent TypeComponent) *Entity {
	return pool.mapEntity[typeComponent]
}

func (pool *Pool) RegSystem(sys ...ISystem) {
	pool.system = newSystem(sys)
}

func (pool *Pool) SetPoolSystems() {

	for nextList := pool.system; nextList != nil; nextList = nextList.Next() {
		nextList.Value.SetPool(pool)
	}
}

func (pool *Pool) ExecuteSystems() {
	for nextList := pool.system; nextList != nil; nextList = nextList.Next() {
		nextList.Value.Execute()
	}
}
