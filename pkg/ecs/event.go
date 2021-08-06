package ecs

import "sync/atomic"

type (
	TypeEvent int

	ListEvent map[uint32]*Event

	Event struct {
		id        uint32
		condition func(event *Event) bool
		actions   []func(event *Event)
		active    bool
	}
)

func (pool *Pool) RebornEvent(entity *Event) {
	pool.containerWithEvent[entity.id] = entity
}

func (pool *Pool) LazyRebornEvent(entity *Event) {
	pool.containerWithLazyEvent[entity.id] = entity
}

func (pool *Pool) CreateEvent(condition func(event *Event) bool, actions ...func(event *Event)) *Event {
	newEvent := &Event{id: atomic.AddUint32(&pool.idIncEvent, 1), condition: condition, actions: actions, active: true}
	pool.containerWithEvent[newEvent.id] = newEvent
	return newEvent
}

func (pool *Pool) LazyCreateEvent(condition func(event *Event) bool, actions ...func(event *Event)) *Event {
	newEvent := &Event{id: atomic.AddUint32(&pool.idIncEvent, 1), condition: condition, actions: actions, active: true}
	pool.containerWithLazyEvent[newEvent.id] = newEvent
	return newEvent
}

func (pool *Pool) addLazyEvent() {
	for i, v := range pool.containerWithLazyEvent {
		pool.containerWithEvent[i] = v
		delete(pool.containerWithLazyEvent, i)
	}
}

func (pool *Pool) ChangeEvents() {
	for _, e := range pool.containerWithEvent {
		if e.condition(e) {
			for _, a := range e.actions {
				a(e)
			}
		}
	}
	pool.addLazyEvent()
}

func (pool *Pool) DestroyEvent(event *Event) {
	delete(pool.containerWithEvent, event.id)
}

func (e *Event) GetCondition() func(event *Event) bool {
	return e.condition
}

func (e *Event) GetAtions() []func(event *Event) {
	return e.actions
}
