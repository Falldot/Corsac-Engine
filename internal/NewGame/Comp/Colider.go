package Comp

import (
	"github.com/Falldot/ce2d/pkg/ce2d_collision"
	"github.com/Falldot/ce2d/pkg/ce2d_math"
)

// @Comp
type Colider struct {
	MinPosition, MaxPosition *ce2d_math.Point
	callback                 func(self, other *Colider)
}

func CreateColider(x, y, width, height int32, callback func(self, other *Colider)) *Colider {
	return &Colider{
		MinPosition: ce2d_math.CreatePoint(x, y),
		MaxPosition: ce2d_math.CreatePoint(x+width, y+height),
		callback:    callback,
	}
}

func (a *Colider) AddCallBack(callback func(self, other *Colider)) {
	a.callback = callback
}

func (a *Colider) Chek(b *Colider) {
	if ce2d_collision.DividingAxisCheck(a.MinPosition, a.MaxPosition, b.MinPosition, b.MaxPosition) {
		a.callback(a, b)
		b.callback(b, a)
	}
}

func (a *Colider) SetX(value int32) {
	width := a.MaxPosition.X - a.MinPosition.X
	a.MinPosition.X = value
	a.MaxPosition.X = value + width
}

func (a *Colider) SetY(value int32) {
	height := a.MaxPosition.Y - a.MinPosition.Y
	a.MinPosition.Y = value
	a.MaxPosition.Y = value + height
}
