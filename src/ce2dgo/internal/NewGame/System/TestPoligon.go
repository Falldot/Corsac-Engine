package system

import (
	"github.com/Falldot/ce2d/internal/NewGame/Comp"
	"github.com/Falldot/ce2d/pkg/ECS"
	"github.com/Falldot/ce2d/pkg/Resourcer"
)

// @Initer -> Play <- GameController
var GameController ECS.Initer = func() {
	ECS.AddSystem(InitorLoadResources, InitorCreaterUnits, ExiterUnLoadResources)

	ECS.AddSystem(TranslateGetter, TranslateExecuter)
	ECS.AddSystem(RendererGetter, RendererExecuter)
	ECS.AddSystem(AnimatedGetter, AnimatedExecuter)
	ECS.AddSystem(ControlPlayerGetter, ControlPlayerExecuter)
	ECS.AddSystem(CollisionGetter, CollisionExecuter)
}

// @Initer -> GameController
var InitorLoadResources ECS.Initer = func() {
	Resourcer.LoadSystemFont("Arial", 14)
	Resourcer.LoadTexture("fox", "./assets/fox.png")
}

// @Initer -> GameController
var InitorCreaterUnits ECS.Initer = func() {
	// Create player
	ECS.CreateEntity().
		Add(Comp.TRANSFORM, Comp.CreateTransform(100, 100, 300)).
		Add(Comp.SPRITE, Comp.CreateSprite(Resourcer.GetTexture("fox"), 32, 32, 0, 2)).
		Add(Comp.ANIMATION, Comp.CreateAnimation(func(anim *Comp.Animation) {
			anim.AddAnimation("Idle", 0, 6, 300)
			anim.AddAnimation("Right", 1, 6, 100)
			anim.AddAnimation("Down", 2, 6, 100)
			anim.AddAnimation("Up", 3, 6, 100)
			anim.AddAnimation("DownRight", 4, 6, 100)
			anim.Play("Idle")
		})).
		Add(Comp.PLAYER, Comp.CreatePlayer()).
		Add(Comp.COLLIDER, Comp.CreateColider(100, 100, 64, 64, func(self, other *Comp.Colider) bool {
			return true
		}, func(self, other *Comp.Colider) {}))

	ECS.CreateEntity().
		Add(Comp.TRANSFORM, Comp.CreateTransform(300, 300, 300)).
		Add(Comp.SPRITE, Comp.CreateSprite(Resourcer.GetTexture("fox"), 32, 32, 0, 2)).
		Add(Comp.ANIMATION, Comp.CreateAnimation(func(anim *Comp.Animation) {
			anim.AddAnimation("Idle", 0, 6, 300)
			anim.AddAnimation("Right", 1, 6, 100)
			anim.AddAnimation("Down", 2, 6, 100)
			anim.AddAnimation("Up", 3, 6, 100)
			anim.AddAnimation("DownRight", 4, 6, 100)
			anim.Play("Idle")
		})).
		Add(Comp.COLLIDER, Comp.CreateColider(300, 300, 64, 64, func(self, other *Comp.Colider) bool {
			return true
		}, func(self, other *Comp.Colider) {}))
}

// @Exiter -> GameController
var ExiterUnLoadResources ECS.Exiter = func() {
	Resourcer.AllUnload()
}
