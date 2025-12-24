package main

import (
	"encoding/gob"
	"errors"
	"fmt"
	"io"
)

// Ошибки, которые могут возникнуть при работе с очередью
var (
	ErrQueueEmpty = errors.New("queue is empty")
)

// qNode представляет узел односвязного списка для очереди.
type qNode[T any] struct {
	data T
	next *qNode[T]
}

// Queue реализует структуру данных FIFO (First In, First Out).
// Zero-value для Queue не является валидным для использования, используйте NewQueue().
type Queue[T any] struct {
	frontNode *qNode[T] // Указатель на начало (отсюда забираем)
	backNode  *qNode[T] // Указатель на конец (сюда добавляем)
	size      int
}

// NewQueue создает новую пустую очередь.
func NewQueue[T any]() *Queue[T] {
	return &Queue[T]{
		frontNode: nil,
		backNode:  nil,
		size:      0,
	}
}

// Clone создает глубокую копию очереди.
// Аналог конструктора копирования в C++.
func (q *Queue[T]) Clone() *Queue[T] {
	newQ := NewQueue[T]()
	current := q.frontNode
	for current != nil {
		newQ.Enqueue(current.data)
		current = current.next
	}
	return newQ
}

// Enqueue добавляет элемент в конец очереди.
// Сложность: O(1).
func (q *Queue[T]) Enqueue(element T) {
	newNode := &qNode[T]{data: element, next: nil}
	if q.backNode == nil {
		q.frontNode = newNode
		q.backNode = newNode
	} else {
		q.backNode.next = newNode
		q.backNode = newNode
	}
	q.size++
}

// Dequeue удаляет элемент из начала очереди.
// Возвращает ошибку, если очередь пуста.
// Сложность: O(1).
func (q *Queue[T]) Dequeue() error {
	if q.frontNode == nil {
		return ErrQueueEmpty
	}

	// GC сам удалит старый узел, нам нужно только сдвинуть указатель
	q.frontNode = q.frontNode.next

	if q.frontNode == nil {
		q.backNode = nil
	}
	q.size--
	return nil
}

// Front возвращает первый элемент очереди.
// Сложность: O(1).
func (q *Queue[T]) Front() (T, error) {
	if q.frontNode == nil {
		var zero T
		return zero, ErrQueueEmpty
	}
	return q.frontNode.data, nil
}

// Back возвращает последний элемент очереди.
// Сложность: O(1).
func (q *Queue[T]) Back() (T, error) {
	if q.backNode == nil {
		var zero T
		return zero, ErrQueueEmpty
	}
	return q.backNode.data, nil
}

// Size возвращает текущий размер очереди.
func (q *Queue[T]) Size() int {
	return q.size
}

// IsEmpty проверяет, пуста ли очередь.
func (q *Queue[T]) IsEmpty() bool {
	return q.size == 0
}

// Clear полностью очищает очередь.
// В Go достаточно обнулить указатели, сборщик мусора очистит память узлов.
func (q *Queue[T]) Clear() {
	q.frontNode = nil
	q.backNode = nil
	q.size = 0
}

// Print выводит содержимое очереди в stdout.
func (q *Queue[T]) Print() {
	fmt.Print("Front -> [")
	current := q.frontNode
	for current != nil {
		fmt.Print(current.data)
		if current.next != nil {
			fmt.Print(", ")
		}
		current = current.next
	}
	fmt.Println("] <- Back")
}

// Serialize сохраняет состояние очереди в бинарном формате (gob).
// Аналог serialize/serializeBinary из C++.
func (q *Queue[T]) Serialize(w io.Writer) error {
	enc := gob.NewEncoder(w)
	
	// 1. Записываем размер
	if err := enc.Encode(q.size); err != nil {
		return err
	}

	// 2. Записываем элементы от головы к хвосту
	current := q.frontNode
	for current != nil {
		if err := enc.Encode(current.data); err != nil {
			return err
		}
		current = current.next
	}
	return nil
}

// Deserialize восстанавливает очередь из бинарного формата (gob).
func (q *Queue[T]) Deserialize(r io.Reader) error {
	dec := gob.NewDecoder(r)
	q.Clear()

	var newSize int
	if err := dec.Decode(&newSize); err != nil {
		return err
	}

	for i := 0; i < newSize; i++ {
		var val T
		if err := dec.Decode(&val); err != nil {
			return err
		}
		q.Enqueue(val)
	}
	return nil
}

// SerializeText сохраняет элементы в текстовом виде через пробел.
func (q *Queue[T]) SerializeText(w io.Writer) error {
	if _, err := fmt.Fprintln(w, q.size); err != nil {
		return err
	}
	
	current := q.frontNode
	for current != nil {
		if _, err := fmt.Fprint(w, current.data); err != nil {
			return err
		}
		if current.next != nil {
			if _, err := fmt.Fprint(w, " "); err != nil {
				return err
			}
		}
		current = current.next
	}
	if _, err := fmt.Fprintln(w); err != nil {
		return err
	}
	return nil
}

// DeserializeText считывает элементы из текстового потока.
func (q *Queue[T]) DeserializeText(r io.Reader) error {
	q.Clear()
	var newSize int
	if _, err := fmt.Fscan(r, &newSize); err != nil {
		return err
	}

	for i := 0; i < newSize; i++ {
		var val T
		if _, err := fmt.Fscan(r, &val); err != nil {
			return err
		}
		q.Enqueue(val)
	}
	return nil
}