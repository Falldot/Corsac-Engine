package system

import (
	"github.com/Falldot/ce2d/internal/component"
	"github.com/Falldot/ce2d/pkg/ecs"
	"github.com/Falldot/ce2d/pkg/gdl"
	"github.com/veandco/go-sdl2/sdl"
)

type PlayerControl struct {
	comps ecs.Entities
}

func (k *PlayerControl) SetPool(pool *ecs.Pool) {
	k.comps = pool.GetEntities(component.CONTROL)
}

func (k *PlayerControl) Execute() {
	for _, v := range k.comps {
		c := v.Get(component.TRANSFORM)
		sm := v.Get(component.SMANIMATION)
		s := v.Get(component.SPRITE)

		if gdl.KeyPress(sdl.SCANCODE_W, sdl.SCANCODE_D, sdl.SCANCODE_S, sdl.SCANCODE_A) {
			c.(*component.Transform).Speed = 5
		}
		if gdl.KeyPress(sdl.SCANCODE_W) {
			c.(*component.Transform).Direction.Set(0, -1)
			sm.(*component.SMAnimation).Play("Up")
			s.(*component.Sprite).Flip = sdl.FLIP_NONE
		}
		if gdl.KeyPress(sdl.SCANCODE_D) {
			c.(*component.Transform).Direction.Set(1, 0)
			sm.(*component.SMAnimation).Play("Right")
			s.(*component.Sprite).Flip = sdl.FLIP_NONE
		}
		if gdl.KeyPress(sdl.SCANCODE_S) {
			c.(*component.Transform).Direction.Set(0, 1)
			sm.(*component.SMAnimation).Play("Down")
			s.(*component.Sprite).Flip = sdl.FLIP_NONE
		}
		if gdl.KeyPress(sdl.SCANCODE_A) {
			c.(*component.Transform).Direction.Set(-1, 0)
			sm.(*component.SMAnimation).Play("Right")
			s.(*component.Sprite).Flip = sdl.FLIP_HORIZONTAL
		}

		if gdl.KeyPressMod(sdl.SCANCODE_A, sdl.SCANCODE_W) {
			c.(*component.Transform).Direction.Set(-1, 0)
			c.(*component.Transform).Direction.Rotate(45)
			sm.(*component.SMAnimation).Play("Right")
			s.(*component.Sprite).Flip = sdl.FLIP_HORIZONTAL
		}

		if gdl.KeyPressMod(sdl.SCANCODE_D, sdl.SCANCODE_W) {
			c.(*component.Transform).Direction.Set(0, -1)
			c.(*component.Transform).Direction.Rotate(45)
			sm.(*component.SMAnimation).Play("Right")
			s.(*component.Sprite).Flip = sdl.FLIP_NONE
		}

		if gdl.KeyPressMod(sdl.SCANCODE_D, sdl.SCANCODE_S) {
			c.(*component.Transform).Direction.Set(1, 0)
			c.(*component.Transform).Direction.Rotate(45)
			sm.(*component.SMAnimation).Play("DownRight")
			s.(*component.Sprite).Flip = sdl.FLIP_NONE
		}

		if gdl.KeyPressMod(sdl.SCANCODE_A, sdl.SCANCODE_S) {
			c.(*component.Transform).Direction.Set(0, 1)
			c.(*component.Transform).Direction.Rotate(45)
			sm.(*component.SMAnimation).Play("DownRight")
			s.(*component.Sprite).Flip = sdl.FLIP_HORIZONTAL
		}

		if !gdl.KeyPress(sdl.SCANCODE_W, sdl.SCANCODE_D, sdl.SCANCODE_S, sdl.SCANCODE_A) {
			c.(*component.Transform).Speed = 0
			sm.(*component.SMAnimation).Play("Idle")
		}
	}
}
