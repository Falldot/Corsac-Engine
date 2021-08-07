package ce2d_ecs

import (
	"github.com/Falldot/ce2d/pkg/ce2d_math"
	"github.com/Falldot/ce2d/pkg/ecs"
	"github.com/Falldot/ce2d/pkg/gdl"
	"github.com/veandco/go-sdl2/sdl"
)

type (
	Sprite struct {
		Tex      *sdl.Texture
		Src, Dst *sdl.Rect
		Angle    float64
		Center   ce2d_math.Vector2D
		Flip     sdl.RendererFlip
	}

	SDLRender struct {
		comps ecs.Entities
	}
)

func CreateSprite(tex *sdl.Texture, width int32, height int32, angle float64, scale int32) *Sprite {
	s := &Sprite{
		Tex: tex,
		Src: &sdl.Rect{
			W: width,
			H: height,
		},
		Dst: &sdl.Rect{
			W: width * scale,
			H: height * scale,
		},
		Angle: angle,
	}
	return s
}

func (sys *SDLRender) Init(pool *ecs.Pool) {
	sys.comps = pool.GetEntities(SPRITE)
}

func (sys *SDLRender) Update() {
	for _, v := range sys.comps {
		s := v.Get(SPRITE)
		c := v.Get(TRANSFORM)

		s.(*Sprite).Dst.X = int32(c.(*Transform).Position.X)
		s.(*Sprite).Dst.Y = int32(c.(*Transform).Position.Y)

		err := gdl.Render.CopyEx(s.(*Sprite).Tex, s.(*Sprite).Src, s.(*Sprite).Dst, s.(*Sprite).Angle, nil, s.(*Sprite).Flip)
		if err != nil {
			panic(err)
		}
	}
}
