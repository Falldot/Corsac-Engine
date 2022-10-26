package ce2d_math

type DoublyList struct {
	next, prev *DoublyList
	Value      interface{}
}

func CreateDoublyList(value ...interface{}) *DoublyList {
	root := &DoublyList{
		next:  nil,
		prev:  nil,
		Value: value[0],
	}

	for i := 1; i < len(value); i++ {
		root.Add(value[i])
	}

	return root
}

func (l *DoublyList) Next() *DoublyList {
	return l.next
}

func (l *DoublyList) Add(value interface{}) *DoublyList {
	newEl := &DoublyList{
		next:  l.next,
		prev:  l,
		Value: value,
	}
	if l.next != nil {
		l.next.prev = newEl
	}
	l.next = newEl
	return l.next
}

func (l *DoublyList) Remove() {
	if l.next != nil {
		l.next.prev = l.prev
	}
	if l.prev != nil {
		l.prev.next = l.next
	}
}

func (l *DoublyList) ForEach(callback func(*DoublyList)) {
	for nextList := l; nextList != nil; nextList = nextList.Next() {
		callback(nextList)
	}
}

func (l *DoublyList) Clear() *DoublyList {
	l.next = nil
	return l.next
}
