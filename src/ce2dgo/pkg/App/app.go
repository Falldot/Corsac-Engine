package App

import (
	"github.com/Falldot/ce2d/pkg/ECS"
	"github.com/Falldot/ce2d/pkg/Event"
	"github.com/Falldot/ce2d/pkg/GUI"
	"github.com/Falldot/ce2d/pkg/Resourcer"
	"github.com/veandco/go-sdl2/sdl"
)

var (
	IdleThreshold   float64 = 1
	FPSGameUpdate   float64 = 60
	FPSRenderUpdate float64 = 60

	quit bool
)

func Root(root ECS.Initer) {
	err := GUI.Init()
	if err != nil {
		panic(err)
	}
	Resourcer.Init()
	ECS.Init()
	root()

	gameLoop()
}

func getTicks() float64 {
	return float64(sdl.GetTicks()) * 0.001
}

func gameLoop() {
	ECS.ExecuteIniters()

	var current, elapsed float64
	previous := getTicks()
	secsPerUpdate := 1 / float64(FPSGameUpdate)
	lag := 0.0

	for quit = false; !quit; {
		current = getTicks()
		elapsed = current - previous
		previous = current

		if elapsed > IdleThreshold {
			continue
		}

		quit = Event.Update()

		lag += elapsed

		for lag >= secsPerUpdate {
			GUI.Renderer.Clear()
			ECS.ExecuteExecuters(secsPerUpdate)
			ECS.ExecuteCleaners()
			lag -= secsPerUpdate
		}
		GUI.Renderer.Present()
	}
	ECS.ExecuteExiters()
}

func Exit() {
	quit = false
}
