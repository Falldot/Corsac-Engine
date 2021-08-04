package system

import (
	"fmt"
	"math"

	"github.com/Falldot/ce2d/internal/component"
	"github.com/Falldot/ce2d/pkg/ecs"
	"github.com/Falldot/ce2d/pkg/gdl"
	"github.com/veandco/go-sdl2/sdl"
)

type KeyboardController struct {
	comps ecs.Entities
}

func (k *KeyboardController) SetPool(pool *ecs.Pool) {
	k.comps = pool.GetEntities(component.CONTROL)
	for _, v := range k.comps {
		c := v.Get(component.TRANSFORM)
		sm := v.Get(component.SMANIMATION)
		s := v.Get(component.SPRITE)
		w := v.Get(component.CONTROL)

		var ooo *ecs.Event

		pool.CreateEvent(
			func(e *ecs.Event) bool {
				return gdl.MousePress(sdl.BUTTON_LEFT)
			},
			func(e *ecs.Event) {
				x, y, _ := sdl.GetMouseState()
				w.(*component.Control).Route.Set(float64(x), float64(y))
				x -= int32(c.(*component.Transform).Position.X) + s.(*component.Sprite).Dst.W/2
				y -= int32(c.(*component.Transform).Position.Y) + s.(*component.Sprite).Dst.H/2
				angle := (math.Atan2(float64(y), float64(x))) * (180 / math.Pi)
				c.(*component.Transform).Direction.Set(1, 0)
				c.(*component.Transform).Direction.Rotate(int16(angle))
				c.(*component.Transform).Speed = 5
				if ooo != nil {
					pool.DestroyEvent(ooo)
				}
				ooo = pool.CreateEvent(
					func(e *ecs.Event) bool {
						return true
					},
					func(e *ecs.Event) {
						x := w.(*component.Control).Route.X
						y := w.(*component.Control).Route.Y
						x -= c.(*component.Transform).Position.X + float64(s.(*component.Sprite).Dst.W/2)
						y -= c.(*component.Transform).Position.Y + float64(s.(*component.Sprite).Dst.H/2)
						angle := (math.Atan2(float64(y), float64(x))) * (180 / math.Pi)
						c.(*component.Transform).Direction.Set(1, 0)
						c.(*component.Transform).Direction.Rotate(int16(angle))
					},
				)
			},
		)

		pool.CreateEvent(
			func(event *ecs.Event) bool {
				fmt.Println(c.(*component.Transform).Speed)
				return ((c.(*component.Transform).Direction.Angle() >= -45 && c.(*component.Transform).Direction.Angle() <= 0) ||
					(c.(*component.Transform).Direction.Angle() <= 22 && c.(*component.Transform).Direction.Angle() >= 0)) &&
					c.(*component.Transform).Speed > 0
			},
			func(event *ecs.Event) {
				sm.(*component.SMAnimation).Play("Right")
				s.(*component.Sprite).Flip = sdl.FLIP_NONE
			},
		)

		pool.CreateEvent(
			func(event *ecs.Event) bool {
				return ((c.(*component.Transform).Direction.Angle() <= -135 && c.(*component.Transform).Direction.Angle() >= -180) ||
					(c.(*component.Transform).Direction.Angle() >= 157 && c.(*component.Transform).Direction.Angle() <= 180)) &&
					c.(*component.Transform).Speed > 0
			},
			func(event *ecs.Event) {
				sm.(*component.SMAnimation).Play("Right")
				s.(*component.Sprite).Flip = sdl.FLIP_HORIZONTAL
			},
		)

		pool.CreateEvent(
			func(event *ecs.Event) bool {
				return (c.(*component.Transform).Direction.Angle() <= 112 && c.(*component.Transform).Direction.Angle() >= 68) &&
					c.(*component.Transform).Speed > 0
			},
			func(event *ecs.Event) {
				sm.(*component.SMAnimation).Play("Down")
				s.(*component.Sprite).Flip = sdl.FLIP_NONE
			},
		)

		pool.CreateEvent(
			func(event *ecs.Event) bool {
				return (c.(*component.Transform).Direction.Angle() <= 68 && c.(*component.Transform).Direction.Angle() >= 23) &&
					c.(*component.Transform).Speed > 0
			},
			func(event *ecs.Event) {
				sm.(*component.SMAnimation).Play("DownRight")
				s.(*component.Sprite).Flip = sdl.FLIP_NONE
			},
		)

		pool.CreateEvent(
			func(event *ecs.Event) bool {
				return (c.(*component.Transform).Direction.Angle() <= 158 && c.(*component.Transform).Direction.Angle() >= 112) &&
					c.(*component.Transform).Speed > 0
			},
			func(event *ecs.Event) {
				sm.(*component.SMAnimation).Play("DownRight")
				s.(*component.Sprite).Flip = sdl.FLIP_HORIZONTAL
			},
		)

		pool.CreateEvent(
			func(event *ecs.Event) bool {
				return (c.(*component.Transform).Direction.Angle() >= -112 && c.(*component.Transform).Direction.Angle() <= -68) &&
					c.(*component.Transform).Speed > 0
			},
			func(event *ecs.Event) {
				sm.(*component.SMAnimation).Play("Up")
				s.(*component.Sprite).Flip = sdl.FLIP_NONE
			},
		)

		pool.CreateEvent(
			func(event *ecs.Event) bool {
				return c.(*component.Transform).Position.X+float64(s.(*component.Sprite).Dst.W/2) > w.(*component.Control).Route.X-3 &&
					c.(*component.Transform).Position.X+float64(s.(*component.Sprite).Dst.W/2) < w.(*component.Control).Route.X+3 &&
					c.(*component.Transform).Position.Y+float64(s.(*component.Sprite).Dst.H/2) > w.(*component.Control).Route.Y-3 &&
					c.(*component.Transform).Position.Y+float64(s.(*component.Sprite).Dst.H/2) < w.(*component.Control).Route.Y+3
			},
			func(event *ecs.Event) {
				c.(*component.Transform).Speed = 0
				c.(*component.Transform).Direction.Set(0, 0)
				sm.(*component.SMAnimation).Play("Idle")
				s.(*component.Sprite).Flip = sdl.FLIP_NONE
				pool.DestroyEvent(ooo)
			},
		)

		// pool.CreateEvent(SDL,
		// 	func(data ecs.Events) bool {
		// 		return data[KEYBOARD].([]uint8)[sdl.SCANCODE_W] == 1 ||
		// 			data[KEYBOARD].([]uint8)[sdl.SCANCODE_D] == 1 ||
		// 			data[KEYBOARD].([]uint8)[sdl.SCANCODE_S] == 1 ||
		// 			data[KEYBOARD].([]uint8)[sdl.SCANCODE_A] == 1 ||
		// 			data[WINDOW].(uint32) == sdl.MOUSEMOTION
		// 	},
		// 	func() {
		// 		c.(*component.Transform).Speed = 5
		// 	},
		// 	func() {
		// 		x, y, _ := sdl.GetMouseState()
		// 		x -= int32(c.(*component.Transform).Position.X) + s.(*component.Sprite).Dst.W/2
		// 		y -= int32(c.(*component.Transform).Position.Y) + s.(*component.Sprite).Dst.H/2
		// 		s.(*component.Sprite).Angle = (math.Atan2(float64(y), float64(x))) * (180 / math.Pi)
		// 	},
		// )

		// pool.CreateEvent(SDL,
		// 	func(data ecs.Events) bool {
		// 		return data[KEYBOARD].([]uint8)[sdl.SCANCODE_W] == 1
		// 	},
		// 	func() {
		// 		c.(*component.Transform).Direction.Set(0, -1)
		// 	},
		// )

		// pool.CreateEvent(SDL,
		// 	func(data ecs.Events) bool {
		// 		return data[KEYBOARD].([]uint8)[sdl.SCANCODE_D] == 1
		// 	},
		// 	func() {
		// 		c.(*component.Transform).Direction.Set(1, 0)
		// 	},
		// )

		// pool.CreateEvent(SDL,
		// 	func(data ecs.Events) bool {
		// 		return data[KEYBOARD].([]uint8)[sdl.SCANCODE_S] == 1
		// 	},
		// 	func() {
		// 		c.(*component.Transform).Direction.Set(0, 1)
		// 	},
		// )

		// pool.CreateEvent(SDL,
		// 	func(data ecs.Events) bool {
		// 		return data[KEYBOARD].([]uint8)[sdl.SCANCODE_A] == 1
		// 	},
		// 	func() {
		// 		c.(*component.Transform).Direction.Set(-1, 0)
		// 	},
		// )

		// pool.CreateEvent(SDL,
		// 	func(data ecs.Events) bool {
		// 		return data[KEYBOARD].([]uint8)[sdl.SCANCODE_A] == 1 &&
		// 			data[KEYBOARD].([]uint8)[sdl.SCANCODE_W] == 1
		// 	},
		// 	func() {
		// 		c.(*component.Transform).Direction.Set(-1, 0)
		// 		c.(*component.Transform).Direction.Rotate(45)
		// 	},
		// )

		// pool.CreateEvent(SDL,
		// 	func(data ecs.Events) bool {
		// 		return data[KEYBOARD].([]uint8)[sdl.SCANCODE_W] == 1 &&
		// 			data[KEYBOARD].([]uint8)[sdl.SCANCODE_D] == 1
		// 	},
		// 	func() {
		// 		c.(*component.Transform).Direction.Set(0, -1)
		// 		c.(*component.Transform).Direction.Rotate(45)
		// 	},
		// )

		// pool.CreateEvent(SDL,
		// 	func(data ecs.Events) bool {
		// 		return data[KEYBOARD].([]uint8)[sdl.SCANCODE_D] == 1 &&
		// 			data[KEYBOARD].([]uint8)[sdl.SCANCODE_S] == 1
		// 	},
		// 	func() {
		// 		c.(*component.Transform).Direction.Set(1, 0)
		// 		c.(*component.Transform).Direction.Rotate(45)
		// 	},
		// )

		// pool.CreateEvent(SDL,
		// 	func(data ecs.Events) bool {
		// 		return data[KEYBOARD].([]uint8)[sdl.SCANCODE_S] == 1 &&
		// 			data[KEYBOARD].([]uint8)[sdl.SCANCODE_A] == 1
		// 	},
		// 	func() {
		// 		c.(*component.Transform).Direction.Set(0, 1)
		// 		c.(*component.Transform).Direction.Rotate(45)
		// 	},
		// )
		// pool.CreateEvent(SDL,
		// 	func(data ecs.Events) bool {
		// 		return data[KEYBOARD].([]uint8)[sdl.SCANCODE_W] == 0 &&
		// 			data[KEYBOARD].([]uint8)[sdl.SCANCODE_D] == 0 &&
		// 			data[KEYBOARD].([]uint8)[sdl.SCANCODE_S] == 0 &&
		// 			data[KEYBOARD].([]uint8)[sdl.SCANCODE_A] == 0
		// 	},
		// 	func() {
		// 		c.(*component.Transform).Speed = 0
		// 	},
		// )
	}
}

func (k *KeyboardController) Execute() {

	// 	// if !m.(*Move).W && !m.(*Move).S && !m.(*Move).A && !m.(*Move).D {
	// 	// 	c.(*Transform).Velocity.X = 0
	// 	// 	c.(*Transform).Velocity.Y = 0
	// 	// } else if m.(*Move).D && m.(*Move).S {
	// 	// 	c.(*Transform).Velocity.X = 1 * math.Sin(0.785398)
	// 	// 	c.(*Transform).Velocity.Y = 1 * math.Cos(0.785398)
	// 	// } else if m.(*Move).A && m.(*Move).S {
	// 	// 	c.(*Transform).Velocity.X = -1 * math.Sin(0.785398)
	// 	// 	c.(*Transform).Velocity.Y = 1 * math.Cos(0.785398)
	// 	// } else if m.(*Move).W && m.(*Move).A {
	// 	// 	c.(*Transform).Velocity.X = -1 * math.Sin(0.785398)
	// 	// 	c.(*Transform).Velocity.Y = -1 * math.Cos(0.785398)
	// 	// } else if m.(*Move).W && m.(*Move).D {
	// 	// 	c.(*Transform).Velocity.X = 1 * math.Sin(0.785398)
	// 	// 	c.(*Transform).Velocity.Y = -1 * math.Cos(0.785398)
	// 	// } else if m.(*Move).W {
	// 	// 	c.(*Transform).Velocity.X = 0
	// 	// 	c.(*Transform).Velocity.Y = -1
	// 	// } else if m.(*Move).S {
	// 	// 	c.(*Transform).Velocity.X = 0
	// 	// 	c.(*Transform).Velocity.Y = 1
	// 	// } else if m.(*Move).A {
	// 	// 	c.(*Transform).Velocity.X = -1
	// 	// 	c.(*Transform).Velocity.Y = 0
	// 	// } else if m.(*Move).D {
	// 	// 	c.(*Transform).Velocity.X = 1
	// 	// 	c.(*Transform).Velocity.Y = 0
	// 	// }
}
