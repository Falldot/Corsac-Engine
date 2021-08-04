package testgame

import (
	"strconv"

	"github.com/Falldot/ce2d/internal/component"
	"github.com/Falldot/ce2d/internal/system"
	"github.com/Falldot/ce2d/pkg/ecs"
	"github.com/Falldot/ce2d/pkg/gdl"
	"github.com/veandco/go-sdl2/sdl"
)

type Main struct {
	pool *ecs.Pool
	app  *gdl.App

	err error
}

func (s *Main) Load() {
	s.pool = ecs.CreatePool()
	s.app, s.err = gdl.CreateWindow()
	if s.err != nil {
		panic(s.err)
	}

	s.app.LoadSystemFont("Arial", 14)

	s.pool.RegComponent(component.TRANSFORM, component.SPRITE, component.SMANIMATION, component.CONTROL)

	s.pool.CreateEvent(
		func(event *ecs.Event) bool {
			return gdl.KeyPress(sdl.SCANCODE_Q)
		},
		func(event *ecs.Event) {
			s.programStatus()
			s.pool.DestroyEvent(event)
		})

	playerTex, err := s.app.LoadTexture("player", "./assets/fox.png")
	if err != nil {
		panic(err)
	}

	player := s.pool.CreateEntity()
	s.pool.AddComponent(player, component.TRANSFORM,
		component.CreateTransform(100, 100, 0),
	)
	s.pool.AddComponent(player, component.SPRITE,
		component.CreateSprite(playerTex, 32, 32, 0, 2),
	)
	s.pool.AddComponent(player, component.SMANIMATION,
		component.CreateSMAnimation(func(anim *component.SMAnimation) {
			anim.AddAnimation("Idle", 0, 6, 300)
			anim.AddAnimation("Right", 1, 6, 100)
			anim.AddAnimation("Down", 2, 6, 100)
			anim.AddAnimation("Up", 3, 6, 100)
			anim.AddAnimation("DownRight", 4, 6, 100)
			anim.Play("Idle")
		}),
	)
	s.pool.AddComponent(player, component.CONTROL,
		component.CreateControl(0, 0),
	)

	s.pool.CreateEvent(
		func(e *ecs.Event) bool {
			return gdl.KeyPress(sdl.SCANCODE_SPACE)
		},
		func(e *ecs.Event) {

			s.pool.DestroyEvent(e)
		},
	)

}
func (s *Main) Loop() {

	Translate := &system.Translate{}
	//PlayerControl := &system.PlayerControl{}
	KeyboardController := &system.KeyboardController{}
	Draw := &system.Draw{}
	StopMotion := &system.StopMotion{}

	s.pool.SetSystem(
		//PlayerControl,
		KeyboardController,
		Translate,
		StopMotion,
		Draw,
	)

	s.pool.SetPoolSystems()

	gdl.Window.Show()

	s.app.Start(
		60,
		func(dt float64) {
			//PlayerControl.Execute()
			Translate.Execute()

			s.pool.ChangeEvents()

			StopMotion.Execute()
		},
		func() {
			Draw.Execute()
		})

}

func (s *Main) UnLoad() {
	s.pool = nil
	s.app.Destroy()
	s.app = nil
}

func (s *Main) programStatus() {

	/// Mouse

	x, y := gdl.Mouse()
	str := "X: " + strconv.Itoa(int(x)) + " / " + "Y: " + strconv.Itoa(int(y))

	mouse, mouserect, _ := s.app.CreateTextTexture("Arial", 14, "mouseBoard", str, sdl.Color{0, 255, 0, 255})

	mouseBoard := s.pool.CreateEntity()
	s.pool.AddComponent(mouseBoard, component.TRANSFORM,
		component.CreateTransform(0, 0, 0),
	)
	s.pool.AddComponent(mouseBoard, component.SPRITE,
		component.CreateSprite(mouse, mouserect.W, mouserect.H, 0, 1),
	)

	/// ECS

	sys, _, c := s.pool.LenghtData()
	str3 := "Systems: " + strconv.Itoa(sys) + " / " + "Events: " + strconv.Itoa(c)

	ecsd, ecsdrect, _ := s.app.CreateTextTexture("Arial", 14, "ecsd", str3, sdl.Color{0, 255, 0, 255})

	ecsdStatus := s.pool.CreateEntity()
	s.pool.AddComponent(ecsdStatus, component.TRANSFORM,
		component.CreateTransform(0, float64(mouserect.H), 0),
	)
	s.pool.AddComponent(ecsdStatus, component.SPRITE,
		component.CreateSprite(ecsd, ecsdrect.W, ecsdrect.H, 0, 1),
	)

	/// Resource

	t, f, m, c := s.app.GetAllResourceLenght()
	str2 := "Textures: " + strconv.Itoa(t) + " / " + "Fonts: " + strconv.Itoa(f) + " / " + "Music: " + strconv.Itoa(m) + " / " + "Chunks: " + strconv.Itoa(c)

	resource, resourcerect, _ := s.app.CreateTextTexture("Arial", 14, "Resource", str2, sdl.Color{0, 255, 0, 255})

	resourceStatus := s.pool.CreateEntity()
	s.pool.AddComponent(resourceStatus, component.TRANSFORM,
		component.CreateTransform(0, float64(ecsdrect.H)+float64(mouserect.H), 0),
	)
	s.pool.AddComponent(resourceStatus, component.SPRITE,
		component.CreateSprite(resource, resourcerect.W, resourcerect.H, 0, 1),
	)

	mouseMove := s.pool.CreateEvent(
		func(event *ecs.Event) bool {
			return true
		},
		func(event *ecs.Event) {
			x, y := gdl.Mouse()
			str := "X: " + strconv.Itoa(int(x)) + " / " + "Y: " + strconv.Itoa(int(y))
			s.app.UnLoadTexture("FPS")
			mouse, mouserect, _ = s.app.CreateTextTexture("Arial", 14, "mouseBoard", str, sdl.Color{0, 255, 0, 255})
			mouseBoard.Replace(component.SPRITE, component.CreateSprite(mouse, mouserect.W, mouserect.H, 0, 1))

			sys, _, c := s.pool.LenghtData()
			str3 := "Systems: " + strconv.Itoa(sys) + " / " + "Events: " + strconv.Itoa(c)
			s.app.UnLoadTexture("ecsd")
			ecsd, ecsdrect, _ := s.app.CreateTextTexture("Arial", 14, "ecsd", str3, sdl.Color{0, 255, 0, 255})
			ecsdStatus.Replace(component.SPRITE, component.CreateSprite(ecsd, ecsdrect.W, ecsdrect.H, 0, 1))

			t, f, m, c := s.app.GetAllResourceLenght()
			str2 := "Textures: " + strconv.Itoa(t) + " / " + "Fonts: " + strconv.Itoa(f) + " / " + "Music: " + strconv.Itoa(m) + " / " + "Chunks: " + strconv.Itoa(c)
			s.app.UnLoadTexture("Resource")
			resource, resourcerect, _ := s.app.CreateTextTexture("Arial", 14, "Resource", str2, sdl.Color{0, 255, 0, 255})
			resourceStatus.Replace(component.SPRITE, component.CreateSprite(resource, resourcerect.W, resourcerect.H, 0, 1))

		})

	s.pool.CreateEvent(
		func(event *ecs.Event) bool {
			return !gdl.KeyPressMod(sdl.SCANCODE_Q)
		},
		func(event *ecs.Event) {
			s.pool.DestroyEvent(event)
			s.pool.CreateEvent(
				func(event *ecs.Event) bool {
					return gdl.KeyPressMod(sdl.SCANCODE_Q)
				},
				func(event *ecs.Event) {

					s.pool.DestroyEvent(mouseMove)
					s.pool.RemoveEntity(mouseBoard)
					s.app.UnLoadTexture("mouseBoard")

					s.pool.RemoveEntity(ecsdStatus)
					s.app.UnLoadTexture("ecsd")

					s.pool.RemoveEntity(resourceStatus)
					s.app.UnLoadTexture("Resource")
					s.pool.DestroyEvent(event)

					s.pool.CreateEvent(
						func(event *ecs.Event) bool {
							return !gdl.KeyPressMod(sdl.SCANCODE_Q)
						},
						func(event *ecs.Event) {
							s.pool.DestroyEvent(event)
							s.pool.CreateEvent(
								func(event *ecs.Event) bool {
									return gdl.KeyPress(sdl.SCANCODE_Q)
								},
								func(event *ecs.Event) {
									s.pool.DestroyEvent(event)
									s.programStatus()
								})
						},
					)

				})
		})

}
