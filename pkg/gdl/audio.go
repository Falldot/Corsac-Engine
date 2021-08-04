package gdl

import (
	"github.com/veandco/go-sdl2/mix"
)

/////////////////////////////////////////////////////
// FONT
/////////////////////////////////////////////////////

func (app *App) LoadMusic(tag string, path string) (*mix.Music, error) {
	m, err := mix.LoadMUS(path)
	if err != nil {
		return nil, err
	}
	app.containerMusic[tag] = m
	return m, nil
}

func (app *App) LoadChunk(tag string, path string) (*mix.Chunk, error) {
	m, err := mix.LoadWAV(path)
	if err != nil {
		return nil, err
	}
	app.containerChunk[tag] = m
	return m, nil
}

func (app *App) HasMusic(tag string) bool {
	return app.containerMusic[tag] != nil
}

func (app *App) HasChunk(tag string) bool {
	return app.containerChunk[tag] != nil
}

func (app *App) UnLoadMusic(tag string) {
	app.containerMusic[tag].Free()
	delete(app.containerMusic, tag)
}

func (app *App) UnLoadAllMusic(tag string) {
	for i, v := range app.containerMusic {
		v.Free()
		delete(app.containerMusic, i)
	}
}

func (app *App) UnLoadChunk(tag string) {
	app.containerChunk[tag].Free()
	delete(app.containerChunk, tag)
}

func (app *App) UnLoadAllChunk(tag string) {
	for i, v := range app.containerChunk {
		v.Free()
		delete(app.containerChunk, i)
	}
}

func (app *App) GetMusic(tag string) *mix.Music {
	return app.containerMusic[tag]
}

func (app *App) GetChunk(tag string) *mix.Chunk {
	return app.containerChunk[tag]
}
