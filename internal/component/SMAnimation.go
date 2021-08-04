package component

type animation struct {
	Index, Frames, Speed int32
}

func (a *animation) Set(b *animation) {
	a.Index = b.Index
	a.Frames = b.Frames
	a.Speed = b.Speed
}

type SMAnimation struct {
	Current    *animation
	Animations map[string]*animation
}

func CreateSMAnimation(action func(anim *SMAnimation)) *SMAnimation {
	a := &SMAnimation{
		Current:    &animation{},
		Animations: make(map[string]*animation),
	}
	action(a)
	return a
}

func (s *SMAnimation) Play(name string) {
	s.Current.Set(s.Animations[name])
}

func (s *SMAnimation) AddAnimation(name string, x int32, y int32, speed int32) {
	anim := &animation{Index: x, Frames: y, Speed: speed}
	s.Animations[name] = anim
}
