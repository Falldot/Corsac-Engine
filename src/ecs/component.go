package ecs

type IComponent struct {
	packed []Entity
	sparse []Entity
	Data   []interface{}
}

func Component() *IComponent {
	return &IComponent{
		sparse: make([]Entity, 2),
	}
}

func MakeComponent(length int) *IComponent {
	return &IComponent{
		sparse: make([]Entity, length),
		packed: make([]Entity, 0, length),
		Data:   make([]interface{}, 0, length),
	}
}

func (component *IComponent) Clear() {
	component.packed = component.packed[:0]
	component.Data = component.Data[:0]
}
