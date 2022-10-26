package ECS

type system struct {
	getter   Getter
	filter   Filter
	trigger  Trigger
	executer Executer
}

var getter Getter = func() Group { return nil }
var trigger Trigger = func() bool { return true }

func createSystem() *system {
	return &system{
		getter:  getter,
		filter:  nil,
		trigger: trigger,
	}
}

// @Initer
type Initer func()

// @Getter
type Getter func() Group

// @Filter
type Filter func(entity Entity) bool

// @Trigger
type Trigger func() bool

// @Executer
type Executer func(group Group, dt float64)

// @Cleaner
type Cleaner func()

// @Exiter
type Exiter func()
