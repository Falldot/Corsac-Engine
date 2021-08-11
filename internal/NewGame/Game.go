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

			// fmt.Printf("pool.Entities(): %v\n", pool.Entities())
			// fmt.Printf("pool.Cemetery(): %v\n", pool.Cemetery())
			// fmt.Printf("pool.Systems(): %v\n", pool.Systems())
			// fmt.Printf("pool.Initers(): %v\n", pool.Initers())
			// fmt.Printf("pool.Cleaners(): %v\n", pool.Cleaners())
			// fmt.Printf("pool.Exiters(): %v\n", pool.Exiters())
			// fmt.Printf("pool.Groups(): %v\n", pool.Groups())
			// fmt.Printf("pool.GroupsIndex(): %v\n", pool.GroupsIndex())

			// fmt.Println(core.ResM.Lenght())

			pool.Execute(dt)
		},
		func() {
			pool.Clean()
		},
	)
	pool.Exit()
}
