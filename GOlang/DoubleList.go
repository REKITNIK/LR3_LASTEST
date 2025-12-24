package main

import (
	"encoding/gob"
	"fmt"
	"io"
)

// Ошибки ErrEmptyList и ErrIndexOutOfRange берутся из forward_list.go,
// так как они в одном пакете 'structures'.

type doubleNode[T any] struct {
	Data T
	Next *doubleNode[T]
	Prev *doubleNode[T]
}

// DoubleList реализует двусвязный список.
type DoubleList[T comparable] struct {
	head *doubleNode[T]
	tail *doubleNode[T]
	size int
}

// NewDoubleList создает новый пустой двусвязный список.
func NewDoubleList[T comparable]() *DoubleList[T] {
	return &DoubleList[T]{}
}

// Clone создает глубокую копию списка.
func (l *DoubleList[T]) Clone() *DoubleList[T] {
	newList := NewDoubleList[T]()
	current := l.head
	for current != nil {
		newList.PushBack(current.Data)
		current = current.Next
	}
	return newList
}

// PushFront добавляет элемент в начало списка.
// Сложность: O(1).
func (l *DoubleList[T]) PushFront(element T) {
	newNode := &doubleNode[T]{Data: element}
	if l.head == nil {
		l.head = newNode
		l.tail = newNode
	} else {
		newNode.Next = l.head
		l.head.Prev = newNode
		l.head = newNode
	}
	l.size++
}

// PushBack добавляет элемент в конец списка.
// Сложность: O(1).
func (l *DoubleList[T]) PushBack(element T) {
	newNode := &doubleNode[T]{Data: element}
	if l.tail == nil {
		l.head = newNode
		l.tail = newNode
	} else {
		l.tail.Next = newNode
		newNode.Prev = l.tail
		l.tail = newNode
	}
	l.size++
}

// Insert вставляет элемент по индексу.
// Оптимизация: перебор с начала или с конца.
func (l *DoubleList[T]) Insert(index int, element T) error {
	if index > l.size || index < 0 {
		return ErrIndexOutOfRange
	}
	if index == 0 {
		l.PushFront(element)
		return nil
	}
	if index == l.size {
		l.PushBack(element)
		return nil
	}

	newNode := &doubleNode[T]{Data: element}
	var current *doubleNode[T]

	if index <= l.size/2 {
		current = l.head
		for i := 0; i < index; i++ {
			current = current.Next
		}
	} else {
		current = l.tail
		// Смещение от конца: size-1 - index шагов назад
		for i := 0; i < l.size-1-index; i++ {
			current = current.Prev
		}
	}

	newNode.Next = current
	newNode.Prev = current.Prev
	current.Prev.Next = newNode
	current.Prev = newNode
	l.size++
	return nil
}

// PopFront удаляет первый элемент.
func (l *DoubleList[T]) PopFront() error {
	if l.head == nil {
		return ErrEmptyList
	}
	if l.head == l.tail {
		l.head = nil
		l.tail = nil
	} else {
		l.head = l.head.Next
		l.head.Prev = nil
	}
	l.size--
	return nil
}

// PopBack удаляет последний элемент.
func (l *DoubleList[T]) PopBack() error {
	if l.tail == nil {
		return ErrEmptyList
	}
	if l.head == l.tail {
		l.head = nil
		l.tail = nil
	} else {
		l.tail = l.tail.Prev
		l.tail.Next = nil
	}
	l.size--
	return nil
}

// Remove удаляет элемент по индексу.
func (l *DoubleList[T]) Remove(index int) error {
	if index >= l.size || index < 0 {
		return ErrIndexOutOfRange
	}
	if index == 0 {
		return l.PopFront()
	}
	if index == l.size-1 {
		return l.PopBack()
	}

	var current *doubleNode[T]
	if index <= l.size/2 {
		current = l.head
		for i := 0; i < index; i++ {
			current = current.Next
		}
	} else {
		current = l.tail
		for i := 0; i < l.size-1-index; i++ {
			current = current.Prev
		}
	}

	current.Prev.Next = current.Next
	current.Next.Prev = current.Prev
	l.size--
	return nil
}

// RemoveValue удаляет все вхождения значения.
func (l *DoubleList[T]) RemoveValue(value T) {
	current := l.head
	for current != nil {
		if current.Data == value {
			toDelete := current
			current = current.Next 

			if toDelete == l.head && toDelete == l.tail {
				l.head = nil
				l.tail = nil
			} else if toDelete == l.head {
				l.head = l.head.Next
				l.head.Prev = nil
			} else if toDelete == l.tail {
				l.tail = l.tail.Prev
				l.tail.Next = nil
			} else {
				toDelete.Prev.Next = toDelete.Next
				toDelete.Next.Prev = toDelete.Prev
			}
			l.size--
		} else {
			current = current.Next
		}
	}
}

// Get возвращает указатель на элемент по индексу.
func (l *DoubleList[T]) Get(index int) (*T, error) {
	if index >= l.size || index < 0 {
		return nil, ErrIndexOutOfRange
	}
	var current *doubleNode[T]
	if index <= l.size/2 {
		current = l.head
		for i := 0; i < index; i++ {
			current = current.Next
		}
	} else {
		current = l.tail
		for i := 0; i < l.size-1-index; i++ {
			current = current.Prev
		}
	}
	return &current.Data, nil
}

// Front возвращает указатель на первый элемент.
func (l *DoubleList[T]) Front() (*T, error) {
	if l.head == nil {
		return nil, ErrEmptyList
	}
	return &l.head.Data, nil
}

// Back возвращает указатель на последний элемент.
func (l *DoubleList[T]) Back() (*T, error) {
	if l.tail == nil {
		return nil, ErrEmptyList
	}
	return &l.tail.Data, nil
}

func (l *DoubleList[T]) GetSize() int {
	return l.size
}

func (l *DoubleList[T]) IsEmpty() bool {
	return l.size == 0
}

func (l *DoubleList[T]) Clear() {
	l.head = nil
	l.tail = nil
	l.size = 0
}

func (l *DoubleList[T]) Find(value T) bool {
	current := l.head
	for current != nil {
		if current.Data == value {
			return true
		}
		current = current.Next
	}
	return false
}

func (l *DoubleList[T]) Print() {
	fmt.Print("[")
	current := l.head
	for current != nil {
		fmt.Print(current.Data)
		if current.Next != nil {
			fmt.Print(" <-> ")
		}
		current = current.Next
	}
	fmt.Println("]")
}

func (l *DoubleList[T]) PrintReverse() {
	fmt.Print("[")
	current := l.tail
	for current != nil {
		fmt.Print(current.Data)
		if current.Prev != nil {
			fmt.Print(" <-> ")
		}
		current = current.Prev
	}
	fmt.Println("]")
}

func (l *DoubleList[T]) SerializeBinary(out io.Writer) error {
	enc := gob.NewEncoder(out)
	if err := enc.Encode(l.size); err != nil {
		return err
	}
	current := l.head
	for current != nil {
		if err := enc.Encode(current.Data); err != nil {
			return err
		}
		current = current.Next
	}
	return nil
}

func (l *DoubleList[T]) DeserializeBinary(in io.Reader) error {
	l.Clear()
	dec := gob.NewDecoder(in)
	var newSize int
	if err := dec.Decode(&newSize); err != nil {
		return err
	}
	
	for i := 0; i < newSize; i++ {
		var val T
		if err := dec.Decode(&val); err != nil {
			return err
		}
		l.PushBack(val)
	}
	return nil
}

func (l *DoubleList[T]) SerializeText(out io.Writer) {
	fmt.Fprintln(out, l.size)
	current := l.head
	for current != nil {
		fmt.Fprint(out, current.Data)
		if current.Next != nil {
			fmt.Fprint(out, " ")
		}
		current = current.Next
	}
	fmt.Fprintln(out)
}

func (l *DoubleList[T]) DeserializeText(in io.Reader) error {
	l.Clear()
	var newSize int
	_, err := fmt.Fscan(in, &newSize)
	if err != nil {
		return err
	}
	
	for i := 0; i < newSize; i++ {
		var val T
		_, err := fmt.Fscan(in, &val)
		if err != nil {
			return err
		}
		l.PushBack(val)
	}
	return nil
}