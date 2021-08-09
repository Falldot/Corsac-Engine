package rm

import (
	"github.com/veandco/go-sdl2/mix"
)

/////////////////////////////////////////////////////
// FONT
/////////////////////////////////////////////////////

func (rm *ResourceManager) LoadMusic(tag string, path string) (*mix.Music, error) {
	m, err := mix.LoadMUS(path)
	if err != nil {
		return nil, err
	}
	rm.containerMusic[tag] = m
	return m, nil
}

func (rm *ResourceManager) LoadChunk(tag string, path string) (*mix.Chunk, error) {
	m, err := mix.LoadWAV(path)
	if err != nil {
		return nil, err
	}
	rm.containerChunk[tag] = m
	return m, nil
}

func (rm *ResourceManager) HasMusic(tag string) bool {
	return rm.containerMusic[tag] != nil
}

func (rm *ResourceManager) HasChunk(tag string) bool {
	return rm.containerChunk[tag] != nil
}

func (rm *ResourceManager) UnLoadMusic(tag string) {
	rm.containerMusic[tag].Free()
	delete(rm.containerMusic, tag)
}

func (rm *ResourceManager) UnLoadAllMusic() {
	for i, v := range rm.containerMusic {
		v.Free()
		delete(rm.containerMusic, i)
	}
}

func (rm *ResourceManager) UnLoadChunk(tag string) {
	rm.containerChunk[tag].Free()
	delete(rm.containerChunk, tag)
}

func (rm *ResourceManager) UnLoadAllChunk() {
	for i, v := range rm.containerChunk {
		v.Free()
		delete(rm.containerChunk, i)
	}
}

func (rm *ResourceManager) GetMusic(tag string) *mix.Music {
	return rm.containerMusic[tag]
}

func (rm *ResourceManager) GetChunk(tag string) *mix.Chunk {
	return rm.containerChunk[tag]
}
