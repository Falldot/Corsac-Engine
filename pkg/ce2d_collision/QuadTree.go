package ce2d_collision

import (
	"github.com/Falldot/ce2d/pkg/ce2d_math"
)

type QuadTree struct {
	MinPosition, MaxPosition *ce2d_math.Vector2D
	Nodes                    [4]*QuadTree
	Coliders                 []AABB
	Deep                     int
}

func CreateQuadTree(x, y, width, height float64, deep int) *QuadTree {
	return &QuadTree{
		MinPosition: ce2d_math.CreateVector2D(x, y),
		MaxPosition: ce2d_math.CreateVector2D(x+width, y+height),
		Deep:        0,
	}
}

func (q *QuadTree) addNodes() {
	q.Nodes[0] = CreateQuadTree(
		q.MinPosition.X, q.MinPosition.Y,
		q.MaxPosition.X/2, q.MaxPosition.Y/2, q.Deep+1)
	q.Nodes[1] = CreateQuadTree(
		q.MaxPosition.X/2, q.MinPosition.Y,
		q.MaxPosition.X, q.MaxPosition.Y/2, q.Deep+1)
	q.Nodes[2] = CreateQuadTree(
		q.MinPosition.X, q.MaxPosition.Y/2,
		q.MaxPosition.X/2, q.MaxPosition.Y, q.Deep+1)
	q.Nodes[3] = CreateQuadTree(
		q.MaxPosition.X/2, q.MaxPosition.Y/2,
		q.MaxPosition.X, q.MaxPosition.Y, q.Deep+1)
}

func (q *QuadTree) AddColiders(coliders []AABB) {
	if len(coliders) > 1 && q.Deep < 5 {
		if q.Nodes[0] == nil {
			q.addNodes()
		}
		var t [4][]AABB
		for _, colider := range coliders {
			for i, node := range q.Nodes {
				if dividingAxisCheck(colider.MinPosition, colider.MaxPosition, node.MinPosition, node.MaxPosition) {
					t[i] = append(t[i], colider)
					break
				}
			}
		}
		for i := 0; i < 4; i++ {
			q.Nodes[i].AddColiders(t[i])
		}
	} else {
		q.Coliders = append(q.Coliders, coliders...)
	}
}

func (q *QuadTree) Start() {
	if q.Nodes[0] != nil {
		for _, i := range q.Nodes {
			i.Start()
		}
	} else {
		for index, item := range q.Coliders {
			for i := index + 1; i < len(q.Coliders); i++ {
				item.chek(&q.Coliders[i])
			}
		}
	}
}
