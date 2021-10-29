package ce2d_quadTree

import (
	"github.com/Falldot/ce2d/internal/NewGame/Comp"
	"github.com/Falldot/ce2d/pkg/ce2d_collision"
	"github.com/Falldot/ce2d/pkg/ce2d_math"
)

type QuadTree struct {
	MinPosition, MaxPosition *ce2d_math.Point
	Nodes                    [4]*QuadTree
	Coliders                 []*Comp.Colider
	Deep                     int
}

func CreateQuadTree(x, y, width, height int32, deep int) *QuadTree {
	return &QuadTree{
		MinPosition: ce2d_math.CreatePoint(x, y),
		MaxPosition: ce2d_math.CreatePoint(x+width, y+height),
		Deep:        deep,
	}
}

func (q *QuadTree) addNodes() {
	width := q.MaxPosition.X - q.MinPosition.X
	height := q.MaxPosition.Y - q.MinPosition.Y
	q.Nodes[0] = CreateQuadTree(
		q.MinPosition.X, q.MinPosition.Y,
		width/2, height/2, q.Deep+1)
	q.Nodes[1] = CreateQuadTree(
		q.MinPosition.X+width/2, q.MinPosition.Y,
		width/2, height/2, q.Deep+1)
	q.Nodes[2] = CreateQuadTree(
		q.MinPosition.X, q.MinPosition.Y+height/2,
		width/2, height/2, q.Deep+1)
	q.Nodes[3] = CreateQuadTree(
		q.MinPosition.X+width/2, q.MinPosition.Y+height/2,
		width/2, height/2, q.Deep+1)
}

func (q *QuadTree) AddColiders(coliders ...*Comp.Colider) {
	if len(coliders) > 1 && q.Deep < 5 {
		if q.Nodes[0] == nil {
			q.addNodes()
		}
		var t [4][]*Comp.Colider
		for _, colider := range coliders {
			for i, node := range q.Nodes {
				if ce2d_collision.DividingAxisCheck(colider.MinPosition, colider.MaxPosition, node.MinPosition, node.MaxPosition) {

					t[i] = append(t[i], colider)
				}
			}
		}
		for i := 0; i < 4; i++ {
			q.Nodes[i].AddColiders(t[i]...)
		}
	} else {
		q.Coliders = append(q.Coliders, coliders...)
	}
}

func (q *QuadTree) ForEach(callback func(q *QuadTree)) {
	callback(q)

	if q.Nodes[0] != nil {
		for _, node := range q.Nodes {
			node.ForEach(callback)
		}
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
				item.Chek(q.Coliders[i])
			}
		}
	}
}
