package ce2d_ecs

import (
	"github.com/Falldot/ce2d/pkg/ecs"
)

const (
	TRANSFORM ecs.TypeComponent = iota
	RECTCOLLIDER
	CONTROL
	SPRITE
	ANIMATION
)

func CreatePool() *ecs.Pool {
	return ecs.CreatePool(
		new(sysTransform),
		new(sysAnimation),
		new(sysControl),
	)
}

func Components() []ecs.TypeComponent {
	return []ecs.TypeComponent{
		TRANSFORM,
		SPRITE,
		ANIMATION,
		CONTROL,
	}
}

func NewRender() *SDLRender {
	return new(SDLRender)
}
