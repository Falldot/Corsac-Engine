package ce2d_math

type Point struct {
	X, Y int32
}

func CreatePoint(x, y int32) *Point {
	return &Point{
		X: x,
		Y: y,
	}
}

func (p *Point) Add(other *Point) *Point {
	return CreatePoint(p.X+other.X, p.Y+other.Y)
}

func (p *Point) Sub(other *Point) *Point {
	return CreatePoint(p.X-other.X, p.Y-other.Y)
}

func (p *Point) Mult(num int32) *Point {
	return CreatePoint(p.X*num, p.Y*num)
}

func (p *Point) Div(num int32) *Point {
	return CreatePoint(p.X/num, p.Y/num)
}
