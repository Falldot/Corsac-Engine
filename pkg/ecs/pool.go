package ecs

import (
	"sync/atomic"
)

type (
	Pool interface {
		/////////////////////////////////////////
		// public
		CreateEntity() Entity

		Entities() int
		Cemetery() int
		Systems() int

		Initers() int
		Cleaners() int
		Exiters() int
		Groups() int
		GroupsIndex() int

		AddSystem(actions ...interface{})

		Init()
		Execute(dt float64)
		Clean()
		Exit()

		DestroyAllEntities()

		GetGroup(matchers ...Matcher) Group

		/////////////////////////////////////////
		// private
		destroyEntity(e Entity)
		componentUpdate(e Entity, typeComponent ComponentType, c Component)
	}

	pool struct {
		idIncEntity uint64

		entities       map[uint64]Entity
		entitiesUnused []Entity

		groups      map[uint]Group
		groupsIndex map[int][]Group

		initers  []Initer
		cleaners []Cleaner
		exiters  []Exiter

		systems []*system
	}
)

func CreatePool() Pool {
	return &pool{
		idIncEntity: 0,
		entities:    make(map[uint64]Entity),
		groups:      make(map[uint]Group),
		groupsIndex: make(map[int][]Group),
	}
}

// Создает новый экземпляр сущности
func (p *pool) CreateEntity() Entity {
	// Получаем экземпляр Сущности из пула сущностей
	e := p.getEntity()
	// Записываем сущности в хэш таблицу Сущностей
	p.entities[e.ID()] = e

	for _, g := range p.groups {
		g.handleEntity(e)
	}

	// Возвращаем Сущность
	return e
}

func (p *pool) AddSystem(actions ...interface{}) {
	newSystem := createSystem()

	for _, a := range actions {
		switch a.(type) {
		case Initer:
			p.initers = append(p.initers, a.(Initer))
		case Cleaner:
			p.cleaners = append(p.cleaners, a.(Cleaner))
		case Exiter:
			p.exiters = append(p.exiters, a.(Exiter))

		case Trigger:
			newSystem.trigger = a.(Trigger)
		case Filter:
			newSystem.filter = a.(Filter)
		case Getter:
			newSystem.getter = a.(Getter)
		case Executer:
			newSystem.executer = a.(Executer)
		}
	}

	if newSystem.executer != nil {
		p.systems = append(p.systems, newSystem)
	}
}

func (p *pool) Init() {
	for _, v := range p.initers {
		v(p)
	}
}

func (p *pool) Execute(dt float64) {
	for _, sys := range p.systems {
		group := sys.getter(p)
		group = group.Filter(func(e Entity) bool {
			return sys.filter(e)
		})
		if sys.trigger(p) {
			sys.executer(p, group, dt)
		}
	}
}

func (p *pool) Clean() {
	for _, v := range p.cleaners {
		v(p)
	}
}

func (p *pool) Exit() {
	for _, v := range p.exiters {
		v(p)
	}
}

// Получить кол-во активных сущностей
func (p *pool) Entities() int {
	return len(p.entities)
}

// Получить кол-во активных сущностей
func (p *pool) Systems() int {
	return len(p.systems)
}

func (p *pool) Initers() int {
	return len(p.initers)
}

func (p *pool) Cleaners() int {
	return len(p.cleaners)
}

func (p *pool) Exiters() int {
	return len(p.exiters)
}

func (p *pool) Groups() int {
	return len(p.groups)
}

func (p *pool) GroupsIndex() int {
	return len(p.groupsIndex)
}

// Получить кол-во неактивных сущностей
func (p *pool) Cemetery() int {
	return len(p.entitiesUnused)
}

func (p *pool) HasEntity(e Entity) bool {
	entity, exist := p.entities[e.ID()]
	return exist && entity == e
}

func (p *pool) DestroyAllEntities() {
	p.entities = make(map[uint64]Entity)
}

// Получаем экземпляр Сущности
// Создаем новый или переиспользуем удаленный (неиспользуемый)
func (p *pool) getEntity() Entity {
	var newEntity Entity

	lenghtEntitiesUnused := len(p.entitiesUnused)

	// Если мы имеет удаленные (неиспользуемые) Сущности
	// то берем новый экземпляр из их контейнера
	// иначе создаем новый экземпляр
	if lenghtEntitiesUnused > 0 {
		last := lenghtEntitiesUnused - 1
		newEntity = p.entitiesUnused[last]
		p.entitiesUnused = p.entitiesUnused[:last]
	} else {
		newEntity = createEntity(p, p.idIncEntity)
		atomic.AddUint64(&p.idIncEntity, 1)
	}

	return newEntity
}

func (p *pool) componentUpdate(e Entity, typeComponent ComponentType, c Component) {
	p.forMatchingGroup(e, typeComponent, c, func(g Group) {
		g.handleEntity(e)
	})
}

func (p *pool) destroyEntity(e Entity) {

	e.removeAll()

	delete(p.entities, e.ID())

	for _, g := range p.groups {
		g.handleEntity(e)
	}

	p.entitiesUnused = append(p.entitiesUnused, e)
}

func (p *pool) GetGroup(matchers ...Matcher) Group {
	hash := HashMatcher(matchers...)

	if g, ok := p.groups[hash]; ok {
		return g
	}

	g := newGroup(matchers...)
	for _, e := range p.entities {
		g.handleEntity(e)
	}
	p.groups[hash] = g

	for _, m := range matchers {
		for _, t := range m.ComponentTypes() {
			p.groupsIndex[t] = append(p.groupsIndex[t], g)
		}
	}

	return g
}

func (p *pool) forMatchingGroup(e Entity, componentType ComponentType, c Component, f func(g Group)) {
	if p.HasEntity(e) {
		for _, g := range p.groupsIndex[int(componentType)] {
			f(g)
		}
	}
}
