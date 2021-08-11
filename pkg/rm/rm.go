package rm

import (
	"github.com/Falldot/ce2d/pkg/ce2d_math"
	"github.com/veandco/go-sdl2/img"
	"github.com/veandco/go-sdl2/mix"
	"github.com/veandco/go-sdl2/sdl"
	"github.com/veandco/go-sdl2/ttf"
)

type ResourceManager struct {
	*sdl.Window
	*sdl.Renderer

	containerTexture map[string]*sdl.Texture
	containerFonts   map[string]*ttf.Font
	containerMusic   map[string]*mix.Music
	containerChunk   map[string]*mix.Chunk

	containerSprites ce2d_math.List
}

func Init() (*ResourceManager, error) {

	if err := sdl.Init(sdl.INIT_VIDEO | sdl.INIT_AUDIO | sdl.INIT_EVENTS | sdl.INIT_TIMER); err != nil {
		return nil, err
	}
	if err := img.Init(img.INIT_PNG | img.INIT_JPG); err != nil {
		return nil, err
	}
	if err := ttf.Init(); err != nil {
		return nil, err
	}
	if err := mix.Init(mix.INIT_MP3); err != nil {
		return nil, err
	}
	if err := mix.OpenAudio(44100, mix.DEFAULT_FORMAT, 2, 2048); err != nil {
		return nil, err
	}
	window, err := sdl.CreateWindow("", sdl.WINDOWPOS_UNDEFINED, sdl.WINDOWPOS_UNDEFINED,
		800, 600, 0)
	if err != nil {
		return nil, err
	}
	renderer, err := sdl.CreateRenderer(window, -1, sdl.RENDERER_ACCELERATED)
	if err != nil {
		return nil, err
	}

	return &ResourceManager{
		Window:   window,
		Renderer: renderer,

		containerTexture: make(map[string]*sdl.Texture),
		containerFonts:   make(map[string]*ttf.Font),
		containerMusic:   make(map[string]*mix.Music),
		containerChunk:   make(map[string]*mix.Chunk),

		containerSprites: *ce2d_math.CreateList(nil),
	}, nil
}

func (rm *ResourceManager) AllUnload() {
	rm.UnLoadAllTexture()
	rm.UnLoadAllChunk()
	rm.UnLoadAllFonts()
	rm.UnLoadAllMusic()
}

func (rm *ResourceManager) Destroy() {
	rm.Renderer.Destroy()
	rm.Window.Destroy()
	mix.CloseAudio()
	mix.Quit()
	ttf.Quit()
	img.Quit()
	sdl.Quit()
}

func (rm *ResourceManager) Lenght() (int, int, int, int) {
	t := len(rm.containerTexture)
	f := len(rm.containerFonts)
	m := len(rm.containerMusic)
	c := len(rm.containerChunk)
	return t, f, m, c
}
