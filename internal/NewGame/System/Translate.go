package system

import (
	"github.com/Falldot/ce2d/internal/NewGame/Comp"
	"github.com/Falldot/ce2d/pkg/ECS"
)

// @Getter [TRANSLATE] -> GameController
var TranslateGetter ECS.Getter = func() ECS.Group {
	return ECS.GetGroup(ECS.AllOf(Comp.TRANSFORM))
}

// @Executer [TRANSLATE] -> GameController
var TranslateExecuter ECS.Executer = func(group ECS.Group, dt float64) {
	group.ForEach(func(e ECS.Entity) {
		c := e.Get(Comp.TRANSFORM)
		c.(*Comp.Transform).Position.X += (c.(*Comp.Transform).Direction.X * c.(*Comp.Transform).CurrentSpeed) * dt
		c.(*Comp.Transform).Position.Y += (c.(*Comp.Transform).Direction.Y * c.(*Comp.Transform).CurrentSpeed) * dt
	})
}
