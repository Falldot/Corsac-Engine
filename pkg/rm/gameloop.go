package rm

import (
	"fmt"

	"github.com/veandco/go-sdl2/sdl"
)

var (
	gameEvent      sdl.Event
	mouseX, mouseY int32
	mouseEvent     uint32
	typeEvent      uint32

	FramePerSecond string

	keys []uint8

	keyDown  [512]bool
	keyPress [512]bool
	keyUp    [512]bool
)

func (rm *ResourceManager) Gameloop(fps uint16, limitfps uint32, update func(float64), render func()) {

	secsPerUpdate := 1 / float64(fps)
	var IdleThreshold, current, elapsed float64
	IdleThreshold = 1
	previous := float64(sdl.GetTicks()) * 0.001
	lag := 0.0

	var frameStart, frameTime uint32
	limitFps := limitfps
	frameDelay := uint32(1000 / limitFps)

	//keys := sdl.GetKeyboardState()

	for quit := false; !quit; {

		frameStart = sdl.GetTicks()
		current = float64(sdl.GetTicks()) * 0.001
		elapsed = current - previous
		previous = current

		FramePerSecond = fmt.Sprintf("%.0f", 1/elapsed)

		if elapsed > IdleThreshold {
			continue
		}

		frameTime = sdl.GetTicks() - frameStart

		if frameDelay > frameTime {
			sdl.Delay(frameDelay - frameTime)
		}

		lag += elapsed

		for gameEvent = sdl.PollEvent(); gameEvent != nil; gameEvent = sdl.PollEvent() {
			typeEvent = gameEvent.GetType()
			if typeEvent == sdl.QUIT {
				fmt.Println("QUIT")
				quit = true
			}
			keys = sdl.GetKeyboardState()
			mouseX, mouseY, mouseEvent = sdl.GetMouseState()
		}

		for lag >= secsPerUpdate {
			Renderer.Clear()
			keysManager()
			update(secsPerUpdate)
			lag -= secsPerUpdate
		}

		render()
		Renderer.Present()
	}
}

func GetFPS() string {
	return FramePerSecond
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

func keysManager() {
	for i, k := range keys {
		keyUp[i] = false
		keyDown[i] = false

		if k != 0 {
			if !keyPress[i] {
				keyDown[i] = true
			}
			keyPress[i] = true
		}

		if k == 0 && keyPress[i] {
			keyPress[i] = false
			keyUp[i] = true
		}
	}
}

func KeyDown(event ...sdl.Keycode) bool {
	for _, v := range event {
		if keyDown[v] {
			return true
		}
	}
	return false
}

func KeyDownMod(event ...uint32) bool {
	var count int
	for _, v := range event {
		if keyDown[v] {
			count++
		}
		if count == len(event) {
			return true
		}
	}
	return false
}

func KeyPress(event ...sdl.Keycode) bool {
	for _, v := range event {
		if keyPress[v] {
			return true
		}
	}
	return false
}

func KeyPressMod(event ...uint32) bool {
	var count int
	for _, v := range event {
		if keyPress[v] {
			count++
		}
		if count == len(event) {
			return true
		}
	}
	return false
}

func KeyUp(event ...sdl.Keycode) bool {
	for _, v := range event {
		if keyUp[v] {
			return true
		}
	}
	return false
}

func KeyUpMod(event ...uint32) bool {
	var count int
	for _, v := range event {
		if keyUp[v] {
			count++
		}
		if count == len(event) {
			return true
		}
	}
	return false
}
