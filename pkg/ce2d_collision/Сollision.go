package ce2d_collision

import (
	"github.com/Falldot/ce2d/pkg/ce2d_math"
)

type AABB struct {
	MinPosition, MaxPosition *ce2d_math.Vector2D
	callback                 func(self, other *AABB)
}

func CreateColider(x, y, width, height float64, callback func(self, other *AABB)) *AABB {
	return &AABB{
		MinPosition: ce2d_math.CreateVector2D(x, y),
		MaxPosition: ce2d_math.CreateVector2D(x+width, y+height),
		callback:    callback,
	}
}

func dividingAxisCheck(aMinPoint, aMaxPoint, bMinPoint, bMaxPoint *ce2d_math.Vector2D) bool {
	return aMaxPoint.X < bMinPoint.X ||
		aMinPoint.X > bMaxPoint.X ||
		aMaxPoint.Y < bMinPoint.Y ||
		aMinPoint.Y > bMaxPoint.Y
}

func (a *AABB) chek(b *AABB) {
	if dividingAxisCheck(a.MinPosition, a.MaxPosition, b.MinPosition, b.MaxPosition) {
		a.callback(a, b)
		b.callback(a, b)
	}
}
