package main

import (
	"fmt"

	ECS "github.com/Falldot/ce2d/pkg/NewECS"
)

func main() {
	ECS.Init()

	a := ECS.CreateEntity()
	ECS.CreateEntity().Destroy()
	ECS.CreateEntity().Destroy()
	ECS.CreateEntity().Destroy()
	ECS.CreateEntity()

	a.Destroy()

	ECS.CreateEntity()

	fmt.Printf("ECS.LenghtEntities(): %v\n", ECS.LenghtEntities())
	fmt.Printf("ECS.LenghtEntitiesCashe(): %v\n", ECS.LenghtEntitiesCashe())
	fmt.Printf("ECS.LenghtEntitiesPool(): %v\n", ECS.LenghtEntitiesPool())

}
