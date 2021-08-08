package ce2d_ecs

import (
	"github.com/Falldot/ce2d/pkg/ce2d_math"
	"github.com/Falldot/ce2d/pkg/ecs"
	"github.com/Falldot/ce2d/pkg/gdl"
	"github.com/veandco/go-sdl2/sdl"
)

type (
	Control struct {
		Route *ce2d_math.Vector2D
	}

	sysControl struct {
		comps ecs.Entities
	}
)

func CreateControl(x, y float64) *Control {
	return &Control{
		Route: ce2d_math.CreateVector2D(x, y),
	}
}

func (sys *sysControl) Init(pool *ecs.Pool) {
	sys.comps = pool.GetEntities(CONTROL)
}

func (sys *sysControl) Update(dt float64) {
	for _, v := range sys.comps {
		c := v.Get(TRANSFORM)
		sm := v.Get(ANIMATION)
		s := v.Get(SPRITE)

		if gdl.KeyPress(sdl.SCANCODE_W, sdl.SCANCODE_D, sdl.SCANCODE_S, sdl.SCANCODE_A) {
			c.(*Transform).CurrentSpeed = c.(*Transform).Speed
		}
		if gdl.KeyPress(sdl.SCANCODE_W) {
			c.(*Transform).Direction.Set(0, -1)
			sm.(*Animation).Play("Up")
			s.(*Sprite).Flip = sdl.FLIP_NONE
		}
		if gdl.KeyPress(sdl.SCANCODE_D) {
			c.(*Transform).Direction.Set(1, 0)
			sm.(*Animation).Play("Right")
			s.(*Sprite).Flip = sdl.FLIP_NONE
		}
		if gdl.KeyPress(sdl.SCANCODE_S) {
			c.(*Transform).Direction.Set(0, 1)
			sm.(*Animation).Play("Down")
			s.(*Sprite).Flip = sdl.FLIP_NONE
		}
		if gdl.KeyPress(sdl.SCANCODE_A) {
			c.(*Transform).Direction.Set(-1, 0)
			sm.(*Animation).Play("Right")
			s.(*Sprite).Flip = sdl.FLIP_HORIZONTAL
		}

		if gdl.KeyPressMod(sdl.SCANCODE_A, sdl.SCANCODE_W) {
			c.(*Transform).Direction.Set(-1, 0)
			c.(*Transform).Direction.Rotate(45)
			sm.(*Animation).Play("Right")
			s.(*Sprite).Flip = sdl.FLIP_HORIZONTAL
		}

		if gdl.KeyPressMod(sdl.SCANCODE_D, sdl.SCANCODE_W) {
			c.(*Transform).Direction.Set(0, -1)
			c.(*Transform).Direction.Rotate(45)
			sm.(*Animation).Play("Right")
			s.(*Sprite).Flip = sdl.FLIP_NONE
		}

		if gdl.KeyPressMod(sdl.SCANCODE_D, sdl.SCANCODE_S) {
			c.(*Transform).Direction.Set(1, 0)
			c.(*Transform).Direction.Rotate(45)
			sm.(*Animation).Play("DownRight")
			s.(*Sprite).Flip = sdl.FLIP_NONE
		}

		if gdl.KeyPressMod(sdl.SCANCODE_A, sdl.SCANCODE_S) {
			c.(*Transform).Direction.Set(0, 1)
			c.(*Transform).Direction.Rotate(45)
			sm.(*Animation).Play("DownRight")
			s.(*Sprite).Flip = sdl.FLIP_HORIZONTAL
		}

		if !gdl.KeyPress(sdl.SCANCODE_W, sdl.SCANCODE_D, sdl.SCANCODE_S, sdl.SCANCODE_A) {
			c.(*Transform).CurrentSpeed = 0
			sm.(*Animation).Play("Idle")
		}
	}
}
