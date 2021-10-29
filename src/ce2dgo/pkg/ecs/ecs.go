package ECS

var p Pool

func Init() {
	p = createPool()
}

func CreateEntity() Entity {
	return p.CreateEntity()
}

func AddSystem(actions ...interface{}) {
	p.AddSystem(actions...)
}

func ExecuteIniters() {
	p.Init()
}

func ExecuteExecuters(dt float64) {
	p.Execute(dt)
}

func ExecuteCleaners() {
	p.Clean()
}

func ExecuteExiters() {
	p.Exit()
}

func GetGroup(matchers ...Matcher) Group {
	return p.GetGroup(matchers...)
}

func GetCountSystems() (int, int, int, int) {
	i := p.Initers()
	e := p.Exiters()
	c := p.Cleaners()
	ex := p.Exiters()
	return i, e, c, ex
}

func GetCountEntities() int {
	return p.Entities()
}

func GetCountGroups() int {
	return p.Groups()
}
