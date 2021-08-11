package gdl

import (
	"github.com/Falldot/ce2d/pkg/ce2d_math"
	"github.com/veandco/go-sdl2/sdl"
)

type Sprite struct {
	Texture  *sdl.Texture
	Src, Dst *sdl.Rect
	Angle    float64
	Center   ce2d_math.Vector2D
	Flip     sdl.RendererFlip
}

func (app *App) CreateSprite(tagTexture string, width, height int32, angle float64, scale int32) *Sprite {
	return &Sprite{
		Texture: app.GetTexture(tagTexture),
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
}
