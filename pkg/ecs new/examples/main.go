package main

import (
	"fmt"

	"github.com/Falldot/ce2d/pkg/ecs"
)

const (
	TRANSFORM = iota
	VELOCITY
	SPRITE
)

type Transform struct {
	X, Y float64
}

type Velocity struct {
	X, Y float64
}

type Sprite struct {
	X, Y float64
}

// Translate

type Translate struct {
	*ecs.Entity
}

func (t *Translate) SetPool(pool *ecs.Pool) {
	t.Entity = pool.GetEntities(TRANSFORM)
}

func (t *Translate) Execute() {
	t.Entity.Map(func(e *ecs.Entity) {
		fmt.Printf("e.GetComponent(1): %v\n", e.GetComponent(TRANSFORM))
	})
}

// Draw

type Draw struct {
	*ecs.Entity
}

func (t *Draw) SetPool(pool *ecs.Pool) {
	t.Entity = pool.GetEntities(VELOCITY)
}

func (t *Draw) Execute() {
	t.Entity.Map(func(e *ecs.Entity) {
		fmt.Printf("e.GetComponent(2): %v\n", e.GetComponent(VELOCITY))
	})
}

// Control

type Control struct {
	*ecs.Entity
}

func (t *Control) SetPool(pool *ecs.Pool) {
	t.Entity = pool.GetEntities(SPRITE)
}

func (t *Control) Execute() {
	t.Entity.Map(func(e *ecs.Entity) {
		fmt.Printf("e.GetComponent(3): %v\n", e.GetComponent(SPRITE))
	})
}

func main() {
	pool := ecs.NewPool()

	pool.RegComponent(TRANSFORM)
	pool.RegComponent(VELOCITY)
	pool.RegComponent(SPRITE)

	pool.RegSystem(new(Translate), new(Draw), new(Control))

	obj := pool.CreateEntity()
	pool.AddComponent(obj, TRANSFORM, &Transform{30, 20})
	pool.AddComponent(obj, VELOCITY, &Transform{100, 200})
	obj2 := pool.CreateEntity()
	pool.AddComponent(obj2, TRANSFORM, &Transform{100, 200})
	pool.AddComponent(obj2, SPRITE, &Transform{100, 200})

	//fmt.Printf("pool.GetEntities(TRANSFORM): %v\n", pool.GetEntities(TRANSFORM).Lenght())

	//pool.SetPoolSystems()

	//pool.ExecuteSystems()
}
