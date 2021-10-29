package ECS

import "github.com/Falldot/ce2d/pkg/Math/Container"

type Group interface {
	Entities() Container.List
}

type group struct {
	entities      map[uint64]Entity
	entitiesCache Container.List
	matchers      Container.List

	lenghtEntitiesCashe int
}

func createGroup() Group {
	return &group{
		entities:      make(map[uint64]Entity),
		entitiesCache: Container.NewList(),
		matchers:      Container.NewList(),
	}
}

func (g *group) Entities() Container.List {
	if !g.entitiesCache.Empty() {
		return g.entitiesCache
	}
	for _, e := range g.entities {
		g.entitiesCache.Add(e)
		g.lenghtEntitiesCashe++
	}
	return g.entitiesCache
}

func (g *group) ForEach(callback func(e Entity)) {
	g.Entities().ForEach(func(l Container.List) {
		callback(l.Value().(Entity))
	})
}

func (g *group) handleEntity(e Entity) {
	if g.matches(e) {
		g.addEntity(e)
	} else {
		g.removeEntity(e)
	}
}

func (g *group) matches(e Entity) bool {
	handle := true
	g.matchers.ForEach(func(l Container.List) {
		if !l.Value().(Matcher).Matches(e) {
			handle = false
		}
	})
	return handle
}

// Добавить "Сущность" в "Группу" (кэшируется)
func (g *group) addEntity(e Entity) {
	g.entities[e.ID()] = e
	if !g.entitiesCache.Empty() {
		g.entitiesCache.Add(e)
		g.lenghtEntitiesCashe++
	}
}

// Удалить "Сущность" из "Группы"
func (g *group) removeEntity(e Entity) {
	delete(g.entities, e.ID())
	g.entitiesCache.Clear()
}
