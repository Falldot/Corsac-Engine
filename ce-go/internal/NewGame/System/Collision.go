package system

import (
	"github.com/Falldot/ce2d/internal/NewGame/Comp"
	"github.com/Falldot/ce2d/pkg/ECS"
	"github.com/Falldot/ce2d/pkg/GUI"
	"github.com/Falldot/ce2d/pkg/ce2d_quadTree"
	"github.com/veandco/go-sdl2/sdl"
)

var tree = ce2d_quadTree.CreateQuadTree(0, 0, 800, 600, 0)

// @Getter [COLLISION] -> GameController
var CollisionGetter ECS.Getter = func() ECS.Group {
	return ECS.GetGroup(ECS.AllOf(Comp.COLLIDER))
}

// @Executer [COLLISION] -> GameController
var CollisionExecuter ECS.Executer = func(group ECS.Group, dt float64) {

	GUI.Renderer.SetDrawColor(140, 255, 255, 255)

	tree = ce2d_quadTree.CreateQuadTree(0, 0, 800, 600, 0)

	var colliders []*Comp.Colider

	group.ForEach(func(e ECS.Entity) {

		transform := e.Get(Comp.TRANSFORM)
		collider := e.Get(Comp.COLLIDER).(*Comp.Colider)

		collider.SetX(int32(transform.(*Comp.Transform).Position.X))
		collider.SetY(int32(transform.(*Comp.Transform).Position.Y))

		GUI.Renderer.DrawRect(&sdl.Rect{
			X: collider.MinPosition.X,
			Y: collider.MinPosition.Y,
			W: collider.MaxPosition.X - collider.MinPosition.X,
			H: collider.MaxPosition.Y - collider.MinPosition.Y,
		})

		colliders = append(colliders, collider)

	})

	tree.AddColiders(colliders...)

	GUI.Renderer.SetDrawColor(177, 40, 255, 255)
	tree.ForEach(func(q *ce2d_quadTree.QuadTree) {
		GUI.Renderer.DrawRect(&sdl.Rect{
			X: q.MinPosition.X,
			Y: q.MinPosition.Y,
			W: q.MaxPosition.X - q.MinPosition.X,
			H: q.MaxPosition.Y - q.MinPosition.Y,
		})
	})

	tree.Start()
	GUI.Renderer.SetDrawColor(0, 0, 0, 255)
}
