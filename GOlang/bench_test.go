package main

import (
	"bytes"
	"math/rand"
	"testing"
)

// --- ARRAY BENCHMARKS ---

func BenchmarkArray_Add(b *testing.B) {
	arr := NewArray[int](0)
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		arr.Add(i)
	}
}

func BenchmarkArray_Get(b *testing.B) {
	arr := NewArray[int](10000)
	for i := 0; i < 10000; i++ {
		arr.Add(i)
	}
	// Фиксированный seed для детерминизма
	rnd := rand.New(rand.NewSource(42))
	size := arr.Size()

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		idx := rnd.Intn(size)
		_, _ = arr.Get(idx)
	}
}

func BenchmarkArray_RemoveBack(b *testing.B) {
	// Для Remove нам нужно постоянно заполнять массив
	b.StopTimer()
	arr := NewArray[int](b.N)
	for i := 0; i < b.N; i++ {
		arr.Add(i)
	}
	b.StartTimer()

	for i := 0; i < b.N; i++ {
		// Удаляем с конца
		if arr.Size() > 0 {
			_ = arr.Remove(arr.Size() - 1)
		} else {
			// Если массив пуст, приостанавливаем таймер и заполняем
			b.StopTimer()
			for k := 0; k < 1000; k++ {
				arr.Add(k)
			}
			b.StartTimer()
			_ = arr.Remove(arr.Size() - 1)
		}
	}
}

// --- FORWARD LIST BENCHMARKS ---

func BenchmarkForwardList_PushFront(b *testing.B) {
	list := NewForwardList[int]()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.PushFront(i)
	}
}

func BenchmarkForwardList_Find(b *testing.B) {
	list := NewForwardList[int]()
	// Заполняем 1000 элементами
	for i := 0; i < 1000; i++ {
		list.PushFront(i)
	}
	rnd := rand.New(rand.NewSource(42))

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		target := rnd.Intn(1000)
		list.Find(target)
	}
}

// --- DOUBLE LIST BENCHMARKS ---

func BenchmarkDoubleList_PushBack(b *testing.B) {
	list := NewDoubleList[int]()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		list.PushBack(i)
	}
}

func BenchmarkDoubleList_Access(b *testing.B) {
	list := NewDoubleList[int]()
	for i := 0; i < 1000; i++ {
		list.PushBack(i)
	}
	rnd := rand.New(rand.NewSource(42))
	size := list.GetSize()

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		idx := rnd.Intn(size)
		_, _ = list.Get(idx)
	}
}

// --- QUEUE BENCHMARKS ---

func BenchmarkQueue_Enqueue(b *testing.B) {
	q := NewQueue[int]()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		q.Enqueue(i)
	}
}

func BenchmarkQueue_Dequeue(b *testing.B) {
	q := NewQueue[int]()
	b.StopTimer()
	for i := 0; i < b.N; i++ {
		q.Enqueue(i)
	}
	b.StartTimer()

	for i := 0; i < b.N; i++ {
		if q.IsEmpty() {
			b.StopTimer()
			q.Enqueue(i)
			b.StartTimer()
		}
		q.Dequeue()
	}
}

// --- STACK BENCHMARKS ---

func BenchmarkStack_Push(b *testing.B) {
	s := NewStack[int]()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		s.Push(i)
	}
}

func BenchmarkStack_Pop(b *testing.B) {
	s := NewStack[int]()
	b.StopTimer()
	for i := 0; i < b.N; i++ {
		s.Push(i)
	}
	b.StartTimer()

	for i := 0; i < b.N; i++ {
		if s.IsEmpty() {
			b.StopTimer()
			s.Push(i)
			b.StartTimer()
		}
		s.Pop()
	}
}

// --- HASHTABLE BENCHMARKS ---

func BenchmarkHashTable_Insert(b *testing.B) {
	ht := NewHashTable[int, int]()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		ht.Insert(i, i*2)
	}
}

func BenchmarkHashTable_Find(b *testing.B) {
	ht := NewHashTable[int, int]()
	for i := 0; i < 10000; i++ {
		ht.Insert(i, i*2)
	}
	rnd := rand.New(rand.NewSource(42))

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		key := rnd.Intn(10000)
		ht.Find(key)
	}
}

// --- FULL BINARY TREE BENCHMARKS ---

func BenchmarkFullBinaryTree_Insert(b *testing.B) {
	tree := NewFullBinaryTree[int]()
	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		tree.Insert(i)
	}
}

func BenchmarkFullBinaryTree_InvariantCheck(b *testing.B) {
	tree := NewFullBinaryTree[int]()
	// Создаем дерево среднего размера
	for i := 0; i < 1000; i++ {
		tree.Insert(i)
	}

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		tree.IsFullBinaryTree()
	}
}

// --- SERIALIZATION BENCHMARKS ---

func BenchmarkSerialization_Array(b *testing.B) {
	arr := NewArray[int](0)
	for i := 0; i < 1000; i++ {
		arr.Add(i)
	}

	var buf bytes.Buffer
	buf.Grow(10000)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		buf.Reset()
		arr.Serialize(&buf)
	}
}

func BenchmarkDeserialization_Array(b *testing.B) {
	arr := NewArray[int](0)
	for i := 0; i < 1000; i++ {
		arr.Add(i)
	}
	var buf bytes.Buffer
	arr.Serialize(&buf)
	data := buf.Bytes()

	target := NewArray[int](0)
	r := bytes.NewReader(data)

	b.ResetTimer()
	for i := 0; i < b.N; i++ {
		r.Reset(data)
		target.Deserialize(r)
	}
}