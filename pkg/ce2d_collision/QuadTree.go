package ce2d_collision

import (
	"github.com/Falldot/ce2d/pkg/ce2d_math"
)

type QuadTree struct {
	Position      ce2d_math.Vector2D
	Width, Height int
	Parent        *QuadTree
	Deep          int
	Nodes         [4]*QuadTree
	Coliders      []AABB
}

func CreateQuadTree(x, y float64, width, height int) *QuadTree {

	return &QuadTree{}
}

func CreateEmptyQuadTree(width, height int) *QuadTree {
	return &QuadTree{}
}
