package Comp

import "github.com/Falldot/ce2d/pkg/ce2d_math"

// @Comp
type Transform struct {
	Position     *ce2d_math.Vector2D
	Direction    *ce2d_math.Vector2D
	Speed        float64
	CurrentSpeed float64
}

func CreateTransform(x float64, y float64, speed float64) *Transform {
	return &Transform{
		Position:     ce2d_math.CreateVector2D(x, y),
		Direction:    ce2d_math.CreateVector2D(0, 0),
		Speed:        speed,
		CurrentSpeed: speed,
	}
}
