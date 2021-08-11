package system

import (
	"github.com/Falldot/ce2d/internal/NewGame/Comp"
	"github.com/Falldot/ce2d/pkg/ecs"
	"github.com/Falldot/ce2d/pkg/rm"
)

// @Getter [RENDERER] -> Main
var RendererGetter ecs.Getter = func(pool ecs.Pool) ecs.Group {
	return pool.GetGroup(ecs.AllOf(Comp.SPRITE))
}

// @Executer [RENDERER] -> Main
var RendererExecuter ecs.Executer = func(pool ecs.Pool, group ecs.Group, dt float64) {
	group.ForEach(func(e ecs.Entity) {
		c := e.Get(Comp.TRANSFORM)
		s := e.Get(Comp.SPRITE)

		s.(*Comp.Sprite).Dst.X = int32(c.(*Comp.Transform).Position.X)
		s.(*Comp.Sprite).Dst.Y = int32(c.(*Comp.Transform).Position.Y)

		err := rm.Renderer.CopyEx(s.(*Comp.Sprite).Tex, s.(*Comp.Sprite).Src, s.(*Comp.Sprite).Dst, s.(*Comp.Sprite).Angle, nil, s.(*Comp.Sprite).Flip)
		if err != nil {
			panic(err)
		}
	})
}
