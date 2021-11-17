package app

// #include "window.h"
import "C"

func Init() error {
	C.corsac_create_window()
	C.corsac_input()
	return nil
}

func DisplaySize(x int32, y int32) {}

func WindowTitle(name string) {}

func WindowIcon(path string) error {
	return nil
}

func Update() {}

func Destroy() {}
