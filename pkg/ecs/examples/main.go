package main

import (
	"fmt"

	ecs "github.com/Falldot/ce2d/pkg/newecs"
)

type Transform struct {
	X, Y float64
}

type Velocity struct {
	X, Y float64
}

type Direction struct {
	X, Y float64
}

const (
	TRANSFORM = iota
	VELOCITY
	DIRECTION
)

// @Initor
var Translate_Initor ecs.Initer = func(pool ecs.Pool) {
	pool.CreateEntity().
		Add(TRANSFORM, &Transform{20, 30}).
		Add(VELOCITY, &Velocity{70, 10}).
		Add(DIRECTION, &Direction{500, 0})

	pool.CreateEntity().
		Add(TRANSFORM, &Transform{10, 1})
}

// @Getter
var Translate_Getter ecs.Getter = func(pool ecs.Pool) ecs.Group {
	return pool.GetGroup(ecs.AllOf(TRANSFORM))
}

// @Filter
var Translate_Filter ecs.Filter = func(entity ecs.Entity) bool {
	return entity.Get(TRANSFORM).(*Transform).X >= 10
}

// @Trigger
var Translate_Trigger ecs.Trigger = func(pool ecs.Pool) bool {
	return true
}

// @Executer
var Translate_Executer ecs.Executer = func(pool ecs.Pool, group ecs.Group, dt float64) {
	group.ForEach(func(e ecs.Entity) {
		fmt.Println(e.Get(TRANSFORM))
	})
}

// @Cleaner
var Translate_Cleaner ecs.Cleaner = func(pool ecs.Pool) {
	fmt.Println("Cleaner")
}

// @Exiter
var Translate_Exiter ecs.Exiter = func(pool ecs.Pool) {
	fmt.Println("Exiter")
}

func main() {
	pool := ecs.CreatePool()

	pool.AddSystem(Translate_Initor, Translate_Getter, Translate_Filter, Translate_Trigger, Translate_Executer, Translate_Cleaner, Translate_Exiter)

	pool.Init()
	pool.Execute(1)
	pool.Clean()
	pool.Exit()
}
