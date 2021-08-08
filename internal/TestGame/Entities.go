package testgame

import (
	"strconv"

	"github.com/Falldot/ce2d/internal/ce2d_ecs"
	"github.com/Falldot/ce2d/pkg/ecs"
	"github.com/Falldot/ce2d/pkg/gdl"
	"github.com/veandco/go-sdl2/sdl"
)

func CreatePlayer(pool *ecs.Pool, app *gdl.App) {
	player := pool.CreateEntity()
	pool.AddComponent(player, ce2d_ecs.TRANSFORM,
		ce2d_ecs.CreateTransform(100, 100, 300),
	)
	pool.AddComponent(player, ce2d_ecs.SPRITE,
		ce2d_ecs.CreateSprite(app.GetTexture("fox"), 32, 32, 0, 2),
	)
	pool.AddComponent(player, ce2d_ecs.ANIMATION,
		ce2d_ecs.CreateAnimation(func(anim *ce2d_ecs.Animation) {
			anim.AddAnimation("Idle", 0, 6, 300)
			anim.AddAnimation("Right", 1, 6, 100)
			anim.AddAnimation("Down", 2, 6, 100)
			anim.AddAnimation("Up", 3, 6, 100)
			anim.AddAnimation("DownRight", 4, 6, 100)
			anim.Play("Idle")
		}),
	)
	pool.AddComponent(player, ce2d_ecs.CONTROL,
		ce2d_ecs.CreateControl(0, 0),
	)
}

func CreateNPC(pool *ecs.Pool, app *gdl.App) {
	npc := pool.CreateEntity()
	pool.AddComponent(npc, ce2d_ecs.TRANSFORM,
		ce2d_ecs.CreateTransform(300, 300, 300),
	)
	pool.AddComponent(npc, ce2d_ecs.SPRITE,
		ce2d_ecs.CreateSprite(app.GetTexture("fox"), 32, 32, 0, 2),
	)
	pool.AddComponent(npc, ce2d_ecs.ANIMATION,
		ce2d_ecs.CreateAnimation(func(anim *ce2d_ecs.Animation) {
			anim.AddAnimation("Idle", 0, 6, 300)
			anim.AddAnimation("Right", 1, 6, 100)
			anim.AddAnimation("Down", 2, 6, 100)
			anim.AddAnimation("Up", 3, 6, 100)
			anim.AddAnimation("DownRight", 4, 6, 100)
			anim.Play("Idle")
		}),
	)
}

func (s *Main) programStatus(creater *ecs.Event) {

	/// Mouse

	stringFps := gdl.GetFPS()

	fps, fpsrect, _ := s.app.CreateTextTexture("Arial", 14, "fps", "FPS: "+stringFps, sdl.Color{0, 255, 0, 255})

	fpsBoard := s.pool.CreateEntity()
	s.pool.AddComponent(fpsBoard, ce2d_ecs.TRANSFORM,
		ce2d_ecs.CreateTransform(0, 0, 0),
	)
	s.pool.AddComponent(fpsBoard, ce2d_ecs.SPRITE,
		ce2d_ecs.CreateSprite(fps, fpsrect.W, fpsrect.H, 0, 1),
	)

	/// Mouse

	x, y := gdl.Mouse()
	str := "X: " + strconv.Itoa(int(x)) + " / " + "Y: " + strconv.Itoa(int(y))

	mouse, mouserect, _ := s.app.CreateTextTexture("Arial", 14, "mouseBoard", str, sdl.Color{0, 255, 0, 255})

	mouseBoard := s.pool.CreateEntity()
	s.pool.AddComponent(mouseBoard, ce2d_ecs.TRANSFORM,
		ce2d_ecs.CreateTransform(0, float64(fpsrect.H), 0),
	)
	s.pool.AddComponent(mouseBoard, ce2d_ecs.SPRITE,
		ce2d_ecs.CreateSprite(mouse, mouserect.W, mouserect.H, 0, 1),
	)

	/// ECS

	sys, _, c := s.pool.LenghtData()
	str3 := "Systems: " + strconv.Itoa(sys) + " / " + "Events: " + strconv.Itoa(c)

	ecsd, ecsdrect, _ := s.app.CreateTextTexture("Arial", 14, "ecsd", str3, sdl.Color{0, 255, 0, 255})

	ecsdStatus := s.pool.CreateEntity()
	s.pool.AddComponent(ecsdStatus, ce2d_ecs.TRANSFORM,
		ce2d_ecs.CreateTransform(0, float64(mouserect.H)+float64(fpsrect.H), 0),
	)
	s.pool.AddComponent(ecsdStatus, ce2d_ecs.SPRITE,
		ce2d_ecs.CreateSprite(ecsd, ecsdrect.W, ecsdrect.H, 0, 1),
	)

	/// Resource

	t, f, m, c := s.app.GetAllResourceLenght()
	str2 := "Textures: " + strconv.Itoa(t) + " / " + "Fonts: " + strconv.Itoa(f) + " / " + "Music: " + strconv.Itoa(m) + " / " + "Chunks: " + strconv.Itoa(c)

	resource, resourcerect, _ := s.app.CreateTextTexture("Arial", 14, "Resource", str2, sdl.Color{0, 255, 0, 255})

	resourceStatus := s.pool.CreateEntity()
	s.pool.AddComponent(resourceStatus, ce2d_ecs.TRANSFORM,
		ce2d_ecs.CreateTransform(0, float64(ecsdrect.H)+float64(mouserect.H)+float64(fpsrect.H), 0),
	)
	s.pool.AddComponent(resourceStatus, ce2d_ecs.SPRITE,
		ce2d_ecs.CreateSprite(resource, resourcerect.W, resourcerect.H, 0, 1),
	)

	mouseMove := s.pool.CreateEvent(
		func(event *ecs.Event) bool {
			return true
		},
		func(event *ecs.Event) {

			stringFps := gdl.GetFPS()
			s.app.UnLoadTexture("fps")
			fps, fpsrect, _ := s.app.CreateTextTexture("Arial", 14, "fps", "FPS: "+stringFps, sdl.Color{0, 255, 0, 255})
			fpsBoard.Replace(ce2d_ecs.SPRITE, ce2d_ecs.CreateSprite(fps, fpsrect.W, fpsrect.H, 0, 1))

			x, y := gdl.Mouse()
			str := "X: " + strconv.Itoa(int(x)) + " / " + "Y: " + strconv.Itoa(int(y))
			s.app.UnLoadTexture("mouseBoard")
			mouse, mouserect, _ = s.app.CreateTextTexture("Arial", 14, "mouseBoard", str, sdl.Color{0, 255, 0, 255})
			mouseBoard.Replace(ce2d_ecs.SPRITE, ce2d_ecs.CreateSprite(mouse, mouserect.W, mouserect.H, 0, 1))

			sys, _, c := s.pool.LenghtData()
			str3 := "Systems: " + strconv.Itoa(sys) + " / " + "Events: " + strconv.Itoa(c)
			s.app.UnLoadTexture("ecsd")
			ecsd, ecsdrect, _ := s.app.CreateTextTexture("Arial", 14, "ecsd", str3, sdl.Color{0, 255, 0, 255})
			ecsdStatus.Replace(ce2d_ecs.SPRITE, ce2d_ecs.CreateSprite(ecsd, ecsdrect.W, ecsdrect.H, 0, 1))

			t, f, m, c := s.app.GetAllResourceLenght()
			str2 := "Textures: " + strconv.Itoa(t) + " / " + "Fonts: " + strconv.Itoa(f) + " / " + "Music: " + strconv.Itoa(m) + " / " + "Chunks: " + strconv.Itoa(c)
			s.app.UnLoadTexture("Resource")
			resource, resourcerect, _ := s.app.CreateTextTexture("Arial", 14, "Resource", str2, sdl.Color{0, 255, 0, 255})
			resourceStatus.Replace(ce2d_ecs.SPRITE, ce2d_ecs.CreateSprite(resource, resourcerect.W, resourcerect.H, 0, 1))

		})

	s.pool.LazyCreateEvent(
		func(event *ecs.Event) bool {
			return gdl.KeyDown(sdl.SCANCODE_Q)
		},
		func(event *ecs.Event) {
			s.pool.RemoveEntity(fpsBoard)
			s.app.UnLoadTexture("fps")

			s.pool.DestroyEvent(mouseMove)
			s.pool.RemoveEntity(mouseBoard)
			s.app.UnLoadTexture("mouseBoard")

			s.pool.RemoveEntity(ecsdStatus)
			s.app.UnLoadTexture("ecsd")

			s.pool.RemoveEntity(resourceStatus)
			s.app.UnLoadTexture("Resource")
			s.pool.DestroyEvent(event)

			s.pool.LazyRebornEvent(creater)
		})
}
