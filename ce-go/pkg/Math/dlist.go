package Container

type DList interface {

	// PUBLIC:

	Add(value interface{})
	ForEach(callback func(DList))
	Clear()
	Empty() bool
	First() DList
	Value() interface{}
	Remove()

	// PRIVATE:

	front() *dlist
}

type dlist struct {
	next, prev *dlist
	value      interface{}
}

func NewDList() DList {
	root := &dlist{
		next:  nil,
		prev:  nil,
		value: nil,
	}
	return root
}

func (l *dlist) front() *dlist {
	return l.next
}

func (l *dlist) First() DList {
	return l.next
}

func (l *dlist) Value() interface{} {
	return l.value
}

func (l *dlist) Add(value interface{}) {
	newEl := &dlist{
		next:  l.next,
		prev:  l,
		value: value,
	}
	if l.next != nil {
		l.next.prev = newEl
	}
	l.next = newEl
}

func (l *dlist) Empty() bool {
	return l.next == nil
}

func (l *dlist) Remove() {
	if l.next != nil {
		l.next.prev = l.prev
	}
	if l.prev != nil {
		l.prev.next = l.next
	}
}

func (l *dlist) ForEach(callback func(DList)) {
	for nextList := l.next; nextList != nil; nextList = nextList.front() {
		callback(nextList)
	}
}

func (l *dlist) Clear() {
	l.next = nil
}
