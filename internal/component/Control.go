package component

import "github.com/Falldot/ce2d/pkg/ce2d_math"

type Control struct {
	Route *ce2d_math.Vector2D
}

func CreateControl(x, y float64) *Control {
	return &Control{
		Route: ce2d_math.CreateVector2D(x, y),
	}
}

// type WASDMove struct {
// 	W, A, S, D bool
// }
