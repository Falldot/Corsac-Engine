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
	system struct {
		next  *system
		Value ISystem
	}

	ISystem interface {
		SetPool(pool *Pool)
		Execute()
	}
)

func newSystem(sys []ISystem) *system {
	root := &system{
		next:  nil,
		Value: sys[0],
	}

	for i := 1; i < len(sys); i++ {
		root.push(sys[i])
	}

	return root
}

func (e *system) Next() *system {
	return e.next
}

func (s *system) push(sys ISystem) *system {
	newList := &system{
		next:  s.next,
		Value: sys,
	}
	s.next = newList
	return newList
}
