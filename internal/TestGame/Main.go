package testgame

import (
	"github.com/Falldot/ce2d/internal/ce2d_ecs"
	"github.com/Falldot/ce2d/pkg/ecs"
	"github.com/Falldot/ce2d/pkg/gdl"
	"github.com/veandco/go-sdl2/sdl"
)

type Main struct {
	pool *ecs.Pool
	app  *gdl.App

	render *ce2d_ecs.SDLRender

	err error
}

func (s *Main) Load() {
	s.app, s.err = gdl.CreateWindow()
	if s.err != nil {
		panic(s.err)
	}

	s.pool = ce2d_ecs.CreatePool()
	s.pool.RegComponent(ce2d_ecs.Components())
	s.pool.Init()

	s.render = ce2d_ecs.NewRender()
	s.render.Init(s.pool)

	s.app.LoadSystemFont("Arial", 14)
	s.app.LoadTexture("fox", "./assets/fox.png")

	s.pool.CreateEvent(
		func(event *ecs.Event) bool {
			return gdl.KeyDown(sdl.SCANCODE_Q)
		},
		func(event *ecs.Event) {
			s.pool.DestroyEvent(event)
			s.programStatus(event)
		})

	CreatePlayer(s.pool, s.app)
	CreateNPC(s.pool, s.app)

}
func (s *Main) Loop() {

	gdl.Window.Show()

	s.app.Start(
		60,
		60,
		func(dt float64) {
			s.pool.ChangeEvents()
			s.pool.Update(dt)
		},
		func() {
			s.render.Update()
		},
	)
}

func (s *Main) UnLoad() {
	s.pool = nil
	s.app.Destroy()
	s.app = nil
}
