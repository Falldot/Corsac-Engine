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

func Translate(pool ecs.Pool, dt float64) {
	entities := pool.GetGroup(ecs.AllOf(TRANSFORM))
	for _, v := range entities.Entities() {
		t := v.Get(TRANSFORM).(*Transform)
		fmt.Printf("t: %v\n", t)
	}
}

func main() {
	pool := ecs.CreatePool()

	pool.AddSystem(Translate)

	pool.CreateEntity().
		Add(TRANSFORM, &Transform{20, 30}).
		Add(VELOCITY, &Velocity{70, 10}).
		Add(DIRECTION, &Direction{500, 0})

	pool.CreateEntity().
		Add(TRANSFORM, &Transform{10, 1})

	pool.ExecuteSystems(1)

}
