package main

import (
	"encoding/gob"
	"errors"
	"fmt"
	"io"
)

// Общие ошибки пакета (доступны и в double_list.go)
var (
	ErrEmptyList       = errors.New("list is empty")
	ErrIndexOutOfRange = errors.New("index out of range")
)

type forwardNode[T any] struct {
	Data T
	Next *forwardNode[T]
}

// ForwardList реализует односвязный список.
// T должен быть comparable для поддержки методов Find и RemoveValue.
type ForwardList[T comparable] struct {
	head *forwardNode[T]
	size int
}

// NewForwardList создает новый пустой список.
func NewForwardList[T comparable]() *ForwardList[T] {
	return &ForwardList[T]{}
}

// Clone создает глубокую копию списка.
// Сложность: O(N).
func (l *ForwardList[T]) Clone() *ForwardList[T] {
	newList := NewForwardList[T]()
	if l.head == nil {
		return newList
	}

	// Копируем голову
	newList.head = &forwardNode[T]{Data: l.head.Data}
	newList.size = l.size

	// Копируем остальные элементы
	current := newList.head
	source := l.head.Next

	for source != nil {
		current.Next = &forwardNode[T]{Data: source.Data}
		current = current.Next
		source = source.Next
	}

	return newList
}

// PushFront добавляет элемент в начало списка.
// Сложность: O(1).
func (l *ForwardList[T]) PushFront(element T) {
	newNode := &forwardNode[T]{Data: element, Next: l.head}
	l.head = newNode
	l.size++
}

// PushBack добавляет элемент в конец списка.
// Сложность: O(N), так как в оригинале нет указателя на хвост.
func (l *ForwardList[T]) PushBack(element T) {
	newNode := &forwardNode[T]{Data: element}
	if l.head == nil {
		l.head = newNode
	} else {
		current := l.head
		for current.Next != nil {
			current = current.Next
		}
		current.Next = newNode
	}
	l.size++
}

// Insert вставляет элемент по указанному индексу.
// Сложность: O(N).
func (l *ForwardList[T]) Insert(index int, element T) error {
	if index > l.size || index < 0 {
		return ErrIndexOutOfRange
	}
	if index == 0 {
		l.PushFront(element)
		return nil
	}

	newNode := &forwardNode[T]{Data: element}
	current := l.head
	// Идем до элемента index-1
	for i := 0; i < index-1; i++ {
		current = current.Next
	}
	newNode.Next = current.Next
	current.Next = newNode
	l.size++
	return nil
}

// PopFront удаляет первый элемент списка.
// Сложность: O(1).
func (l *ForwardList[T]) PopFront() error {
	if l.head == nil {
		return ErrEmptyList
	}
	l.head = l.head.Next
	l.size--
	return nil
}

// Remove удаляет элемент по индексу.
// Сложность: O(N).
func (l *ForwardList[T]) Remove(index int) error {
	if index >= l.size || index < 0 {
		return ErrIndexOutOfRange
	}
	if index == 0 {
		return l.PopFront()
	}

	current := l.head
	// Идем до элемента index-1
	for i := 0; i < index-1; i++ {
		current = current.Next
	}
	// current.Next - удаляемый элемент
	current.Next = current.Next.Next
	l.size--
	return nil
}

// RemoveValue удаляет все вхождения заданного значения.
// Сложность: O(N).
func (l *ForwardList[T]) RemoveValue(value T) {
	// Удаляем из головы
	for l.head != nil && l.head.Data == value {
		l.head = l.head.Next
		l.size--
	}

	if l.head == nil {
		return
	}

	current := l.head
	for current.Next != nil {
		if current.Next.Data == value {
			current.Next = current.Next.Next
			l.size--
		} else {
			current = current.Next
		}
	}
}

// Get возвращает указатель на элемент по индексу.
// Сложность: O(N).
func (l *ForwardList[T]) Get(index int) (*T, error) {
	if index >= l.size || index < 0 {
		return nil, ErrIndexOutOfRange
	}
	current := l.head
	for i := 0; i < index; i++ {
		current = current.Next
	}
	return &current.Data, nil
}

// Front возвращает указатель на первый элемент.
func (l *ForwardList[T]) Front() (*T, error) {
	if l.head == nil {
		return nil, ErrEmptyList
	}
	return &l.head.Data, nil
}

func (l *ForwardList[T]) GetSize() int {
	return l.size
}

func (l *ForwardList[T]) IsEmpty() bool {
	return l.size == 0
}

func (l *ForwardList[T]) Clear() {
	l.head = nil
	l.size = 0
}

func (l *ForwardList[T]) Find(value T) bool {
	current := l.head
	for current != nil {
		if current.Data == value {
			return true
		}
		current = current.Next
	}
	return false
}

func (l *ForwardList[T]) Print() {
	fmt.Print("[")
	current := l.head
	for current != nil {
		fmt.Print(current.Data)
		if current.Next != nil {
			fmt.Print(" -> ")
		}
		current = current.Next
	}
	fmt.Println("]")
}

func (l *ForwardList[T]) SerializeBinary(out io.Writer) error {
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

func (l *ForwardList[T]) DeserializeBinary(in io.Reader) error {
	l.Clear()
	dec := gob.NewDecoder(in)
	
	var newSize int
	if err := dec.Decode(&newSize); err != nil {
		return err
	}
	if newSize == 0 {
		return nil
	}

	var val T
	if err := dec.Decode(&val); err != nil {
		return err
	}
	l.head = &forwardNode[T]{Data: val}
	l.size = 1

	current := l.head
	for i := 1; i < newSize; i++ {
		if err := dec.Decode(&val); err != nil {
			return err
		}
		current.Next = &forwardNode[T]{Data: val}
		current = current.Next
		l.size++
	}
	return nil
}

func (l *ForwardList[T]) SerializeText(out io.Writer) {
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

func (l *ForwardList[T]) DeserializeText(in io.Reader) error {
	l.Clear()
	var newSize int
	_, err := fmt.Fscan(in, &newSize)
	if err != nil {
		return err
	}
	if newSize == 0 {
		return nil
	}

	var val T
	_, err = fmt.Fscan(in, &val)
	if err != nil {
		return err
	}
	l.head = &forwardNode[T]{Data: val}
	l.size = 1

	current := l.head
	for i := 1; i < newSize; i++ {
		_, err = fmt.Fscan(in, &val)
		if err != nil {
			return err
		}
		current.Next = &forwardNode[T]{Data: val}
		current = current.Next
		l.size++
	}
	return nil
}