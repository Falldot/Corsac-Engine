package component

import (
	"github.com/Falldot/ce2d/pkg/ce2d_math"
	"github.com/Falldot/ce2d/pkg/gdl"
	"github.com/veandco/go-sdl2/sdl"
)

type Sprite struct {
	Tex      *sdl.Texture
	Src, Dst *sdl.Rect
	Angle    float64
	Center   ce2d_math.Vector2D
	Flip     sdl.RendererFlip
}

func (s *Sprite) LoadTexture(app *gdl.App, tag string, path string) {
	s.Tex, _ = app.LoadTexture(tag, path)
}

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
