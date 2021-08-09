package rm

import (
	"github.com/Falldot/ce2d/pkg/ce2d_math"
	"github.com/veandco/go-sdl2/sdl"
)

type sprite struct {
	Texture  *sdl.Texture
	Src, Dst *sdl.Rect
	Angle    float64
	Center   ce2d_math.Vector2D
	Flip     sdl.RendererFlip
}

func (rm *ResourceManager) AddSprite(tagTexture string, x, y, width, height int32, angle float64, scale int32) {
	sp := &sprite{
		Texture: rm.GetTexture(tagTexture),
		Src: &sdl.Rect{
			W: width,
			H: height,
		},
		Dst: &sdl.Rect{
			X: x,
			Y: y,
			W: width * scale,
			H: height * scale,
		},
		Angle: angle,
	}

	rm.containerSprites.Add(sp)
}

func (rm *ResourceManager) Draw() {
	rm.Renderer.Clear()
	rm.containerSprites.ForEach(func(l *ce2d_math.List) {
		if l.Value != nil {
			sp := l.Value.(*sprite)
			rm.Renderer.CopyEx(sp.Texture, sp.Src, sp.Dst, sp.Angle, nil, sp.Flip)
		}
	})
	rm.Renderer.Present()
	rm.containerSprites.Clear()
}
