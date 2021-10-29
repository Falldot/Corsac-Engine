package ecs

type (
	Loader interface {
		Load(group []Entity)
	}
	Executer interface {
		Update(delta float64)
	}
	Drawer interface {
		Draw()
	}
	Exiter interface {
		UnLoad()
	}
)

type System struct {
	group  *Group
	script interface{}
}

func Script(script interface{}) System {
	return System{
		group:  CreateGroup(),
		script: script,
	}
}

func MakeScript(length int, script interface{}) System {
	return System{
		group:  MakeGroup(length),
		script: script,
	}
}

func (sys *System) Load() {
	sys.script.(Loader).Load(sys.group.packed)
}

func (sys *System) Update() {
	sys.script.(Executer).Update(1)
}

func (sys *System) Draw() {
	sys.script.(Drawer).Draw()
}

func (sys *System) UnLoad() {
	sys.script.(Exiter).UnLoad()
}
