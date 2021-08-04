package gdl

import (
	"fmt"

	"github.com/veandco/go-sdl2/sdl"
)

var (
	gameEvent      sdl.Event
	mouseX, mouseY int32
	mouseEvent     uint32
	keys           []uint8
	typeEvent      uint32

	keyDown [512]bool
	keyKeep [512]bool
	keyUp   [512]bool
)

func (app *App) Start(fps uint16, update func(float64), render func()) {

	secsPerUpdate := 1 / float64(fps)
	var IdleThreshold, current, elapsed float64
	IdleThreshold = 1

	previous := float64(sdl.GetTicks()) * 0.001
	lag := 0.0

	//keys := sdl.GetKeyboardState()

	for quit := false; !quit; {
		current = float64(sdl.GetTicks()) * 0.001
		elapsed = current - previous
		previous = current

		if elapsed > IdleThreshold {
			continue
		}

		lag += elapsed

		for gameEvent = sdl.PollEvent(); gameEvent != nil; gameEvent = sdl.PollEvent() {
			if typeEvent == sdl.QUIT {
				fmt.Println("QUIT")
				quit = true
			}
			mouseX, mouseY, mouseEvent = sdl.GetMouseState()
			typeEvent = gameEvent.GetType()

			keys = sdl.GetKeyboardState()
			keysManager()
		}

		for lag >= secsPerUpdate {
			update(secsPerUpdate)
			lag -= secsPerUpdate
		}

		Render.Clear()
		render()
		Render.Present()
	}
}

func keysManager() {
	for i, v := range keys {
		if keyDown[i] {
			keyDown[i] = false
		}
		if keyUp[i] {
			keyUp[i] = false
		}
		if v != 0 {
			keyDown[i] = true
			keyKeep[i] = true
		} else {
			keyKeep[i] = false
			keyUp[i] = true
		}
	}
}

func Mouse() (x, y int32) {
	return mouseX, mouseY
}

func MousePress(event ...uint32) bool {
	for _, v := range event {
		if v == sdl.MOUSEMOTION {
			if typeEvent == sdl.MOUSEMOTION {
				return true
			}
		}
		if v == mouseEvent {
			return true
		}
	}
	return false
}

func MousePressMod(event ...uint32) bool {
	var count int
	for _, v := range event {
		if v == sdl.MOUSEMOTION {
			if typeEvent == sdl.MOUSEMOTION {
				count++
			}
		}
		if v == mouseEvent {
			count++
		}
		if count == len(event) {
			return true
		}
	}
	return false
}

func KeyPress(event ...uint32) bool {
	for _, v := range event {
		if keys[v] != 0 {
			return true
		}
	}
	return false
}

func KeyPressMod(event ...uint32) bool {
	var count int
	for _, v := range event {
		if keys[v] != 0 {
			count++
		}
		if count == len(event) {
			return true
		}
	}
	return false
}
