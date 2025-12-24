package main

import (
	"encoding/gob"
	"fmt"
	"hash/fnv"
	"io"
)

// entry представляет узел в цепочке (связном списке) бакета.
type entry[K comparable, V any] struct {
	Key   K
	Value V
	Next  *entry[K, V]
}

// HashTable реализует словарь на основе метода цепочек.
type HashTable[K comparable, V any] struct {
	buckets     []*entry[K, V]
	size        int
	bucketCount int
}

// NewHashTable создает новую хеш-таблицу.
// initialBucketCount опционален (по умолчанию 16).
func NewHashTable[K comparable, V any](initialBucketCount ...int) *HashTable[K, V] {
	bc := 16
	if len(initialBucketCount) > 0 && initialBucketCount[0] > 0 {
		bc = initialBucketCount[0]
	}
	return &HashTable[K, V]{
		buckets:     make([]*entry[K, V], bc),
		size:        0,
		bucketCount: bc,
	}
}

// hash вычисляет индекс бакета для ключа.
// Использует FNV-1a хеширование строкового представления ключа.
func (ht *HashTable[K, V]) hash(key K) int {
	h := fnv.New64a()
	// fmt.Sprintf("%v") - универсальный способ получить байты для comparable типов
	h.Write([]byte(fmt.Sprintf("%v", key)))
	return int(h.Sum64() % uint64(ht.bucketCount))
}

// rehash увеличивает размер таблицы в 2 раза и перераспределяет элементы.
func (ht *HashTable[K, V]) rehash() {
	oldBuckets := ht.buckets
	ht.bucketCount *= 2
	ht.buckets = make([]*entry[K, V], ht.bucketCount)
	
	// Сбрасываем size, так как будем пересчитывать его (или просто переносить узлы)
	// В C++ оригинале size обнуляется и инкрементируется при переносе.
	ht.size = 0

	for _, head := range oldBuckets {
		current := head
		for current != nil {
			next := current.Next // Сохраняем ссылку на следующий
			
			// Пересчитываем индекс для новой размерности
			newIndex := ht.hash(current.Key)
			
			// Вставляем в начало списка нового бакета (re-linking)
			current.Next = ht.buckets[newIndex]
			ht.buckets[newIndex] = current
			
			ht.size++
			current = next
		}
	}
}

// Insert вставляет или обновляет пару ключ-значение.
func (ht *HashTable[K, V]) Insert(key K, value V) {
	if ht.LoadFactor() > 0.75 {
		ht.rehash()
	}

	index := ht.hash(key)
	current := ht.buckets[index]

	// Проверка на обновление существующего ключа
	for current != nil {
		if current.Key == key {
			current.Value = value
			return
		}
		current = current.Next
	}

	// Вставка нового узла в начало списка (Head insert)
	newEntry := &entry[K, V]{
		Key:   key,
		Value: value,
		Next:  ht.buckets[index],
	}
	ht.buckets[index] = newEntry
	ht.size++
}

// Remove удаляет элемент по ключу.
func (ht *HashTable[K, V]) Remove(key K) error {
	index := ht.hash(key)
	current := ht.buckets[index]
	var prev *entry[K, V] = nil

	for current != nil {
		if current.Key == key {
			if prev != nil {
				prev.Next = current.Next
			} else {
				ht.buckets[index] = current.Next
			}
			// GC сам очистит current, так как ссылок на него больше нет
			ht.size--
			return nil
		}
		prev = current
		current = current.Next
	}

	return fmt.Errorf("key not found")
}

// Get получает значение по ключу.
func (ht *HashTable[K, V]) Get(key K) (V, error) {
	index := ht.hash(key)
	current := ht.buckets[index]

	for current != nil {
		if current.Key == key {
			return current.Value, nil
		}
		current = current.Next
	}

	var zero V
	return zero, fmt.Errorf("key not found")
}

// Find проверяет наличие ключа в таблице.
func (ht *HashTable[K, V]) Find(key K) bool {
	index := ht.hash(key)
	current := ht.buckets[index]

	for current != nil {
		if current.Key == key {
			return true
		}
		current = current.Next
	}
	return false
}

// GetSize возвращает текущее количество элементов.
func (ht *HashTable[K, V]) GetSize() int {
	return ht.size
}

// GetBucketCount возвращает текущее количество бакетов.
func (ht *HashTable[K, V]) GetBucketCount() int {
	return ht.bucketCount
}

// IsEmpty проверяет, пуста ли таблица.
func (ht *HashTable[K, V]) IsEmpty() bool {
	return ht.size == 0
}

// Clear полностью очищает таблицу.
func (ht *HashTable[K, V]) Clear() {
	// В Go достаточно занулить ссылки в слайсе, GC удалит цепочки.
	for i := 0; i < ht.bucketCount; i++ {
		ht.buckets[i] = nil
	}
	ht.size = 0
}

// LoadFactor вычисляет текущий коэффициент заполнения.
func (ht *HashTable[K, V]) LoadFactor() float64 {
	return float64(ht.size) / float64(ht.bucketCount)
}

// Print выводит содержимое непустых бакетов в консоль.
func (ht *HashTable[K, V]) Print() {
	fmt.Printf("HashTable (size: %d, buckets: %d):\n", ht.size, ht.bucketCount)
	for i, head := range ht.buckets {
		if head != nil {
			fmt.Printf("  Bucket %d: ", i)
			current := head
			for current != nil {
				fmt.Printf("[%v:%v]", current.Key, current.Value)
				if current.Next != nil {
					fmt.Print(" -> ")
				}
				current = current.Next
			}
			fmt.Println()
		}
	}
}

// --- Сериализация ---

// entryData используется для gob сериализации плоского списка данных
type entryData[K comparable, V any] struct {
	Key   K
	Value V
}

// Serialize сохраняет таблицу в бинарном формате (используя gob).
func (ht *HashTable[K, V]) Serialize(out io.Writer) error {
	// Gob требует регистрации интерфейсов, но для конкретных типов работает сразу.
	// Мы сохраняем метаданные и список всех пар.
	enc := gob.NewEncoder(out)

	// 1. Сохраняем bucketCount (чтобы восстановить структуру хеширования)
	if err := enc.Encode(ht.bucketCount); err != nil {
		return err
	}
	
	// 2. Сохраняем size
	if err := enc.Encode(ht.size); err != nil {
		return err
	}

	// 3. Сохраняем все элементы подряд
	for _, head := range ht.buckets {
		current := head
		for current != nil {
			data := entryData[K, V]{Key: current.Key, Value: current.Value}
			if err := enc.Encode(data); err != nil {
				return err
			}
			current = current.Next
		}
	}
	return nil
}

// Deserialize восстанавливает таблицу из бинарного формата.
func (ht *HashTable[K, V]) Deserialize(in io.Reader) error {
	ht.Clear()
	
	dec := gob.NewDecoder(in)
	
	var newBucketCount int
	if err := dec.Decode(&newBucketCount); err != nil {
		return err
	}
	
	var newSize int
	if err := dec.Decode(&newSize); err != nil {
		return err
	}

	// Пересоздаем структуру
	ht.bucketCount = newBucketCount
	ht.buckets = make([]*entry[K, V], ht.bucketCount)
	// size будет увеличиваться внутри Insert, поэтому пока оставим 0
	// (или можно использовать внутренний метод без проверки LoadFactor, но Insert безопаснее)

	for i := 0; i < newSize; i++ {
		var data entryData[K, V]
		if err := dec.Decode(&data); err != nil {
			// Если EOF случился раньше ожидаемого размера — это ошибка данных
			return err
		}
		// Используем Insert для корректного распределения по бакетам
		ht.Insert(data.Key, data.Value)
	}

	return nil
}

// SerializeText сохраняет таблицу в текстовом виде.
// Формат: <size> <bucket_count>\n<key> <value>\n...
func (ht *HashTable[K, V]) SerializeText(out io.Writer) error {
	if _, err := fmt.Fprintf(out, "%d %d\n", ht.size, ht.bucketCount); err != nil {
		return err
	}

	for _, head := range ht.buckets {
		current := head
		for current != nil {
			if _, err := fmt.Fprintf(out, "%v %v\n", current.Key, current.Value); err != nil {
				return err
			}
			current = current.Next
		}
	}
	return nil
}

// DeserializeText восстанавливает таблицу из текстового формата.
func (ht *HashTable[K, V]) DeserializeText(in io.Reader) error {
	ht.Clear()

	var newSize, newBucketCount int
	if _, err := fmt.Fscan(in, &newSize, &newBucketCount); err != nil {
		return err
	}

	ht.bucketCount = newBucketCount
	ht.buckets = make([]*entry[K, V], ht.bucketCount)

	for i := 0; i < newSize; i++ {
		var key K
		var value V
		// Fscan ожидает пробельные разделители
		if _, err := fmt.Fscan(in, &key, &value); err != nil {
			return err
		}
		ht.Insert(key, value)
	}
	return nil
}