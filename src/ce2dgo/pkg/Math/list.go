package Container

type List interface {

	// PUBLIC:

	Add(value interface{})
	ForEach(callback func(List))
	Clear()
	Empty() bool
	Value() interface{}

	// PRIVATE:

	front() List
}

type list struct {
	next  List
	value interface{}
}

func NewList() List {
	root := &list{
		next:  nil,
		value: nil,
	}
	return root
}

func (l *list) front() List {
	return l.next
}

func (l *list) Value() interface{} {
	return l.value
}

func (l *list) Add(value interface{}) {
	l.next = &list{
		next:  l.next,
		value: value,
	}
}

func (l *list) Empty() bool {
	return l.next == nil
}

func (l *list) ForEach(callback func(List)) {
	for nextList := l.next; nextList != nil; nextList = nextList.front() {
		callback(nextList)
	}
}

func (l *list) Clear() {
	l.next = nil
}
