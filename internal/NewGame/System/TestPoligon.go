package system

import (
	"github.com/Falldot/ce2d/internal/NewGame/Comp"
	"github.com/Falldot/ce2d/pkg/ecs"
	"github.com/Falldot/ce2d/pkg/rm"
)

// @Initer -> Main
var InitorLoadResources ecs.Initer = func(pool ecs.Pool) {
	rm.ResM.LoadSystemFont("Arial", 14)
	rm.ResM.LoadTexture("fox", "./assets/fox.png")
}

// @Initer -> Main
var InitorCreaterUnits ecs.Initer = func(pool ecs.Pool) {
	// Create player
	pool.CreateEntity().
		Add(Comp.TRANSFORM, Comp.CreateTransform(100, 100, 300)).
		Add(Comp.SPRITE, Comp.CreateSprite(rm.ResM.GetTexture("fox"), 32, 32, 0, 2)).
		Add(Comp.ANIMATION, Comp.CreateAnimation(func(anim *Comp.Animation) {
			anim.AddAnimation("Idle", 0, 6, 300)
			anim.AddAnimation("Right", 1, 6, 100)
			anim.AddAnimation("Down", 2, 6, 100)
			anim.AddAnimation("Up", 3, 6, 100)
			anim.AddAnimation("DownRight", 4, 6, 100)
			anim.Play("Idle")
		})).
		Add(Comp.PLAYER, Comp.CreatePlayer()).
		Add(Comp.COLLIDER, Comp.CreateColider(100, 100, 64, 64, func(self, other *Comp.Colider) {

		}))

	pool.CreateEntity().
		Add(Comp.TRANSFORM, Comp.CreateTransform(300, 300, 300)).
		Add(Comp.SPRITE, Comp.CreateSprite(rm.ResM.GetTexture("fox"), 32, 32, 0, 2)).
		Add(Comp.ANIMATION, Comp.CreateAnimation(func(anim *Comp.Animation) {
			anim.AddAnimation("Idle", 0, 6, 300)
			anim.AddAnimation("Right", 1, 6, 100)
			anim.AddAnimation("Down", 2, 6, 100)
			anim.AddAnimation("Up", 3, 6, 100)
			anim.AddAnimation("DownRight", 4, 6, 100)
			anim.Play("Idle")
		})).
		Add(Comp.COLLIDER, Comp.CreateColider(300, 300, 64, 64, func(self, other *Comp.Colider) {

		}))

	pool.CreateEntity().
		Add(Comp.TRANSFORM, Comp.CreateTransform(0, 0, 0)).
		Add(Comp.COLLIDER, Comp.CreateColider(0, 0, 1, 599, func(self, other *Comp.Colider) {

		}))
	pool.CreateEntity().
		Add(Comp.TRANSFORM, Comp.CreateTransform(2, 0, 0)).
		Add(Comp.COLLIDER, Comp.CreateColider(2, 0, 797, 1, func(self, other *Comp.Colider) {

		}))
	pool.CreateEntity().
		Add(Comp.TRANSFORM, Comp.CreateTransform(800, 0, 0)).
		Add(Comp.COLLIDER, Comp.CreateColider(800, 0, 1, 599, func(self, other *Comp.Colider) {

		}))
	pool.CreateEntity().
		Add(Comp.TRANSFORM, Comp.CreateTransform(2, 598, 0)).
		Add(Comp.COLLIDER, Comp.CreateColider(2, 598, 797, 1, func(self, other *Comp.Colider) {

		}))
}

// @Exiter -> Main
var ExiterUnLoadResources ecs.Exiter = func(pool ecs.Pool) {
	rm.ResM.AllUnload()
}
