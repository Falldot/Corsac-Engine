package gdl

import (
	"github.com/veandco/go-sdl2/img"
	"github.com/veandco/go-sdl2/sdl"
)

/////////////////////////////////////////////////////
// IMAGE
/////////////////////////////////////////////////////

func (app *App) LoadTexture(tag string, path string) (*sdl.Texture, error) {
	surf, err := img.Load(path)
	if err != nil {
		return nil, err
	}
	tex, err := Render.CreateTextureFromSurface(surf)
	if err != nil {
		return nil, err
	}
	surf.Free()
	app.containerTexture[tag] = tex
	return tex, nil
}

func (app *App) HasTexture(tag string) bool {
	return app.containerTexture[tag] != nil
}

func (app *App) UnLoadTexture(tag string) {
	app.containerTexture[tag].Destroy()
	delete(app.containerTexture, tag)
}

func (app *App) UnLoadAllTexture() {
	for i, v := range app.containerTexture {
		v.Destroy()
		delete(app.containerTexture, i)
	}
}

func (app *App) GetTexture(tag string) *sdl.Texture {
	return app.containerTexture[tag]
}
