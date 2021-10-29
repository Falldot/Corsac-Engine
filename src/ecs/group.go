package ecs

type Group struct {
	packed []Entity
	sparse []Entity
}

func CreateGroup() *Group {
	return &Group{
		sparse: make([]Entity, 2),
	}
}

func MakeGroup(length int) *Group {
	return &Group{
		sparse: make([]Entity, length),
		packed: make([]Entity, length),
	}
}

func (group *Group) Has(entity Entity) bool {
	return group.sparse[entity] < (Entity)(len(group.packed)) && group.packed[group.sparse[entity]] == entity
}

func (group *Group) Add(entity Entity) {
	if int(entity) >= len(group.sparse) {
		group.sparse = append(group.sparse, make([]Entity, entity*2)...)
	}
	if group.Has(entity) {
		panic("Invalid operation! This entity already exist!")
	}
	group.sparse[entity] = (Entity)(len(group.packed))
	group.packed = append(group.packed, entity)
}

func (group *Group) Remove(entity Entity) {
	if group.Has(entity) {
		panic("Invalid operation! This entity not exist!")
	}
	index := group.sparse[entity]

	last := len(group.packed) - 1
	swap := group.packed[last]
	group.packed[last] = 0

	if swap != entity {
		group.packed[index] = swap
		group.sparse[swap] = index
	}
}

func (group *Group) Clear() {
	group.packed = group.packed[:0]
}
