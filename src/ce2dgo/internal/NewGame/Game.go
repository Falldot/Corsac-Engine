package newgame

import (
	system "github.com/Falldot/ce2d/internal/NewGame/System"
	"github.com/Falldot/ce2d/pkg/App"
)

// @<-Play
func Play() {
	App.Root(system.GameController)
}
