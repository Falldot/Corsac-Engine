package ce2d_ecs

import (
	"github.com/Falldot/ce2d/pkg/ce2d_math"
	"github.com/Falldot/ce2d/pkg/ecs"
)

type (
	Transform struct {
		Position     *ce2d_math.Vector2D
		Direction    *ce2d_math.Vector2D
		Speed        float64
		CurrentSpeed float64
	}

	sysTransform struct {
		comps ecs.Entities
	}
)

func CreateTransform(x float64, y float64, speed float64) *Transform {
	return &Transform{
		Position:     ce2d_math.CreateVector2D(x, y),
		Direction:    ce2d_math.CreateVector2D(0, 0),
		Speed:        speed,
		CurrentSpeed: speed,
	}
}

func (sys *sysTransform) Init(pool *ecs.Pool) {
	sys.comps = pool.GetEntities(TRANSFORM)
}

func (sys *sysTransform) Update(dt float64) {
	for _, v := range sys.comps {
		c := v.Get(TRANSFORM)
		c.(*Transform).Position.X += (c.(*Transform).Direction.X * c.(*Transform).CurrentSpeed) * dt
		c.(*Transform).Position.Y += (c.(*Transform).Direction.Y * c.(*Transform).CurrentSpeed) * dt
	}
}
