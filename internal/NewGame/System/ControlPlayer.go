package system

import (
	"github.com/Falldot/ce2d/internal/NewGame/Comp"
	"github.com/Falldot/ce2d/pkg/ecs"
	"github.com/Falldot/ce2d/pkg/rm"
	"github.com/veandco/go-sdl2/sdl"
)

// @Getter [CONTROLPLAYER] -> Main
var ControlPlayerGetter ecs.Getter = func(pool ecs.Pool) ecs.Group {
	return pool.GetGroup(ecs.AllOf(Comp.PLAYER))
}

// @Executer [CONTROLPLAYER] -> Main
var ControlPlayerExecuter ecs.Executer = func(pool ecs.Pool, group ecs.Group, dt float64) {
	group.ForEach(func(e ecs.Entity) {
		c := e.Get(Comp.TRANSFORM)
		sm := e.Get(Comp.ANIMATION)
		s := e.Get(Comp.SPRITE)

		if rm.KeyPress(sdl.SCANCODE_W, sdl.SCANCODE_D, sdl.SCANCODE_S, sdl.SCANCODE_A) {
			c.(*Comp.Transform).CurrentSpeed = c.(*Comp.Transform).Speed
		}
		if rm.KeyPress(sdl.SCANCODE_W) {
			c.(*Comp.Transform).Direction.Set(0, -1)
			sm.(*Comp.Animation).Play("Up")
			s.(*Comp.Sprite).Flip = sdl.FLIP_NONE
		}
		if rm.KeyPress(sdl.SCANCODE_D) {
			c.(*Comp.Transform).Direction.Set(1, 0)
			sm.(*Comp.Animation).Play("Right")
			s.(*Comp.Sprite).Flip = sdl.FLIP_NONE
		}
		if rm.KeyPress(sdl.SCANCODE_S) {
			c.(*Comp.Transform).Direction.Set(0, 1)
			sm.(*Comp.Animation).Play("Down")
			s.(*Comp.Sprite).Flip = sdl.FLIP_NONE
		}
		if rm.KeyPress(sdl.SCANCODE_A) {
			c.(*Comp.Transform).Direction.Set(-1, 0)
			sm.(*Comp.Animation).Play("Right")
			s.(*Comp.Sprite).Flip = sdl.FLIP_HORIZONTAL
		}

		if rm.KeyPressMod(sdl.SCANCODE_A, sdl.SCANCODE_W) {
			c.(*Comp.Transform).Direction.Set(-1, 0)
			c.(*Comp.Transform).Direction.Rotate(45)
			sm.(*Comp.Animation).Play("Right")
			s.(*Comp.Sprite).Flip = sdl.FLIP_HORIZONTAL
		}

		if rm.KeyPressMod(sdl.SCANCODE_D, sdl.SCANCODE_W) {
			c.(*Comp.Transform).Direction.Set(0, -1)
			c.(*Comp.Transform).Direction.Rotate(45)
			sm.(*Comp.Animation).Play("Right")
			s.(*Comp.Sprite).Flip = sdl.FLIP_NONE
		}

		if rm.KeyPressMod(sdl.SCANCODE_D, sdl.SCANCODE_S) {
			c.(*Comp.Transform).Direction.Set(1, 0)
			c.(*Comp.Transform).Direction.Rotate(45)
			sm.(*Comp.Animation).Play("DownRight")
			s.(*Comp.Sprite).Flip = sdl.FLIP_NONE
		}

		if rm.KeyPressMod(sdl.SCANCODE_A, sdl.SCANCODE_S) {
			c.(*Comp.Transform).Direction.Set(0, 1)
			c.(*Comp.Transform).Direction.Rotate(45)
			sm.(*Comp.Animation).Play("DownRight")
			s.(*Comp.Sprite).Flip = sdl.FLIP_HORIZONTAL
		}

		if !rm.KeyPress(sdl.SCANCODE_W, sdl.SCANCODE_D, sdl.SCANCODE_S, sdl.SCANCODE_A) {
			c.(*Comp.Transform).CurrentSpeed = 0
			sm.(*Comp.Animation).Play("Idle")
		}
	})
}
