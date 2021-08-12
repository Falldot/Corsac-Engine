package main

import (
	"fmt"

	"github.com/Falldot/ce2d/pkg/ECS"
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
var Translate_Initor ECS.Initer = func() {
	ECS.CreateEntity().
		Add(TRANSFORM, &Transform{20, 30}).
		Add(VELOCITY, &Velocity{70, 10}).
		Add(DIRECTION, &Direction{500, 0})

	ECS.CreateEntity().
		Add(TRANSFORM, &Transform{10, 1})
}

// @Getter
var Translate_Getter ECS.Getter = func() ECS.Group {
	return ECS.GetGroup(ECS.AllOf(TRANSFORM))
}

// @Filter
var Translate_Filter ECS.Filter = func(entity ECS.Entity) bool {
	return entity.Get(TRANSFORM).(*Transform).X >= 10
}

// @Trigger
var Translate_Trigger ECS.Trigger = func() bool {
	return true
}

// @Executer
var Translate_Executer ECS.Executer = func(group ECS.Group, dt float64) {
	group.ForEach(func(e ECS.Entity) {
		fmt.Println(e.Get(TRANSFORM))
	})
}

// @Cleaner
var Translate_Cleaner ECS.Cleaner = func() {
	fmt.Println("Cleaner")
}

// @Exiter
var Translate_Exiter ECS.Exiter = func() {
	fmt.Println("Exiter")
}

func main() {
	ECS.Init()

	ECS.AddSystem(Translate_Initor, Translate_Getter, Translate_Filter, Translate_Trigger, Translate_Executer, Translate_Cleaner, Translate_Exiter)

	ECS.ExecuteIniters()
	ECS.ExecuteExecuters(1)
	ECS.ExecuteCleaners()
	ECS.ExecuteExiters()
}
