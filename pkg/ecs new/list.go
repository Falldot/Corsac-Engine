package ecs

type Element struct {
	next, prev *Element
	list       *List
	Value      interface{}
}

func (e *Element) Next() *Element {
	if p := e.next; e.list != nil && p != &e.list.root {
		return p
	}
	return nil
}

func (e *Element) Prev() *Element {
	if p := e.prev; e.list != nil && p != &e.list.root {
		return p
	}
	return nil
}

type List struct {
	root Element
	len  uint
}

func New() *List {
	l := new(List)
	l.root.next = &l.root
	l.root.prev = &l.root
	l.len = 0
	return l
}

func (l *List) Map(do func(e *Element)) {
	for e := l.Front(); e != nil; e = e.Next() {
		do(e)
	}
}

func (l *List) Len() uint {
	return l.len
}

func (l *List) Push(v interface{}) {
	e := &Element{Value: v}
	e.prev = l.root.prev
	e.next = l.root.prev.next
	e.prev.next = e
	e.next.prev = e
	e.list = l
	l.len++
}

func (l *List) Front() *Element {
	return l.root.next
}

func (l *List) Remove(e *Element) {
	e.prev.next = e.next
	e.next.prev = e.prev
	e.next = nil
	e.prev = nil
	e.list = nil
	l.len--
}
