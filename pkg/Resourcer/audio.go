package Resourcer

import (
	"github.com/veandco/go-sdl2/mix"
)

/////////////////////////////////////////////////////
// FONT
/////////////////////////////////////////////////////

func LoadMusic(tag string, path string) (*mix.Music, error) {
	m, err := mix.LoadMUS(path)
	if err != nil {
		return nil, err
	}
	resourcer.containerMusic[tag] = m
	return m, nil
}

func LoadChunk(tag string, path string) (*mix.Chunk, error) {
	m, err := mix.LoadWAV(path)
	if err != nil {
		return nil, err
	}
	resourcer.containerChunk[tag] = m
	return m, nil
}

func HasMusic(tag string) bool {
	return resourcer.containerMusic[tag] != nil
}

func HasChunk(tag string) bool {
	return resourcer.containerChunk[tag] != nil
}

func UnLoadMusic(tag string) {
	resourcer.containerMusic[tag].Free()
	delete(resourcer.containerMusic, tag)
}

func UnLoadAllMusic() {
	for i, v := range resourcer.containerMusic {
		v.Free()
		delete(resourcer.containerMusic, i)
	}
}

func UnLoadChunk(tag string) {
	resourcer.containerChunk[tag].Free()
	delete(resourcer.containerChunk, tag)
}

func UnLoadAllChunk() {
	for i, v := range resourcer.containerChunk {
		v.Free()
		delete(resourcer.containerChunk, i)
	}
}

func GetMusic(tag string) *mix.Music {
	return resourcer.containerMusic[tag]
}

func GetChunk(tag string) *mix.Chunk {
	return resourcer.containerChunk[tag]
}
