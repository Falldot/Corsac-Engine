package Resourcer

import (
	"github.com/Falldot/ce2d/pkg/GUI"
	"github.com/veandco/go-sdl2/img"
	"github.com/veandco/go-sdl2/sdl"
)

/////////////////////////////////////////////////////
// IMAGE
/////////////////////////////////////////////////////

func LoadTexture(tag string, path string) (*sdl.Texture, error) {
	surf, err := img.Load(path)
	if err != nil {
		return nil, err
	}
	tex, err := GUI.Renderer.CreateTextureFromSurface(surf)
	if err != nil {
		return nil, err
	}
	surf.Free()
	resourcer.containerTexture[tag] = tex
	return tex, nil
}

func HasTexture(tag string) bool {
	return resourcer.containerTexture[tag] != nil
}

func UnLoadTexture(tag string) {
	resourcer.containerTexture[tag].Destroy()
	delete(resourcer.containerTexture, tag)
}

func UnLoadAllTexture() {
	for i, v := range resourcer.containerTexture {
		v.Destroy()
		delete(resourcer.containerTexture, i)
	}
}

func GetTexture(tag string) *sdl.Texture {
	return resourcer.containerTexture[tag]
}
