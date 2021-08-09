package ecs

type (
	Group interface {
		Entities() []Entity
		HandleEntity(e Entity)
		Matches(e Entity) bool
	}

	group struct {
		entities map[uint64]Entity

		cache    []Entity
		matchers []Matcher
	}
)

func newGroup(matchers ...Matcher) Group {
	return &group{
		entities: make(map[uint64]Entity),
		matchers: matchers,
	}
}

// Получить ВСЕ "Сущности" в "Группы" (кэшируется)
//
//	tiles := tilesGroup.Entities()
func (g *group) Entities() []Entity {
	cache := g.cache
	if cache == nil {
		cache = make([]Entity, 0, len(g.entities))

		for _, v := range g.entities {
			cache = append(cache, v)
		}

		g.cache = cache
	}
	return cache
}

func (g *group) HandleEntity(e Entity) {
	if g.Matches(e) {
		g.addEntity(e)
	} else {
		g.removeEntity(e)
	}
}

func (g *group) Matches(e Entity) bool {
	for _, m := range g.matchers {
		if !m.Matches(e) {
			return false
		}
	}
	return true
}

// Добавить "Сущность" в "Группу" (кэшируется)
func (g *group) addEntity(e Entity) {
	g.entities[e.ID()] = e
	if g.cache != nil {
		g.cache = append(g.cache, e)
	}
}

// Удалить "Сущность" из "Группы"
func (g *group) removeEntity(e Entity) {
	delete(g.entities, e.ID())
	g.cache = nil
}
