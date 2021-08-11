package system

import (
	"github.com/Falldot/ce2d/internal/NewGame/Comp"
	"github.com/Falldot/ce2d/pkg/ecs"
)

// @Getter [TRANSLATE] -> Main
var TranslateGetter ecs.Getter = func(pool ecs.Pool) ecs.Group {
	return pool.GetGroup(ecs.AllOf(Comp.TRANSFORM))
}

// @Executer [TRANSLATE] -> Main
var TranslateExecuter ecs.Executer = func(pool ecs.Pool, group ecs.Group, dt float64) {
	group.ForEach(func(e ecs.Entity) {
		c := e.Get(Comp.TRANSFORM)
		c.(*Comp.Transform).Position.X += (c.(*Comp.Transform).Direction.X * c.(*Comp.Transform).CurrentSpeed) * dt
		c.(*Comp.Transform).Position.Y += (c.(*Comp.Transform).Direction.Y * c.(*Comp.Transform).CurrentSpeed) * dt
	})
}
