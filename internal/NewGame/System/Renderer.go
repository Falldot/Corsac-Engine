package system

import (
	"github.com/Falldot/ce2d/internal/NewGame/Comp"
	"github.com/Falldot/ce2d/pkg/ECS"
	"github.com/Falldot/ce2d/pkg/GUI"
)

// @Getter [RENDERER] -> GameController
var RendererGetter ECS.Getter = func() ECS.Group {
	return ECS.GetGroup(ECS.AllOf(Comp.SPRITE))
}

// @Executer [RENDERER] -> GameController
var RendererExecuter ECS.Executer = func(group ECS.Group, dt float64) {
	group.ForEach(func(e ECS.Entity) {
		c := e.Get(Comp.TRANSFORM)
		s := e.Get(Comp.SPRITE)

		s.(*Comp.Sprite).Dst.X = int32(c.(*Comp.Transform).Position.X)
		s.(*Comp.Sprite).Dst.Y = int32(c.(*Comp.Transform).Position.Y)

		err := GUI.Renderer.CopyEx(s.(*Comp.Sprite).Tex, s.(*Comp.Sprite).Src, s.(*Comp.Sprite).Dst, s.(*Comp.Sprite).Angle, nil, s.(*Comp.Sprite).Flip)
		if err != nil {
			panic(err)
		}
	})
}
