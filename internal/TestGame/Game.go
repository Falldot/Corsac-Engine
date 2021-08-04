package testgame

import (
	"github.com/Falldot/ce2d/pkg/gdl"
)

const (
	NONE = iota
	MAIN
)

func Play() {
	gdl.Init()
	gdl.NewScene(MAIN, &Main{})
	gdl.LoadScene(MAIN)
	gdl.PlayScene(MAIN)
	defer gdl.Destroy()
}
