package Event

import (
	"github.com/veandco/go-sdl2/sdl"
)

var (
	mouseX, mouseY int32
	mouseEvent     uint32

	keys []uint8

	keyDown  [512]bool
	keyPress [512]bool
	keyUp    [512]bool
)

func Init() {}

func Update() bool {

	sdl.PumpEvents()

	for gameEvent := sdl.PollEvent(); gameEvent != nil; gameEvent = sdl.PollEvent() {
		switch gameEvent.GetType() {
		case sdl.QUIT:
			return true
		case sdl.KEYDOWN:
			keys = sdl.GetKeyboardState()
			keysManager()
		case sdl.KEYUP:
			keys = sdl.GetKeyboardState()
			keysManager()
		case sdl.MOUSEMOTION:
			mouseX, mouseY, mouseEvent = sdl.GetMouseState()
		case sdl.MOUSEBUTTONDOWN:

		case sdl.MOUSEBUTTONUP:

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
