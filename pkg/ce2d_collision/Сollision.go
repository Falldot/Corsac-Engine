package ce2d_collision

import (
	"github.com/Falldot/ce2d/pkg/ce2d_math"
)

type AABB struct {
	Position      *ce2d_math.Vector2D
	Width, Height int
}

func CreateAABB(x, y float64, width, height int) *AABB {
	return &AABB{
		Position: ce2d_math.CreateVector2D(x, y),
		Width:    width,
		Height:   height,
	}
}

func (aabb *AABB) chek(AABB) bool {
	return true
}
