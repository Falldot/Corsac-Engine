package ce2d_collision

import (
	"github.com/Falldot/ce2d/pkg/ce2d_math"
)

type AABB struct {
	Position      *ce2d_math.Vector2D
	Width, Height int
	callback      func(self, other *AABB)
	condition     func(other *AABB) bool
}

func CreateAABB(x, y float64, width, height int, callback func(self, other *AABB), condition func(other *AABB) bool) *AABB {
	return &AABB{
		Position: ce2d_math.CreateVector2D(x, y),
		Width:    width,
		Height:   height,
		callback: callback,
	}
}

func (aabb *AABB) chek(other *AABB) bool {
	return aabb.condition(other)
}
