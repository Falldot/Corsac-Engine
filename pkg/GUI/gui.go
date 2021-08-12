package GUI

import (
	"github.com/veandco/go-sdl2/img"
	"github.com/veandco/go-sdl2/mix"
	"github.com/veandco/go-sdl2/sdl"
	"github.com/veandco/go-sdl2/ttf"
)

var (
	Window   *sdl.Window
	Renderer *sdl.Renderer
)

func Init() error {
	if err := sdl.Init(sdl.INIT_VIDEO | sdl.INIT_AUDIO | sdl.INIT_EVENTS | sdl.INIT_TIMER); err != nil {
		return err
	}
	if err := img.Init(img.INIT_PNG | img.INIT_JPG); err != nil {
		return err
	}
	if err := ttf.Init(); err != nil {
		return err
	}
	if err := mix.Init(mix.INIT_MP3); err != nil {
		return err
	}
	if err := mix.OpenAudio(44100, mix.DEFAULT_FORMAT, 2, 2048); err != nil {
		return err
	}
	window, err := sdl.CreateWindow("", sdl.WINDOWPOS_UNDEFINED, sdl.WINDOWPOS_UNDEFINED,
		800, 600, 0)
	if err != nil {
		return err
	}
	renderer, err := sdl.CreateRenderer(window, -1, sdl.RENDERER_ACCELERATED)
	if err != nil {
		return err
	}

	Window = window
	Renderer = renderer

	return nil
}

func Destroy() {
	Renderer.Destroy()
	Window.Destroy()
	mix.CloseAudio()
	mix.Quit()
	ttf.Quit()
	img.Quit()
	sdl.Quit()
}
