package main

import (
	"fmt"

	"github.com/Falldot/ce2d/src/ecs"
)

const (
	hpPerStr = 20
	epPerAgl = 20
	mpPerInt = 20
)

type TypeAbility int

const (
	Damage TypeAbility = iota
	Recreation
)

const (
	attack = "attack"
	relax  = "relax"
)

type TAbilities []*TAbility

type TAbility struct {
	Type       TypeAbility
	Name       string
	strModif   float32
	aglModif   float32
	intModif   float32
	costEnergy int
	costMana   int
	active     func(int, *ecs.Entity, *ecs.Entity)
}

func (a *TAbility) Activate(e1, e2 *ecs.Entity) {
	if e1.Get(CEnergy).(int) < a.costEnergy || e1.Get(CMana).(int) < a.costMana {
		return
	}
	a.payMana(e1)
	a.payEnergy(e1)
	a.active(a.calculateModifer(e1), e1, e2)
}

func (a *TAbility) payMana(e *ecs.Entity) {
	mp := e.Take(CMana)
	*mp = (*mp).(int) - a.costMana
}

func (a *TAbility) payEnergy(e *ecs.Entity) {
	ep := e.Take(CEnergy)
	*ep = (*ep).(int) - a.costEnergy
}

func (a *TAbility) СalculateModifer(e *ecs.Entity) int {
	return int(float32(e.Get(CAgility).(int))*a.aglModif +
		float32(e.Get(CStrength).(int))*a.strModif +
		float32(e.Get(CIntellect).(int))*a.intModif)
}

type TAI func(*ecs.Entity, *ecs.Entity)

func (ai *TAI) Call(e1, e2 *ecs.Entity) {
	*ai = func(e1, e2 *ecs.Entity) {

	}
}

///////////////////////////////////////////////////////////

var (
	CAbility   *ecs.IComponent = ecs.Component()
	CName      *ecs.IComponent = ecs.Component()
	CHealth    *ecs.IComponent = ecs.Component()
	CEnergy    *ecs.IComponent = ecs.Component()
	CMana      *ecs.IComponent = ecs.Component()
	CStrength  *ecs.IComponent = ecs.Component()
	CAgility   *ecs.IComponent = ecs.Component()
	CIntellect *ecs.IComponent = ecs.Component()
	CAI        *ecs.IComponent = ecs.Component()
)

// type SBlackGoblinAI struct {
// 	entities []ecs.Entity
// }

// func (sys *SBlackGoblinAI) Load(entities []ecs.Entity) {
// 	sys.entities = entities
// }

// func (sys *SBlackGoblinAI) Update(delta float64) {

// }

// func (sys *SBlackGoblinAI) Draw() {

// }

// func (sys *SBlackGoblinAI) UnLoad() {

// }

func CreatePerson(name string, aiPatter func(*ecs.Entity, *ecs.Entity)) *ecs.Entity {
	person := ecs.GameObject()
	return person.Add(CStrength, 10).
		Add(CAgility, 10).
		Add(CIntellect, 10).
		Add(CAbility,
			TAbilities{
				&TAbility{Damage, attack, 0, 1, 0, 10, 0, func(value int, e1, e2 *ecs.Entity) {
					hp2 := e2.Take(CHealth)
					*hp2 = (*hp2).(int) - value
				}},
				&TAbility{Recreation, relax, 0, 1, 0, 0, 0, func(value int, e1, e2 *ecs.Entity) {
					en2 := e1.Take(CEnergy)
					*en2 = (*en2).(int) + value
				}},
			},
		).
		Add(CHealth, person.Get(CStrength).(int)*hpPerStr).
		Add(CMana, person.Get(CIntellect).(int)*mpPerInt).
		Add(CEnergy, person.Get(CAgility).(int)*epPerAgl).
		Add(CName, name).
		Add(CAI, aiPatter)
}

func AIPattern(e1, e2 *ecs.Entity) {
	hp := e1.Get(CAbility);
	var target TypeAbility
	if 

	for _, ability := range e1.Get(CAbility).(TAbilities) {

	}

	if e1.Get(CAbility).(TAbilities)[0].costMana > e1.Get(CMana) ||
		e1.Get(CAbility).(TAbilities)[0].costEnergy > e1.Get(CEnergy) {
		e1.Get(CAbility).(TAbilities)[1].Activate(e1, e2)
	} else {
		e1.Get(CAbility).(TAbilities)[0].Activate(e1, e2)
	}
}

func Battle(e1, e2 *ecs.Entity) *ecs.Entity {
	var winner *ecs.Entity = nil
	for winner == nil {
		if e1.Get(CHealth).(int) <= 0 {
			winner = e2
		}
		e1.Get(CAI).(func(*ecs.Entity, *ecs.Entity))(e1, e2)
		if e2.Get(CHealth).(int) <= 0 {
			winner = e1
		}
		e2.Get(CAI).(func(*ecs.Entity, *ecs.Entity))(e2, e1)
	}
	return winner
}

func main() {
	//app.Init()

	person := CreatePerson("Markus", AIPattern)
	enemy := CreatePerson("Black Goblin", AIPattern)

	fmt.Printf("Победил %v!\n", Battle(person, enemy).Get(CName))
}
