package ce2d_video

type VideoManager struct {
	Textures [][]*Texture
}

func Create() *VideoManager {
	return &VideoManager{
		Textures: make([][]*Texture, 0),
	}
}

func (v *VideoManager) AddTexture(texture *Texture, level int) {
	v.Textures[level] = append(v.Textures[level], texture)
}

func (v *VideoManager) Draw() {
	for _, level := range v.Textures {
		for _, Texture := range level {
			Texture.Draw()
		}
	}
}
