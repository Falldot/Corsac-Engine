package ecs

type system struct {
	getter   Getter
	filter   Filter
	trigger  Trigger
	executer Executer
}

var getter Getter = func(pool Pool) Group { return nil }
var filter Filter = func(entity Entity) bool { return true }
var trigger Trigger = func(pool Pool) bool { return true }

func createSystem() *system {
	return &system{
		getter:  getter,
		filter:  filter,
		trigger: trigger,
	}
}

// @Initer
type Initer func(pool Pool)

// @Getter
type Getter func(pool Pool) Group

// @Filter
type Filter func(entity Entity) bool

// @Trigger
type Trigger func(pool Pool) bool

// @Executer
type Executer func(pool Pool, group Group, dt float64)

// @Cleaner
type Cleaner func(pool Pool)

// @Exiter
type Exiter func(pool Pool)
