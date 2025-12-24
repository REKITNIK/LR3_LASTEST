package main

import (
	"encoding/gob"
	"errors"
	"fmt"
	"io"
)

// Array реализует динамический массив с автоматическим изменением ёмкости.
// Порт C++ класса Array<T>.
type Array[T any] struct {
	// data представляет собой буфер. len(data) соответствует capacity в C++.
	// Мы не используем append, а управляем размером вручную, чтобы сохранить логику оригинала.
	data []T
	// size текущее количество реально используемых элементов.
	size int
}

// NewArray создает новый массив с указанной начальной ёмкостью.
// Соответствует конструктору Array(size_t initial_capacity).
func NewArray[T any](capacity int) *Array[T] {
	if capacity < 0 {
		capacity = 0
	}
	return &Array[T]{
		data: make([]T, capacity),
		size: 0,
	}
}

// resize изменяет ёмкость массива.
// Выделяет новый буфер, копирует существующие элементы и заменяет старый буфер.
// Соответствует приватному методу void resize(size_t new_capacity).
func (a *Array[T]) resize(newCapacity int) {
	if newCapacity < 0 {
		newCapacity = 0
	}

	// Выделяем новую память (аналог new T[new_capacity])
	newData := make([]T, newCapacity)

	// Вычисляем размер копирования (min(size, newCapacity))
	copySize := a.size
	if newCapacity < copySize {
		copySize = newCapacity
	}

	// Копируем данные
	// В Go copy заменяет цикл for, но алгоритмически это то же самое O(N)
	copy(newData, a.data[:copySize])

	// Заменяем старый буфер (старый будет собран GC, аналог delete[])
	a.data = newData
	
	// Если массив усекли
	if a.size > newCapacity {
		a.size = newCapacity
	}
}

// Add добавляет элемент в конец массива.
// Если ёмкости недостаточно, происходит перевыделение памяти (x2).
func (a *Array[T]) Add(element T) {
	capacity := len(a.data)
	if a.size >= capacity {
		newCap := 1
		if capacity > 0 {
			newCap = capacity * 2
		}
		a.resize(newCap)
	}
	a.data[a.size] = element
	a.size++
}

// Insert вставляет элемент по указанному индексу.
// Элементы сдвигаются вправо.
// Возвращает error вместо throw std::out_of_range.
func (a *Array[T]) Insert(index int, element T) error {
	if index < 0 || index > a.size {
		return errors.New("index out of range")
	}

	capacity := len(a.data)
	if a.size >= capacity {
		newCap := 1
		if capacity > 0 {
			newCap = capacity * 2
		}
		a.resize(newCap)
	}

	// Сдвиг элементов вправо, начиная с конца
	// Симулируем цикл: for (size_t i = size; i > index; --i)
	for i := a.size; i > index; i-- {
		a.data[i] = a.data[i-1]
	}

	a.data[index] = element
	a.size++
	return nil
}

// Remove удаляет элемент по индексу.
// Элементы сдвигаются влево.
func (a *Array[T]) Remove(index int) error {
	if index < 0 || index >= a.size {
		return errors.New("index out of range")
	}

	// Сдвиг элементов влево
	// Симулируем цикл: for (size_t i = index; i < size - 1; ++i)
	for i := index; i < a.size-1; i++ {
		a.data[i] = a.data[i+1]
	}

	// Важно для GC в Go: обнуляем последний элемент, который теперь "удален",
	// чтобы избежать утечки памяти, если T содержит указатели.
	// В C++ это не требовалось из-за отсутствия GC, но в Go это best practice.
	var zero T
	a.data[a.size-1] = zero

	a.size--
	return nil
}

// Get возвращает элемент по индексу.
func (a *Array[T]) Get(index int) (T, error) {
	if index < 0 || index >= a.size {
		var zero T
		return zero, errors.New("index out of range")
	}
	return a.data[index], nil
}

// Set перезаписывает элемент по индексу.
func (a *Array[T]) Set(index int, element T) error {
	if index < 0 || index >= a.size {
		return errors.New("index out of range")
	}
	a.data[index] = element
	return nil
}

// GetSize возвращает текущее количество элементов.
func (a *Array[T]) Size() int {
	return a.size
}

// GetCapacity возвращает текущую ёмкость буфера.
func (a *Array[T]) Capacity() int {
	return len(a.data)
}

// IsEmpty проверяет, пуст ли массив.
func (a *Array[T]) IsEmpty() bool {
	return a.size == 0
}

// Clear полностью очищает массив и освобождает память.
func (a *Array[T]) Clear() {
	// В Go make([]T, 0) создает новый пустой слайс.
	// Старый массив будет удален GC.
	a.data = make([]T, 0)
	a.size = 0
}

// Print выводит содержимое массива в стандартный поток вывода.
func (a *Array[T]) Print() {
	fmt.Print("[")
	for i := 0; i < a.size; i++ {
		fmt.Print(a.data[i])
		if i < a.size-1 {
			fmt.Print(", ")
		}
	}
	fmt.Println("]")
}

// Serialize выполняет сериализацию данных в поток writer.
// Использует encoding/gob вместо raw bytes для безопасности типов.
func (a *Array[T]) Serialize(w io.Writer) error {
	encoder := gob.NewEncoder(w)
	
	// Сохраняем только активную часть данных
	activeData := a.data[:a.size]
	
	if err := encoder.Encode(activeData); err != nil {
		return fmt.Errorf("failed to encode data: %w", err)
	}
	return nil
}

// Deserialize восстанавливает массив из потока reader.
func (a *Array[T]) Deserialize(r io.Reader) error {
	decoder := gob.NewDecoder(r)
	
	var loadedData []T
	if err := decoder.Decode(&loadedData); err != nil {
		return fmt.Errorf("failed to decode data: %w", err)
	}

	a.Clear()
	
	// Если загруженные данные больше текущей (нулевой) ёмкости, ресайзим
	neededCap := len(loadedData)
	if neededCap > 0 {
		a.resize(neededCap)
	}
	
	// Копируем данные
	copy(a.data, loadedData)
	a.size = neededCap
	
	return nil
}