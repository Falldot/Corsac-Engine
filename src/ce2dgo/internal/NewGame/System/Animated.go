package system

import (
	"github.com/Falldot/ce2d/internal/NewGame/Comp"
	"github.com/Falldot/ce2d/pkg/ECS"
	"github.com/veandco/go-sdl2/sdl"
)

// @Getter [ANIMATED] -> GameController
var AnimatedGetter ECS.Getter = func() ECS.Group {
	return ECS.GetGroup(ECS.AllOf(Comp.ANIMATION))
}

// @Executer [ANIMATED] -> GameController
var AnimatedExecuter ECS.Executer = func(group ECS.Group, dt float64) {
	group.ForEach(func(e ECS.Entity) {
		s := e.Get(Comp.SPRITE)
		sm := e.Get(Comp.ANIMATION)

		s.(*Comp.Sprite).Src.X = s.(*Comp.Sprite).Src.W * (int32(sdl.GetTicks()) / sm.(*Comp.Animation).Current.Speed % sm.(*Comp.Animation).Current.Frames)
		s.(*Comp.Sprite).Src.Y = sm.(*Comp.Animation).Current.Index * s.(*Comp.Sprite).Src.H
	})
}
