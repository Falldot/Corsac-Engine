package system

import (
	"github.com/Falldot/ce2d/internal/NewGame/Comp"
	"github.com/Falldot/ce2d/pkg/ce2d_quadTree"
	"github.com/Falldot/ce2d/pkg/ecs"
	"github.com/Falldot/ce2d/pkg/rm"
	"github.com/veandco/go-sdl2/sdl"
)

var tree = ce2d_quadTree.CreateQuadTree(0, 0, 800, 600, 0)

// @Getter [COLLISION] -> Main
var CollisionGetter ecs.Getter = func(pool ecs.Pool) ecs.Group {
	return pool.GetGroup(ecs.AllOf(Comp.COLLIDER))
}

// @Executer [COLLISION] -> Main
var CollisionExecuter ecs.Executer = func(pool ecs.Pool, group ecs.Group, dt float64) {

	rm.Renderer.SetDrawColor(140, 255, 255, 255)

	tree = ce2d_quadTree.CreateQuadTree(0, 0, 800, 600, 0)

	var colliders []*Comp.Colider

	group.ForEach(func(e ecs.Entity) {

		transform := e.Get(Comp.TRANSFORM)
		collider := e.Get(Comp.COLLIDER).(*Comp.Colider)

		collider.SetX(int32(transform.(*Comp.Transform).Position.X))
		collider.SetY(int32(transform.(*Comp.Transform).Position.Y))

		rm.Renderer.DrawRect(&sdl.Rect{
			X: collider.MinPosition.X,
			Y: collider.MinPosition.Y,
			W: collider.MaxPosition.X - collider.MinPosition.X,
			H: collider.MaxPosition.Y - collider.MinPosition.Y,
		})

		colliders = append(colliders, collider)

	})

	tree.AddColiders(colliders...)

	rm.Renderer.SetDrawColor(177, 40, 255, 255)
	tree.ForEach(func(q *ce2d_quadTree.QuadTree) {
		rm.Renderer.DrawRect(&sdl.Rect{
			X: q.MinPosition.X,
			Y: q.MinPosition.Y,
			W: q.MaxPosition.X - q.MinPosition.X,
			H: q.MaxPosition.Y - q.MinPosition.Y,
		})
	})

	tree.Start()
	rm.Renderer.SetDrawColor(0, 0, 0, 255)
}
