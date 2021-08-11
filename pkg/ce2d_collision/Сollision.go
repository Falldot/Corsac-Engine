package ce2d_collision

import (
	"github.com/Falldot/ce2d/pkg/ce2d_math"
)

type AABB struct {
	MinPosition, MaxPosition *ce2d_math.Point
	callback                 func(self, other *AABB)
}

func CreateColider(x, y, width, height int32, callback func(self, other *AABB)) *AABB {
	return &AABB{
		MinPosition: ce2d_math.CreatePoint(x, y),
		MaxPosition: ce2d_math.CreatePoint(x+width, y+height),
		callback:    callback,
	}
}

func dividingAxisCheck(aMinPoint, aMaxPoint, bMinPoint, bMaxPoint *ce2d_math.Point) bool {
	if aMaxPoint.X < bMinPoint.X || aMinPoint.X > bMaxPoint.X {
		return false
	}
	if aMaxPoint.Y < bMinPoint.Y || aMinPoint.Y > bMaxPoint.Y {
		return false
	}
	return true
}

func (a *AABB) chek(b *AABB) {
	if dividingAxisCheck(a.MinPosition, a.MaxPosition, b.MinPosition, b.MaxPosition) {
		a.callback(a, b)
		b.callback(b, a)
	}
}

func (a *AABB) SetX(value int32) {
	width := a.MaxPosition.X - a.MinPosition.X
	a.MinPosition.X = value
	a.MaxPosition.X = value + width
}

func (a *AABB) SetY(value int32) {
	height := a.MaxPosition.Y - a.MinPosition.Y
	a.MinPosition.Y = value
	a.MaxPosition.Y = value + height
}
