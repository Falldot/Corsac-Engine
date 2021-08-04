package gdl

import (
	"runtime"
	"strconv"

	"github.com/veandco/go-sdl2/sdl"
	"github.com/veandco/go-sdl2/ttf"
)

/////////////////////////////////////////////////////
// FONT
/////////////////////////////////////////////////////

func (app *App) LoadSystemFont(tag string, size int) error {
	var path string
	if runtime.GOOS == "windows" {
		path = `C:\Windows\Fonts\` + tag + `.ttf`
	}
	fullTag := tag + `_` + strconv.Itoa(size)
	font, err := ttf.OpenFont(path, size)
	if err != nil {
		return err
	}
	app.containerFonts[fullTag] = font
	return nil
}

func (app *App) LoadFont(tag string, path string, size int) error {
	font, err := ttf.OpenFont(path, size)
	if err != nil {
		return err
	}
	fullTag := tag + `_` + strconv.Itoa(size)
	app.containerFonts[fullTag] = font
	return nil
}

func (win *App) CreateTextTexture(font string, size int, tag string, text string, color sdl.Color) (*sdl.Texture, *sdl.Rect, error) {
	fullTag := font + `_` + strconv.Itoa(size)
	surf, err := win.containerFonts[fullTag].RenderUTF8Blended(text, color)
	if err != nil {
		return nil, nil, err
	}
	tex, err := Render.CreateTextureFromSurface(surf)
	if err != nil {
		return nil, nil, err
	}
	defer surf.Free()
	win.containerTexture[tag] = tex
	return tex, &sdl.Rect{W: surf.W, H: surf.H}, nil
}

func (app *App) UnLoadFonts(tag string) {
	app.containerFonts[tag].Close()
	delete(app.containerFonts, tag)
}

func (app *App) UnLoadAllFonts() {
	for i, v := range app.containerFonts {
		v.Close()
		delete(app.containerFonts, i)
	}
}

func (app *App) HasFont(tag string) bool {
	return app.containerFonts[tag] != nil
}
