package ce2d_video

import "github.com/Falldot/ce2d/pkg/ce2d_error"

type VideoManager struct {
	Textures [][]*Texture
}

func Create() *VideoManager {
	return &VideoManager{
		Textures: make([][]*Texture, 0),
	}
}

func (v *VideoManager) AddLevel() {
	v.Textures = append(v.Textures, []*Texture{})
}

func (v *VideoManager) AddTexture(texture *Texture, level int) {
	if len(v.Textures) > level {
		v.Textures[level] = append(v.Textures[level], texture)
	} else {
		ce2d_error.Error("Добавляете текстуру к уровню которого нет")
	}
}

func (v *VideoManager) Draw() {
	for _, level := range v.Textures {
		for _, Texture := range level {
			Texture.Draw()
		}
	}
}
