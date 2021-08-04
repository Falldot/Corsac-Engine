package gdl

var (
	currentScene int

	containerScene map[int]Scene
)

type Scene interface {
	Load()
	Loop()
	UnLoad()
}

func NewScene(tag int, scene Scene) {
	containerScene[tag] = scene
}

func LoadScene(tag int) {
	containerScene[tag].Load()
}

func PlayScene(tag int) {
	if currentScene != 0 {
		containerScene[currentScene].UnLoad()
	}
	currentScene = tag
	containerScene[currentScene].Loop()
}
