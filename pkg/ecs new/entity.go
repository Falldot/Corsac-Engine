package ecs

/*
	Реализация односвязного списка для "Систем".

	Критерии списка:
	* нет необходимости в удалении и фильтрации систем;
	* добавление системы только в начало списка нас удовлетворяет;
	* нам необходимо поочередно выполнить методы систем без какой
	  либо иной манипуляции с ними.
*/

type (
	Entity struct {
		next, prev *Entity

		components map[TypeComponent]Component
	}
)

func newListEntity() *Entity {
	return &Entity{
		next:       nil,
		prev:       nil,
		components: make(map[TypeComponent]Component),
	}
}

func (e *Entity) GetComponent(typeComponent TypeComponent) Component {
	return e.components[typeComponent]
}

func (e *Entity) onward() *Entity {
	return e.next
}

func (e *Entity) Map(do func(e *Entity)) {
	for entity := e; entity != nil; entity = entity.onward() {
		do(entity)
	}
}

func (e *Entity) Add(en *Entity) {
	// fmt.Println("_______________________________")
	// fmt.Println(e.next == en.next, "|", e.next, "|", en.next)
	// fmt.Println(en.prev == e, "|", en.prev, "|", e)
	// fmt.Println(e.next == en, "|", e.next, "|", en)
	en.next = e.next
	en.prev = e
	e.next = en
}

// 1 -> 3 -> 2

func (e *Entity) Remove() {
	e.prev.next = e.next
	e.next.prev = e.prev
}

func (e *Entity) Lenght() int {
	i := 0
	for list := e; list != nil; list = list.next {
		i++
	}
	return i
}
