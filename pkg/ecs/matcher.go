package ECS

import (
	"sort"
)

const (
	componentHashFactor uint = 647
	allHashFactor            = 653
	anyHashFactor            = 659
	noneHashFactor           = 661
	arrayHashFactor          = 673
)

type Matcher interface {
	Matches(entity Entity) bool
	Hash() uint
	ComponentTypes() []int
}

type baseMatcher struct {
	componentTypes []int
	hash           uint
}

func (b *baseMatcher) ComponentTypes() []int {
	return b.componentTypes
}

func newBaseMatcher(componentTypes ...int) baseMatcher {
	mtype := make(map[int]bool)
	for _, t := range componentTypes {
		mtype[t] = true
	}

	componentTypes = make([]int, 0, len(mtype))
	for t := range mtype {
		componentTypes = append(componentTypes, t)
	}
	sort.IntsAreSorted(componentTypes)

	return baseMatcher{componentTypes: componentTypes}
}

func (b *baseMatcher) Hash() uint {
	return b.hash
}

// AllOf
type AllMatcher struct {
	baseMatcher
}

func AllOf(types ...int) Matcher {
	b := newBaseMatcher(types...)

	b.hash = Hash(allHashFactor, b.ComponentTypes()...)
	return &AllMatcher{b}
}

func (a *AllMatcher) Matches(entity Entity) bool {
	return entity.has(a.ComponentTypes()...)
}

// AnyOf
type AnyMatcher struct {
	baseMatcher
}

func AnyOf(types ...int) Matcher {
	b := newBaseMatcher(types...)
	b.hash = Hash(anyHashFactor, b.ComponentTypes()...)
	return &AnyMatcher{b}
}

func (a *AnyMatcher) Matches(entity Entity) bool {
	return entity.has(a.ComponentTypes()...)
}

// Utilities
func Hash(factor uint, types ...int) uint {
	var hash uint
	for _, t := range types {
		hash ^= uint(t) * componentHashFactor
	}
	hash ^= uint(len(types)) * factor
	return uint(hash)
}

func HashMatcher(matchers ...Matcher) uint {
	if len(matchers) == 1 {
		return matchers[0].Hash()
	}

	hash := uint(0)
	for _, m := range matchers {
		hash ^= m.Hash()
	}
	hash ^= uint(len(matchers)) * arrayHashFactor
	return hash
}
