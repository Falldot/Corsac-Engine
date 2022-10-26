package system

import (
	"github.com/Falldot/ce2d/internal/NewGame/Comp"
	"github.com/Falldot/ce2d/pkg/ECS"
	"github.com/Falldot/ce2d/pkg/Event"
	"github.com/veandco/go-sdl2/sdl"
)

// @Getter [CONTROLPLAYER] -> GameController
var ControlPlayerGetter ECS.Getter = func() ECS.Group {
	return ECS.GetGroup(ECS.AllOf(Comp.PLAYER))
}

// @Executer [CONTROLPLAYER] -> GameController
var ControlPlayerExecuter ECS.Executer = func(group ECS.Group, dt float64) {
	group.ForEach(func(e ECS.Entity) {
		c := e.Get(Comp.TRANSFORM)
		sm := e.Get(Comp.ANIMATION)
		s := e.Get(Comp.SPRITE)

		if Event.KeyPress(sdl.SCANCODE_W, sdl.SCANCODE_D, sdl.SCANCODE_S, sdl.SCANCODE_A) {
			c.(*Comp.Transform).CurrentSpeed = c.(*Comp.Transform).Speed
		}
		if Event.KeyPress(sdl.SCANCODE_W) {
			c.(*Comp.Transform).Direction.Set(0, -1)
			sm.(*Comp.Animation).Play("Up")
			s.(*Comp.Sprite).Flip = sdl.FLIP_NONE
		}
		if Event.KeyPress(sdl.SCANCODE_D) {
			c.(*Comp.Transform).Direction.Set(1, 0)
			sm.(*Comp.Animation).Play("Right")
			s.(*Comp.Sprite).Flip = sdl.FLIP_NONE
		}
		if Event.KeyPress(sdl.SCANCODE_S) {
			c.(*Comp.Transform).Direction.Set(0, 1)
			sm.(*Comp.Animation).Play("Down")
			s.(*Comp.Sprite).Flip = sdl.FLIP_NONE
		}
		if Event.KeyPress(sdl.SCANCODE_A) {
			c.(*Comp.Transform).Direction.Set(-1, 0)
			sm.(*Comp.Animation).Play("Right")
			s.(*Comp.Sprite).Flip = sdl.FLIP_HORIZONTAL
		}

		if Event.KeyPressMod(sdl.SCANCODE_A, sdl.SCANCODE_W) {
			c.(*Comp.Transform).Direction.Set(-1, 0)
			c.(*Comp.Transform).Direction.Rotate(45)
			sm.(*Comp.Animation).Play("Right")
			s.(*Comp.Sprite).Flip = sdl.FLIP_HORIZONTAL
		}

		if Event.KeyPressMod(sdl.SCANCODE_D, sdl.SCANCODE_W) {
			c.(*Comp.Transform).Direction.Set(0, -1)
			c.(*Comp.Transform).Direction.Rotate(45)
			sm.(*Comp.Animation).Play("Right")
			s.(*Comp.Sprite).Flip = sdl.FLIP_NONE
		}

		if Event.KeyPressMod(sdl.SCANCODE_D, sdl.SCANCODE_S) {
			c.(*Comp.Transform).Direction.Set(1, 0)
			c.(*Comp.Transform).Direction.Rotate(45)
			sm.(*Comp.Animation).Play("DownRight")
			s.(*Comp.Sprite).Flip = sdl.FLIP_NONE
		}

		if Event.KeyPressMod(sdl.SCANCODE_A, sdl.SCANCODE_S) {
			c.(*Comp.Transform).Direction.Set(0, 1)
			c.(*Comp.Transform).Direction.Rotate(45)
			sm.(*Comp.Animation).Play("DownRight")
			s.(*Comp.Sprite).Flip = sdl.FLIP_HORIZONTAL
		}

		if !Event.KeyPress(sdl.SCANCODE_W, sdl.SCANCODE_D, sdl.SCANCODE_S, sdl.SCANCODE_A) {
			c.(*Comp.Transform).CurrentSpeed = 0
			sm.(*Comp.Animation).Play("Idle")
		}
	})
}
