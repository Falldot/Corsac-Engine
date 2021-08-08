package ce2d_ecs

import (
	"github.com/Falldot/ce2d/pkg/ecs"
	"github.com/veandco/go-sdl2/sdl"
)

type (
	Animation struct {
		Current    *animation
		Animations map[string]*animation
	}

	animation struct {
		Index, Frames, Speed int32
	}

	sysAnimation struct {
		comps ecs.Entities
	}
)

func (a *animation) Set(b *animation) {
	a.Index = b.Index
	a.Frames = b.Frames
	a.Speed = b.Speed
}

func CreateAnimation(action func(anim *Animation)) *Animation {
	a := &Animation{
		Current:    &animation{},
		Animations: make(map[string]*animation),
	}
	action(a)
	return a
}

func (sys *Animation) AddAnimation(name string, x int32, y int32, speed int32) {
	anim := &animation{Index: x, Frames: y, Speed: speed}
	sys.Animations[name] = anim
}

func (sys *Animation) Play(name string) {
	sys.Current.Set(sys.Animations[name])
}

func (sys *sysAnimation) Init(pool *ecs.Pool) {
	sys.comps = pool.GetEntities(ANIMATION)
}

func (sys *sysAnimation) Update(dt float64) {
	for _, v := range sys.comps {
		s := v.Get(SPRITE)
		sm := v.Get(ANIMATION)
		s.(*Sprite).Src.X = s.(*Sprite).Src.W * (int32(sdl.GetTicks()) / sm.(*Animation).Current.Speed % sm.(*Animation).Current.Frames)
		s.(*Sprite).Src.Y = sm.(*Animation).Current.Index * s.(*Sprite).Src.H
	}
}
