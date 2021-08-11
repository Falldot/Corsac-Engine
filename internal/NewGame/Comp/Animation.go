package Comp

// @Comp
type Animation struct {
	Current    *animation
	Animations map[string]*animation
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

type animation struct {
	Index, Frames, Speed int32
}

func (a *animation) Set(b *animation) {
	a.Index = b.Index
	a.Frames = b.Frames
	a.Speed = b.Speed
}
