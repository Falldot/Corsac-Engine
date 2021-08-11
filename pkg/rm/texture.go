package rm

import (
	"github.com/veandco/go-sdl2/img"
	"github.com/veandco/go-sdl2/sdl"
)

/////////////////////////////////////////////////////
// IMAGE
/////////////////////////////////////////////////////

func (rm *ResourceManager) LoadTexture(tag string, path string) (*sdl.Texture, error) {
	surf, err := img.Load(path)
	if err != nil {
		return nil, err
	}
	tex, err := rm.Renderer.CreateTextureFromSurface(surf)
	if err != nil {
		return nil, err
	}
	surf.Free()
	rm.containerTexture[tag] = tex
	return tex, nil
}

func (rm *ResourceManager) HasTexture(tag string) bool {
	return rm.containerTexture[tag] != nil
}

func (rm *ResourceManager) UnLoadTexture(tag string) {
	rm.containerTexture[tag].Destroy()
	delete(rm.containerTexture, tag)
}

func (rm *ResourceManager) UnLoadAllTexture() {
	for i, v := range rm.containerTexture {
		v.Destroy()
		delete(rm.containerTexture, i)
	}
}

func (rm *ResourceManager) GetTexture(tag string) *sdl.Texture {
	return rm.containerTexture[tag]
}
