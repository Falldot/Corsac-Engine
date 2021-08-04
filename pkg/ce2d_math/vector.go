package ce2d_math

import (
	"math"
)

type Vector2D struct {
	X float64
	Y float64

	lenght float64
}

func CreateVector2D(x, y float64) *Vector2D {
	return &Vector2D{
		X:      x,
		Y:      y,
		lenght: lenght(x, y),
	}
}

func (v *Vector2D) Set(x, y float64) {
	v.X = x
	v.Y = y
	v.lenght = lenght(x, y)
}

func (v *Vector2D) Normilize() *Vector2D {
	if v.lenght == 0 {
		return CreateVector2D(0, 0)
	} else {
		return CreateVector2D(v.X/v.lenght, v.Y/v.lenght)
	}

}

func (v *Vector2D) Rotate(angle int16) {
	radian := float64(angle) * (math.Pi / 180)
	x := v.X*math.Cos(radian) - v.Y*math.Sin(radian)
	y := v.X*math.Sin(radian) + v.Y*math.Cos(radian)
	v.X = x
	v.Y = y
}

func (v *Vector2D) Compare(other *Vector2D) bool {
	return v.X == other.X && v.Y == other.Y
}

func (v *Vector2D) Sub(other *Vector2D) *Vector2D {
	return CreateVector2D(v.X-other.X, v.Y-other.Y)
}

func (v *Vector2D) Scal(other *Vector2D) float64 {
	return v.X*other.X + v.Y*other.Y
}

func (v *Vector2D) Angle() int16 {
	return int16(RadianToAngle(math.Atan2(v.Y, v.X)))
}
