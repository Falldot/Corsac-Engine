package Scene

import (
	system "github.com/Falldot/ce2d/internal/NewGame/System"
	"github.com/Falldot/ce2d/pkg/ecs"
)

// @<-Main
func Main(pool ecs.Pool) {
	pool.AddSystem(system.InitorLoadResources, system.InitorCreaterUnits, system.ExiterUnLoadResources)

	pool.AddSystem(system.TranslateGetter, system.TranslateExecuter)
	pool.AddSystem(system.RendererGetter, system.RendererExecuter)
	pool.AddSystem(system.AnimatedGetter, system.AnimatedExecuter)
	pool.AddSystem(system.ControlPlayerGetter, system.ControlPlayerExecuter)
	pool.AddSystem(system.CollisionGetter, system.CollisionExecuter)
}
