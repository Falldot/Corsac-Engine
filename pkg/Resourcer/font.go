package Resourcer

import (
	"runtime"
	"strconv"

	"github.com/Falldot/ce2d/pkg/GUI"
	"github.com/veandco/go-sdl2/sdl"
	"github.com/veandco/go-sdl2/ttf"
)

/////////////////////////////////////////////////////
// FONT
/////////////////////////////////////////////////////

func LoadSystemFont(tag string, size int) error {
	var path string
	if runtime.GOOS == "windows" {
		path = `C:\Windows\Fonts\` + tag + `.ttf`
	}
	fullTag := tag + `_` + strconv.Itoa(size)
	font, err := ttf.OpenFont(path, size)
	if err != nil {
		return err
	}
	resourcer.containerFonts[fullTag] = font
	return nil
}

func LoadFont(tag string, path string, size int) error {
	font, err := ttf.OpenFont(path, size)
	if err != nil {
		return err
	}
	fullTag := tag + `_` + strconv.Itoa(size)
	resourcer.containerFonts[fullTag] = font
	return nil
}

func CreateTextTexture(font string, size int, tag string, text string, color sdl.Color) (*sdl.Texture, *sdl.Rect, error) {
	fullTag := font + `_` + strconv.Itoa(size)
	surf, err := resourcer.containerFonts[fullTag].RenderUTF8Blended(text, color)
	if err != nil {
		return nil, nil, err
	}
	tex, err := GUI.Renderer.CreateTextureFromSurface(surf)
	if err != nil {
		return nil, nil, err
	}
	defer surf.Free()
	resourcer.containerTexture[tag] = tex
	return tex, &sdl.Rect{W: surf.W, H: surf.H}, nil
}

func UnLoadFonts(tag string) {
	resourcer.containerFonts[tag].Close()
	delete(resourcer.containerFonts, tag)
}

func UnLoadAllFonts() {
	for i, v := range resourcer.containerFonts {
		v.Close()
		delete(resourcer.containerFonts, i)
	}
}

func HasFont(tag string) bool {
	return resourcer.containerFonts[tag] != nil
}
