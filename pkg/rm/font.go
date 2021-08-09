package rm

import (
	"runtime"
	"strconv"

	"github.com/veandco/go-sdl2/sdl"
	"github.com/veandco/go-sdl2/ttf"
)

/////////////////////////////////////////////////////
// FONT
/////////////////////////////////////////////////////

func (rm *ResourceManager) LoadSystemFont(tag string, size int) error {
	var path string
	if runtime.GOOS == "windows" {
		path = `C:\Windows\Fonts\` + tag + `.ttf`
	}
	fullTag := tag + `_` + strconv.Itoa(size)
	font, err := ttf.OpenFont(path, size)
	if err != nil {
		return err
	}
	rm.containerFonts[fullTag] = font
	return nil
}

func (rm *ResourceManager) LoadFont(tag string, path string, size int) error {
	font, err := ttf.OpenFont(path, size)
	if err != nil {
		return err
	}
	fullTag := tag + `_` + strconv.Itoa(size)
	rm.containerFonts[fullTag] = font
	return nil
}

func (rm *ResourceManager) CreateTextTexture(font string, size int, tag string, text string, color sdl.Color) (*sdl.Texture, *sdl.Rect, error) {
	fullTag := font + `_` + strconv.Itoa(size)
	surf, err := rm.containerFonts[fullTag].RenderUTF8Blended(text, color)
	if err != nil {
		return nil, nil, err
	}
	tex, err := rm.Renderer.CreateTextureFromSurface(surf)
	if err != nil {
		return nil, nil, err
	}
	defer surf.Free()
	rm.containerTexture[tag] = tex
	return tex, &sdl.Rect{W: surf.W, H: surf.H}, nil
}

func (rm *ResourceManager) UnLoadFonts(tag string) {
	rm.containerFonts[tag].Close()
	delete(rm.containerFonts, tag)
}

func (rm *ResourceManager) UnLoadAllFonts() {
	for i, v := range rm.containerFonts {
		v.Close()
		delete(rm.containerFonts, i)
	}
}

func (rm *ResourceManager) HasFont(tag string) bool {
	return rm.containerFonts[tag] != nil
}
