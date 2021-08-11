package ce2d_math

type List struct {
	next  *List
	Value interface{}
}

func CreateList(value ...interface{}) *List {
	root := &List{
		next:  nil,
		Value: value[0],
	}

	for i := 1; i < len(value); i++ {
		root.Add(value[i])
	}

	return root
}

func (l *List) Next() *List {
	return l.next
}

func (l *List) Add(value interface{}) *List {
	l.next = &List{
		next:  l.next,
		Value: value,
	}
	return l.next
}

func (l *List) ForEach(callback func(*List)) {
	for nextList := l; nextList != nil; nextList = nextList.Next() {
		callback(nextList)
	}
}

func (l *List) Clear() *List {
	l.next = nil
	return l.next
}
