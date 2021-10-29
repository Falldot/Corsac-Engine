package ce2d_collision

import (
	"github.com/Falldot/ce2d/pkg/ce2d_math"
)

func DividingAxisCheck(aMinPoint, aMaxPoint, bMinPoint, bMaxPoint *ce2d_math.Point) bool {
	if aMaxPoint.X < bMinPoint.X || aMinPoint.X > bMaxPoint.X {
		return false
	}
	if aMaxPoint.Y < bMinPoint.Y || aMinPoint.Y > bMaxPoint.Y {
		return false
	}
	return true
}
