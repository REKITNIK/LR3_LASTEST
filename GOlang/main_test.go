package main

import (
	"bytes"
	"testing"
)

// Helper для проверки ошибок
func assertNoError(t *testing.T, err error, msg string) {
	t.Helper()
	if err != nil {
		t.Errorf("%s: unexpected error: %v", msg, err)
	}
}

// Helper для проверки ошибок (ожидаем ошибку)
func assertError(t *testing.T, err error, msg string) {
	t.Helper()
	if err == nil {
		t.Errorf("%s: expected error, got nil", msg)
	}
}

// Helper для проверки равенства
func assertEqual[T comparable](t *testing.T, got, expected T, msg string) {
	t.Helper()
	if got != expected {
		t.Errorf("%s: expected %v, got %v", msg, expected, got)
	}
}

// === TESTING ARRAY ===
func TestArray(t *testing.T) {
	t.Log("=== TESTING ARRAY ===")

	// Используем NewArray напрямую, так как мы в одном пакете main
	arr := NewArray[int](0)
	if !arr.IsEmpty() {
		t.Error("Array: isEmpty on new array failed")
	}
	if arr.Size() != 0 {
		t.Error("Array: size of new array is not 0")
	}

	// Добавление
	arr.Add(10)
	arr.Add(20)
	arr.Add(30)
	assertEqual(t, arr.Size(), 3, "Array: size after adding 3 elements")

	val, _ := arr.Get(0)
	assertEqual(t, val, 10, "Array: get first element")
	val, _ = arr.Get(2)
	assertEqual(t, val, 30, "Array: get last element")

	// Вставка и удаление
	err := arr.Insert(1, 15)
	assertNoError(t, err, "Array: insert")
	val, _ = arr.Get(1)
	assertEqual(t, val, 15, "Array: insert check value")
	assertEqual(t, arr.Size(), 4, "Array: size after insert")

	err = arr.Remove(1)
	assertNoError(t, err, "Array: remove")
	val, _ = arr.Get(1)
	assertEqual(t, val, 20, "Array: element after removal")
	assertEqual(t, arr.Size(), 3, "Array: size after removal")

	// Set
	err = arr.Set(0, 100)
	assertNoError(t, err, "Array: set")
	val, _ = arr.Get(0)
	assertEqual(t, val, 100, "Array: verify set")

	// Очистка
	arr.Clear()
	if !arr.IsEmpty() {
		t.Error("Array: isEmpty after clear")
	}

	// Обработка ошибок
	_, err = arr.Get(0)
	assertError(t, err, "Array: should return error on get from empty")

	// Сериализация (Gob)
	arrSer := NewArray[int](0)
	arrSer.Add(1)
	arrSer.Add(2)
	arrSer.Add(3)

	var buf bytes.Buffer
	err = arrSer.Serialize(&buf)
	assertNoError(t, err, "Array: serialize")

	arrDeser := NewArray[int](0)
	err = arrDeser.Deserialize(&buf)
	assertNoError(t, err, "Array: deserialize")

	assertEqual(t, arrDeser.Size(), 3, "Array: deserialization size")
	val, _ = arrDeser.Get(0)
	assertEqual(t, val, 1, "Array: deserialization data 0")
}

// === TESTING FORWARD LIST ===
func TestForwardList(t *testing.T) {
	t.Log("=== TESTING FORWARD LIST ===")

	list := NewForwardList[int]()
	if !list.IsEmpty() {
		t.Error("ForwardList: expected empty")
	}

	// Вставка
	list.PushFront(10)
	list.PushBack(20)
	list.PushFront(5)
	assertEqual(t, list.GetSize(), 3, "ForwardList: size check")

	front, _ := list.Front()
	assertEqual(t, *front, 5, "ForwardList: front check")

	valPtr, _ := list.Get(1)
	assertEqual(t, *valPtr, 10, "ForwardList: middle element")

	valPtr, _ = list.Get(2)
	assertEqual(t, *valPtr, 20, "ForwardList: last element")

	// Вставка по индексу
	list.Insert(1, 7)
	valPtr, _ = list.Get(1)
	assertEqual(t, *valPtr, 7, "ForwardList: insert at index 1")

	// Поиск
	if !list.Find(7) {
		t.Error("ForwardList: find existing element failed")
	}
	if list.Find(100) {
		t.Error("ForwardList: find non-existing element failed")
	}

	// Удаление
	list.PopFront()
	front, _ = list.Front()
	assertEqual(t, *front, 7, "ForwardList: front after popFront")

	list.Remove(1)
	// Текущий список: [7, 20] (было [7, 10, 20], удалили индекс 1)
	valPtr, _ = list.Get(1)
	assertEqual(t, *valPtr, 20, "ForwardList: element after remove")

	// Clone
	list2 := list.Clone()
	assertEqual(t, list2.GetSize(), list.GetSize(), "ForwardList: clone size")
	f1, _ := list.Front()
	f2, _ := list2.Front()
	assertEqual(t, *f1, *f2, "ForwardList: clone data")

	// Сериализация (Binary)
	var bufBin bytes.Buffer
	err := list.SerializeBinary(&bufBin)
	assertNoError(t, err, "ForwardList: binary serialize")

	listDeserBin := NewForwardList[int]()
	err = listDeserBin.DeserializeBinary(&bufBin)
	assertNoError(t, err, "ForwardList: binary deserialize")
	assertEqual(t, listDeserBin.GetSize(), list.GetSize(), "ForwardList: check size after bin deserialize")

	// Сериализация (Text)
	var bufText bytes.Buffer
	list.SerializeText(&bufText)

	listDeserText := NewForwardList[int]()
	err = listDeserText.DeserializeText(&bufText)
	assertNoError(t, err, "ForwardList: text deserialize")
	assertEqual(t, listDeserText.GetSize(), list.GetSize(), "ForwardList: check size after text deserialize")
}

// === TESTING DOUBLE LIST ===
func TestDoubleList(t *testing.T) {
	t.Log("=== TESTING DOUBLE LIST ===")

	list := NewDoubleList[int]()
	if !list.IsEmpty() {
		t.Error("DoubleList: expected empty")
	}

	list.PushBack(10)
	list.PushBack(20)
	list.PushFront(5)

	assertEqual(t, list.GetSize(), 3, "DoubleList: size")
	f, _ := list.Front()
	assertEqual(t, *f, 5, "DoubleList: front")
	b, _ := list.Back()
	assertEqual(t, *b, 20, "DoubleList: back")

	list.PopBack()
	b, _ = list.Back()
	assertEqual(t, *b, 10, "DoubleList: back after popBack")

	list.PopFront()
	f, _ = list.Front()
	assertEqual(t, *f, 10, "DoubleList: front after popFront")

	// Insert
	list.PushBack(20)
	list.PushBack(30)  // [10, 20, 30]
	list.Insert(1, 15) // [10, 15, 20, 30]

	val, _ := list.Get(1)
	assertEqual(t, *val, 15, "DoubleList: insert middle")

	// Сериализация
	var buf bytes.Buffer
	list.SerializeBinary(&buf)
	listDeser := NewDoubleList[int]()
	listDeser.DeserializeBinary(&buf)

	assertEqual(t, listDeser.GetSize(), list.GetSize(), "DoubleList: deserialization size")
	f1, _ := list.Front()
	f2, _ := listDeser.Front()
	assertEqual(t, *f1, *f2, "DoubleList: deserialization data")
}

// === TESTING QUEUE ===
func TestQueue(t *testing.T) {
	t.Log("=== TESTING QUEUE ===")

	q := NewQueue[int]()
	if !q.IsEmpty() {
		t.Error("Queue: expected empty")
	}

	q.Enqueue(10)
	q.Enqueue(20)
	q.Enqueue(30)

	assertEqual(t, q.Size(), 3, "Queue: size")
	f, _ := q.Front()
	assertEqual(t, f, 10, "Queue: front")
	b, _ := q.Back()
	assertEqual(t, b, 30, "Queue: back")

	q.Dequeue()
	f, _ = q.Front()
	assertEqual(t, f, 20, "Queue: front after dequeue")

	// Очистка
	q.Clear()
	if !q.IsEmpty() {
		t.Error("Queue: not empty after clear")
	}
	_, err := q.Front()
	assertError(t, err, "Queue: error on empty front")

	// Сериализация (Text)
	q2 := NewQueue[int]()
	q2.Enqueue(1)
	q2.Enqueue(2)
	var buf bytes.Buffer
	q2.SerializeText(&buf)

	qDeser := NewQueue[int]()
	qDeser.DeserializeText(&buf)
	assertEqual(t, qDeser.Size(), 2, "Queue: text deser size")
	f, _ = qDeser.Front()
	assertEqual(t, f, 1, "Queue: text deser front")
}

// === TESTING STACK ===
func TestStack(t *testing.T) {
	t.Log("=== TESTING STACK ===")

	s := NewStack[int]()
	if !s.IsEmpty() {
		t.Error("Stack: expected empty")
	}

	s.Push(10)
	s.Push(20)
	s.Push(30)

	assertEqual(t, s.Size(), 3, "Stack: size")
	top, _ := s.Top()
	assertEqual(t, top, 30, "Stack: top")

	s.Pop()
	top, _ = s.Top()
	assertEqual(t, top, 20, "Stack: top after pop")

	// Сериализация
	s2 := s.Clone()
	var buf bytes.Buffer
	err := s2.Serialize(&buf)
	assertNoError(t, err, "Stack: serialize")

	sDeser := NewStack[int]()
	err = sDeser.Deserialize(&buf)
	assertNoError(t, err, "Stack: deserialize")

	assertEqual(t, sDeser.Size(), s2.Size(), "Stack: deser size")
	t1, _ := s2.Top()
	t2, _ := sDeser.Top()
	assertEqual(t, t1, t2, "Stack: deser top")
}

// === TESTING HASH TABLE ===
func TestHashTable(t *testing.T) {
	t.Log("=== TESTING HASH TABLE ===")

	ht := NewHashTable[int, string]()
	if !ht.IsEmpty() {
		t.Error("HashTable: expected empty")
	}

	ht.Insert(1, "one")
	ht.Insert(2, "two")
	ht.Insert(3, "three")

	assertEqual(t, ht.GetSize(), 3, "HashTable: size")

	val, err := ht.Get(1)
	assertNoError(t, err, "HashTable: get")
	assertEqual(t, val, "one", "HashTable: value check")

	if !ht.Find(2) {
		t.Error("HashTable: find existing")
	}
	if ht.Find(99) {
		t.Error("HashTable: find non-existing")
	}

	// Update
	ht.Insert(1, "ONE")
	val, _ = ht.Get(1)
	assertEqual(t, val, "ONE", "HashTable: update check")

	// Remove
	err = ht.Remove(2)
	assertNoError(t, err, "HashTable: remove")
	if ht.Find(2) {
		t.Error("HashTable: found removed element")
	}

	// Сериализация (Text)
	var buf bytes.Buffer
	err = ht.SerializeText(&buf)
	assertNoError(t, err, "HashTable: serialize text")

	htDeser := NewHashTable[int, string]()
	err = htDeser.DeserializeText(&buf)
	assertNoError(t, err, "HashTable: deserialize text")

	val, _ = htDeser.Get(1)
	assertEqual(t, val, "ONE", "HashTable: deser value")
}

// === TESTING FULL BINARY TREE ===
func TestFullBinaryTree(t *testing.T) {
	t.Log("=== TESTING FULL BINARY TREE ===")

	tree := NewFullBinaryTree[int]()
	if !tree.IsFullBinaryTree() {
		t.Error("FullBinaryTree: empty tree should be full")
	}

	tree.Insert(10)
	assertEqual(t, tree.GetSize(), 1, "Tree: size 1")
	if !tree.IsFullBinaryTree() {
		t.Error("Tree: invariant check 1")
	}

	tree.Insert(20)
	// Добавляет 20 как детей к 10 -> 10 (L:20, R:20) для сохранения полноты
	assertEqual(t, tree.GetSize(), 3, "Tree: size 3")
	if !tree.IsFullBinaryTree() {
		t.Error("Tree: invariant check 2")
	}

	tree.Insert(30)
	assertEqual(t, tree.GetSize(), 5, "Tree: size 5")
	if !tree.IsFullBinaryTree() {
		t.Error("Tree: invariant check 3")
	}

	if !tree.Find(10) {
		t.Error("Tree: find 10")
	}
	if !tree.Find(30) {
		t.Error("Tree: find 30")
	}

	// Сериализация
	var buf bytes.Buffer
	err := tree.Serialize(&buf)
	assertNoError(t, err, "Tree: serialize")

	treeDeser := NewFullBinaryTree[int]()
	err = treeDeser.Deserialize(&buf)
	assertNoError(t, err, "Tree: deserialize")

	assertEqual(t, treeDeser.GetSize(), tree.GetSize(), "Tree: deser size")
	if !treeDeser.IsFullBinaryTree() {
		t.Error("Tree: deser invariant")
	}
}

// Интеграционный тест: Файловая сериализация
func TestSerializationFiles(t *testing.T) {
	t.Log("=== TESTING FILE SERIALIZATION SIMULATION ===")

	// Вместо реальных файлов используем Buffer, но логика идентична
	// Array
	arr := NewArray[int](0)
	arr.Add(1)
	arr.Add(2)

	var buf bytes.Buffer
	if err := arr.Serialize(&buf); err != nil {
		t.Fatalf("Failed to serialize array: %v", err)
	}

	arrLoaded := NewArray[int](0)
	if err := arrLoaded.Deserialize(&buf); err != nil {
		t.Fatalf("Failed to deserialize array: %v", err)
	}

	assertEqual(t, arrLoaded.Size(), 2, "FileSim: array size")
}