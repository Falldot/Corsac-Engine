package system

import (
	"github.com/Falldot/ce2d/internal/component"
	"github.com/Falldot/ce2d/pkg/ecs"
	"github.com/Falldot/ce2d/pkg/gdl"
)

type Draw struct {
	comps ecs.Entities
}

func (d *Draw) SetPool(pool *ecs.Pool) {
	d.comps = pool.GetEntities(component.SPRITE)
}

func (d *Draw) Execute() {
	for _, v := range d.comps {
		c := v.Get(component.TRANSFORM)
		s := v.Get(component.SPRITE)

		s.(*component.Sprite).Dst.X = int32(c.(*component.Transform).Position.X)
		s.(*component.Sprite).Dst.Y = int32(c.(*component.Transform).Position.Y)

		err := gdl.Render.CopyEx(s.(*component.Sprite).Tex, s.(*component.Sprite).Src, s.(*component.Sprite).Dst, s.(*component.Sprite).Angle, nil, s.(*component.Sprite).Flip)
		if err != nil {
			panic(err)
		}
	}
}
