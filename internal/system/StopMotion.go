package system

import (
	"github.com/Falldot/ce2d/internal/component"
	"github.com/Falldot/ce2d/pkg/ecs"
	"github.com/veandco/go-sdl2/sdl"
)

type StopMotion struct {
	comps ecs.Entities
}

func (d *StopMotion) SetPool(pool *ecs.Pool) {
	d.comps = pool.GetEntities(component.SMANIMATION)
}

func (d *StopMotion) Execute() {
	for _, v := range d.comps {
		s := v.Get(component.SPRITE)
		sm := v.Get(component.SMANIMATION)

		s.(*component.Sprite).Src.X = s.(*component.Sprite).Src.W * (int32(sdl.GetTicks()) / sm.(*component.SMAnimation).Current.Speed % sm.(*component.SMAnimation).Current.Frames)
		s.(*component.Sprite).Src.Y = sm.(*component.SMAnimation).Current.Index * s.(*component.Sprite).Src.H
	}
}
