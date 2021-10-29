package Resourcer

import (
	"github.com/veandco/go-sdl2/mix"
	"github.com/veandco/go-sdl2/sdl"
	"github.com/veandco/go-sdl2/ttf"
)

var resourcer *ResourceManager

type ResourceManager struct {
	containerTexture map[string]*sdl.Texture
	containerFonts   map[string]*ttf.Font
	containerMusic   map[string]*mix.Music
	containerChunk   map[string]*mix.Chunk
}

func Init() {
	resourcer = &ResourceManager{
		containerTexture: make(map[string]*sdl.Texture),
		containerFonts:   make(map[string]*ttf.Font),
		containerMusic:   make(map[string]*mix.Music),
		containerChunk:   make(map[string]*mix.Chunk),
	}
}

func AllUnload() {
	UnLoadAllTexture()
	UnLoadAllChunk()
	UnLoadAllFonts()
	UnLoadAllMusic()
}

func CountAllLoadTextures() int {
	return len(resourcer.containerTexture)
}

func CountAllLoadFonts() int {
	return len(resourcer.containerFonts)
}

func CountAllLoadMusic() int {
	return len(resourcer.containerMusic)
}

func CountAllLoadChunk() int {
	return len(resourcer.containerChunk)
}
