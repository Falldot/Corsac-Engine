package ce2d_video

import (
	"github.com/Falldot/ce2d/pkg/ce2d_math"
	"github.com/veandco/go-sdl2/sdl"
)

type Texture struct {
	Sprite   *sdl.Texture
	Src, Dst *sdl.Rect
	Angle    float64
	Center   ce2d_math.Vector2D
	Flip     sdl.RendererFlip
}

func CreateTexture(sprite *sdl.Texture, width, height int32, angle float64, scale int32) *Texture {
	return &Texture{
		Sprite: sprite,
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

func (t *Texture) SetDst(x, y int32) {
	t.Dst.X = x
	t.Dst.Y = y
}

func (t *Texture) SetSizeDst(width, height int32) {
	t.Dst.W = width
	t.Dst.H = height
}

func (t *Texture) SetSrc(x, y int32) {
	t.Src.X = x
	t.Src.Y = y
}

func (t *Texture) SetSizeSrc(width, height int32) {
	t.Src.W = width
	t.Src.H = height
}

func (t *Texture) Draw() {

}
