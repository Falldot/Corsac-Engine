package system

import (
	"github.com/Falldot/ce2d/internal/NewGame/Comp"
	"github.com/Falldot/ce2d/pkg/ecs"
	"github.com/veandco/go-sdl2/sdl"
)

// @Getter [ANIMATED] -> Main
var AnimatedGetter ecs.Getter = func(pool ecs.Pool) ecs.Group {
	return pool.GetGroup(ecs.AllOf(Comp.ANIMATION))
}

// @Executer [ANIMATED] -> Main
var AnimatedExecuter ecs.Executer = func(pool ecs.Pool, group ecs.Group, dt float64) {
	group.ForEach(func(e ecs.Entity) {
		s := e.Get(Comp.SPRITE)
		sm := e.Get(Comp.ANIMATION)

		s.(*Comp.Sprite).Src.X = s.(*Comp.Sprite).Src.W * (int32(sdl.GetTicks()) / sm.(*Comp.Animation).Current.Speed % sm.(*Comp.Animation).Current.Frames)
		s.(*Comp.Sprite).Src.Y = sm.(*Comp.Animation).Current.Index * s.(*Comp.Sprite).Src.H
	})
}
