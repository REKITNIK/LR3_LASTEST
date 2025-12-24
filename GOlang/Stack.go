package main

import (
	"encoding/gob"
	"errors"
	"fmt"
	"io"
)

// Ошибки, которые могут возникнуть при работе со стеком
var (
	ErrStackEmpty = errors.New("stack is empty")
)

// sNode представляет узел односвязного списка для стека.
type sNode[T any] struct {
	data T
	next *sNode[T]
}

// Stack реализует структуру данных LIFO (Last In, First Out).
type Stack[T any] struct {
	topNode *sNode[T] // Указатель на вершину
	size    int
}

// NewStack создает новый пустой стек.
func NewStack[T any]() *Stack[T] {
	return &Stack[T]{
		topNode: nil,
		size:    0,
	}
}

// Clone создает глубокую копию стека.
// Внимание: чтобы сохранить порядок, мы собираем элементы в слайс,
// а затем пушим их в обратном порядке (от дна к вершине).
func (s *Stack[T]) Clone() *Stack[T] {
	newStack := NewStack[T]()
	if s.topNode == nil {
		return newStack
	}

	// Собираем элементы: Top -> Bottom
	tempSlice := make([]T, 0, s.size)
	current := s.topNode
	for current != nil {
		tempSlice = append(tempSlice, current.data)
		current = current.next
	}

	// Вставляем элементы: Bottom -> Top, чтобы восстановить структуру
	for i := len(tempSlice) - 1; i >= 0; i-- {
		newStack.Push(tempSlice[i])
	}

	return newStack
}

// Push помещает элемент на вершину стека.
// Сложность: O(1).
func (s *Stack[T]) Push(element T) {
	newNode := &sNode[T]{data: element, next: s.topNode}
	s.topNode = newNode
	s.size++
}

// Pop удаляет элемент с вершины стека.
// Сложность: O(1).
func (s *Stack[T]) Pop() error {
	if s.topNode == nil {
		return ErrStackEmpty
	}
	s.topNode = s.topNode.next
	s.size--
	return nil
}

// Top возвращает элемент на вершине стека.
// Сложность: O(1).
func (s *Stack[T]) Top() (T, error) {
	if s.topNode == nil {
		var zero T
		return zero, ErrStackEmpty
	}
	return s.topNode.data, nil
}

// Size возвращает текущий размер стека.
func (s *Stack[T]) Size() int {
	return s.size
}

// IsEmpty проверяет, пуст ли стек.
func (s *Stack[T]) IsEmpty() bool {
	return s.size == 0
}

// Clear полностью очищает стек.
func (s *Stack[T]) Clear() {
	s.topNode = nil
	s.size = 0
}

// Print выводит содержимое стека.
func (s *Stack[T]) Print() {
	fmt.Print("Top -> [")
	current := s.topNode
	for current != nil {
		fmt.Print(current.data)
		if current.next != nil {
			fmt.Print(", ")
		}
		current = current.next
	}
	fmt.Println("] <- Bottom")
}

// Serialize сохраняет стек.
// Важно: как и в C++, мы сохраняем от Дна к Вершине (Bottom -> Top),
// чтобы при десериализации (Push) порядок восстановился корректно.
func (s *Stack[T]) Serialize(w io.Writer) error {
	enc := gob.NewEncoder(w)

	if err := enc.Encode(s.size); err != nil {
		return err
	}

	// Собираем элементы в слайс (Top -> Bottom)
	tempSlice := make([]T, 0, s.size)
	current := s.topNode
	for current != nil {
		tempSlice = append(tempSlice, current.data)
		current = current.next
	}

	// Пишем в обратном порядке (Bottom -> Top)
	for i := len(tempSlice) - 1; i >= 0; i-- {
		if err := enc.Encode(tempSlice[i]); err != nil {
			return err
		}
	}
	return nil
}

// Deserialize восстанавливает стек.
func (s *Stack[T]) Deserialize(r io.Reader) error {
	dec := gob.NewDecoder(r)
	s.Clear()

	var newSize int
	if err := dec.Decode(&newSize); err != nil {
		return err
	}

	for i := 0; i < newSize; i++ {
		var val T
		if err := dec.Decode(&val); err != nil {
			return err
		}
		s.Push(val)
	}
	return nil
}

// SerializeText текстовая сериализация (Bottom -> Top).
func (s *Stack[T]) SerializeText(w io.Writer) error {
	if _, err := fmt.Fprintln(w, s.size); err != nil {
		return err
	}

	tempSlice := make([]T, 0, s.size)
	current := s.topNode
	for current != nil {
		tempSlice = append(tempSlice, current.data)
		current = current.next
	}

	for i := len(tempSlice) - 1; i >= 0; i-- {
		if _, err := fmt.Fprint(w, tempSlice[i]); err != nil {
			return err
		}
		if i > 0 { // Пробел между элементами, но не после последнего
			if _, err := fmt.Fprint(w, " "); err != nil {
				return err
			}
		}
	}
	if _, err := fmt.Fprintln(w); err != nil {
		return err
	}
	return nil
}

// DeserializeText текстовая десериализация.
func (s *Stack[T]) DeserializeText(r io.Reader) error {
	s.Clear()
	var newSize int
	if _, err := fmt.Fscan(r, &newSize); err != nil {
		return err
	}

	for i := 0; i < newSize; i++ {
		var val T
		if _, err := fmt.Fscan(r, &val); err != nil {
			return err
		}
		s.Push(val)
	}
	return nil
}