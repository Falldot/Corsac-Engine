package ecs

type Entity uint32

var EntityCurrentCount Entity
var EntityCurrentPool []Entity

func Reload() {
	EntityCurrentCount = 0
}

func GameObject(systems ...System) Entity {
	if EntityCurrentPool != nil {
		last := len(EntityCurrentPool) - 1
		entity := EntityCurrentPool[last]
		EntityCurrentPool = EntityCurrentPool[:last]
		return entity
	}
	EntityCurrentCount++
	for _, sys := range systems {
		sys.group.Add(EntityCurrentCount)
	}
	return EntityCurrentCount
}

func (entity *Entity) ID() uint32 {
	return uint32(*entity)
}

func (entity *Entity) Has(component *IComponent) bool {
	return component.sparse[*entity] < (Entity)(len(component.packed)) && component.packed[component.sparse[*entity]] == *entity
}

func (entity *Entity) Add(component *IComponent, data interface{}) *Entity {
	if int(*entity) >= len(component.sparse) {
		component.sparse = append(component.sparse, make([]Entity, *entity*2)...)
	}

	if entity.Has(component) {
		panic("Invalid operation! This entity already exist!")
	}
	component.sparse[*entity] = (Entity)(len(component.packed))
	component.packed = append(component.packed, *entity)
	component.Data = append(component.Data, data)
	return entity
}

func (entity *Entity) Remove(component *IComponent) {
	if !entity.Has(component) {
		panic("Invalid operation! This entity not exist!")
	}
	index := component.sparse[*entity]

	last := len(component.packed) - 1
	swap := component.packed[last]
	swapCmp := component.Data[last]
	component.packed = component.packed[:last]
	component.Data = component.Data[:last]

	if swap != *entity {
		component.packed[index] = swap
		component.Data[index] = swapCmp
		component.sparse[swap] = index
	}
}

func (entity *Entity) Leave(systems ...System) {
	for _, sys := range systems {
		sys.group.Remove(*entity)
	}
}

func (entity *Entity) Join(systems ...System) {
	for _, sys := range systems {
		sys.group.Add(*entity)
	}
}

func (entity *Entity) Get(component *IComponent) interface{} {
	return component.Data[component.sparse[*entity]]
}

func (entity *Entity) Take(component *IComponent) *interface{} {
	return &component.Data[component.sparse[*entity]]
}
