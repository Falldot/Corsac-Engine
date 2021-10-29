package ECS

type Matcher interface {
	Matches(entity Entity) bool
	Hash() uint
	ComponentTypes() []int
}

type baseMatcher struct {
	componentTypes []int
	hash           uint
}
