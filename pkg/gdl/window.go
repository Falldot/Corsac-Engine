package gdl

import (
	"github.com/veandco/go-sdl2/img"
	"github.com/veandco/go-sdl2/mix"
	"github.com/veandco/go-sdl2/sdl"
	"github.com/veandco/go-sdl2/ttf"
)

var (
	Window *sdl.Window
	Render *sdl.Renderer
)

type App struct {
	containerTexture map[string]*sdl.Texture
	containerFonts   map[string]*ttf.Font
	containerMusic   map[string]*mix.Music
	containerChunk   map[string]*mix.Chunk
}

func Init() {
	containerScene = make(map[int]Scene)
}

func CreateWindow() (*App, error) {
	var err error
	app := &App{}
	app.containerFonts = make(map[string]*ttf.Font)
	app.containerTexture = make(map[string]*sdl.Texture)
	app.containerMusic = make(map[string]*mix.Music)
	app.containerChunk = make(map[string]*mix.Chunk)
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
	Window, err = sdl.CreateWindow("", sdl.WINDOWPOS_UNDEFINED, sdl.WINDOWPOS_UNDEFINED,
		800, 600, 0)
	if err != nil {
		return nil, err
	}
	Render, err = sdl.CreateRenderer(Window, -1, sdl.RENDERER_ACCELERATED)
	if err != nil {
		return nil, err
	}
	return app, nil
}

func (app *App) Destroy() {
	app.UnLoadAllTexture()
	app.UnLoadAllChunk()
	app.UnLoadAllFonts()
	app.UnLoadAllMusic()
}

func Destroy() {
	Render.Destroy()
	Window.Destroy()
	mix.CloseAudio()
	mix.Quit()
	ttf.Quit()
	img.Quit()
	sdl.Quit()
}

func (app *App) GetAllResourceLenght() (int, int, int, int) {
	t := len(app.containerTexture)
	f := len(app.containerFonts)
	m := len(app.containerMusic)
	c := len(app.containerChunk)
	return t, f, m, c
}
