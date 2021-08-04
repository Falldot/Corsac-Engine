package system

import (
	"github.com/Falldot/ce2d/internal/component"
	"github.com/Falldot/ce2d/pkg/ecs"
)

type Translate struct {
	comps ecs.Entities
}

func (t *Translate) SetPool(pool *ecs.Pool) {
	t.comps = pool.GetEntities(component.TRANSFORM)
}

func (t *Translate) Execute() {
	for _, v := range t.comps {
		c := v.Get(component.TRANSFORM)
		c.(*component.Transform).Position.X += c.(*component.Transform).Direction.X * c.(*component.Transform).Speed
		c.(*component.Transform).Position.Y += c.(*component.Transform).Direction.Y * c.(*component.Transform).Speed
	}
}
