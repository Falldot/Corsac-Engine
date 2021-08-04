package ce2d_math

import (
	"math"
)

func RadianToAngle(radian float64) int16 {
	return int16(radian * (180 / math.Pi))
}

func AngleToRadian(angle int16) float64 {
	return float64(angle) * (math.Pi / 180)
}

func lenght(x, y float64) float64 {
	return math.Pow((math.Pow(x, 2) + math.Pow(y, 2)), 0.5)
}
