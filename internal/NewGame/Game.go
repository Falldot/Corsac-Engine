package newgame

import (
	"github.com/Falldot/ce2d/internal/NewGame/Scene"
	"github.com/Falldot/ce2d/pkg/ecs"
	"github.com/Falldot/ce2d/pkg/rm"
)

func Play() {
	err := rm.Init()
	if err != nil {
		panic(err)
	}

	pool := ecs.CreatePool()

	Scene.Main(pool)

	pool.Init()
	rm.ResM.Gameloop(
		60,
		60,
		func(dt float64) {
			pool.Execute(dt)
		},
		func() {
			pool.Clean()
		},
	)
	pool.Exit()
}
