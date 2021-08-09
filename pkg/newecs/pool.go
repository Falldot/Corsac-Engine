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
		Cemetery() (int, int)
		Systems() int

		AddSystem(sys System)
		ExecuteSystems(dt float64)

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
		entitiesCache  []Entity
		entitiesUnused []Entity

		cacheComponents [][]Component

		groups      map[uint]Group
		groupsIndex map[int][]Group

		systems []System
	}
)

func CreatePool() Pool {
	return &pool{
		idIncEntity:     0,
		entities:        make(map[uint64]Entity),
		groups:          make(map[uint]Group),
		groupsIndex:     make(map[int][]Group),
		cacheComponents: make([][]Component, 32),
	}
}

// Создает новый экземпляр сущности
func (p *pool) CreateEntity() Entity {
	// Получаем экземпляр Сущности из пула сущностей
	e := p.getEntity()
	// Записываем сущности в хэш таблицу Сущностей
	p.entities[e.ID()] = e
	// Записываем сущности в массива закэшированных Сущностей
	if p.entitiesCache != nil {
		p.entitiesCache = append(p.entitiesCache, e)
	}

	for _, g := range p.groups {
		g.HandleEntity(e)
	}

	// Возвращаем Сущность
	return e
}

func (p *pool) AddSystem(sys System) {
	p.systems = append(p.systems, sys)
}

func (p *pool) ExecuteSystems(dt float64) {
	for _, v := range p.systems {
		v(p, dt)
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

// Получить кол-во неактивных сущностей
func (p *pool) Cemetery() (int, int) {
	return len(p.entitiesUnused), len(p.cacheComponents)
}

func (p *pool) HasEntity(e Entity) bool {
	entity, exist := p.entities[e.ID()]
	return exist && entity == e
}

func (p *pool) DestroyAllEntities() {
	p.entities = make(map[uint64]Entity)
	p.entitiesCache = nil
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
		g.HandleEntity(e)
	})
}

func (p *pool) destroyEntity(e Entity) {

	e.RemoveAll()

	delete(p.entities, e.ID())

	p.entitiesCache = nil
	for _, g := range p.groups {
		g.HandleEntity(e)
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
		g.HandleEntity(e)
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
