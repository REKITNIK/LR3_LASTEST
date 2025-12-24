package main

import (
	"bytes"
	"encoding/gob"
	"errors"
	"fmt"
	"io"
	"strings"
)

// node представляет узел дерева.
// Поля экспортируемые (с большой буквы) для работы encoding/gob.
type node[T any] struct {
	Data  T
	Left  *node[T]
	Right *node[T]
}

// FullBinaryTree реализует полное бинарное дерево.
// T должен быть comparable для поддержки поиска и удаления.
type FullBinaryTree[T comparable] struct {
	root *node[T]
	size int
}

// NewFullBinaryTree создает новое пустое дерево.
func NewFullBinaryTree[T comparable]() *FullBinaryTree[T] {
	return &FullBinaryTree[T]{
		root: nil,
		size: 0,
	}
}

// Clone создает глубокую копию дерева.
// Аналог конструктора копирования C++.
func (t *FullBinaryTree[T]) Clone() *FullBinaryTree[T] {
	newTree := NewFullBinaryTree[T]()
	newTree.size = t.size
	newTree.root = copyTree(t.root)
	return newTree
}

func copyTree[T any](n *node[T]) *node[T] {
	if n == nil {
		return nil
	}
	newNode := &node[T]{Data: n.Data}
	newNode.Left = copyTree(n.Left)
	newNode.Right = copyTree(n.Right)
	return newNode
}

// Clear очищает дерево.
func (t *FullBinaryTree[T]) Clear() {
	// В Go достаточно обнулить корень, GC соберет остальное.
	t.root = nil
	t.size = 0
}

// IsEmpty проверяет, пусто ли дерево.
func (t *FullBinaryTree[T]) IsEmpty() bool {
	return t.size == 0
}

// GetSize возвращает текущее количество узлов.
func (t *FullBinaryTree[T]) GetSize() int {
	return t.size
}

// Insert вставляет значение в дерево.
// Ищет первый лист (BFS) и добавляет к нему два дочерних узла с переданным значением.
func (t *FullBinaryTree[T]) Insert(value T) {
	if t.root == nil {
		t.root = &node[T]{Data: value}
		t.size = 1
		return
	}

	// BFS очередь
	queue := []*node[T]{t.root}

	for len(queue) > 0 {
		current := queue[0]
		queue = queue[1:] // pop

		// Если лист (нет детей), добавляем двоих для сохранения свойства полного дерева
		if current.Left == nil && current.Right == nil {
			current.Left = &node[T]{Data: value}
			current.Right = &node[T]{Data: value}
			t.size += 2
			return
		}

		if current.Left != nil {
			queue = append(queue, current.Left)
		}
		if current.Right != nil {
			queue = append(queue, current.Right)
		}
	}
}

// Find ищет значение в дереве.
func (t *FullBinaryTree[T]) Find(value T) bool {
	if t.root == nil {
		return false
	}

	queue := []*node[T]{t.root}
	for len(queue) > 0 {
		current := queue[0]
		queue = queue[1:]

		if current.Data == value {
			return true
		}

		if current.Left != nil {
			queue = append(queue, current.Left)
		}
		if current.Right != nil {
			queue = append(queue, current.Right)
		}
	}
	return false
}

// Remove удаляет значение из дерева.
// Использует стратегию замены на самый правый лист и удаление пары листьев.
func (t *FullBinaryTree[T]) Remove(value T) {
	if t.root == nil {
		return
	}

	var target *node[T]
	var parentOfTarget *node[T]

	// 1. Поиск удаляемого узла и его родителя
	type nodePair struct {
		n      *node[T]
		parent *node[T]
	}
	queue := []nodePair{{n: t.root, parent: nil}}
	found := false

	for len(queue) > 0 {
		currPair := queue[0]
		queue = queue[1:]

		if currPair.n.Data == value {
			target = currPair.n
			parentOfTarget = currPair.parent
			found = true
			break
		}

		if currPair.n.Left != nil {
			queue = append(queue, nodePair{n: currPair.n.Left, parent: currPair.n})
		}
		if currPair.n.Right != nil {
			queue = append(queue, nodePair{n: currPair.n.Right, parent: currPair.n})
		}
	}

	if !found {
		return
	}

	// Случай А: Удаляемый узел - лист
	if target.Left == nil && target.Right == nil {
		if parentOfTarget != nil {
			// Удаляем обоих детей родителя (чтобы сохранить полноту)
			// В C++ коде удаляется target и его "брат". Так как это FullBinaryTree,
			// если target - лист и не корень, у родителя точно 2 ребенка.
			parentOfTarget.Left = nil
			parentOfTarget.Right = nil
			t.size -= 2
		} else {
			// Удаление корня
			t.root = nil
			t.size = 0
		}
		return
	}

	// Случай Б: Удаляемый узел внутренний (есть дети)
	// Находим самый правый лист
	var rightmost *node[T]
	var rightmostParent *node[T]

	// Снова BFS для поиска самого нижнего правого
	leafQueue := []nodePair{{n: t.root, parent: nil}}
	for len(leafQueue) > 0 {
		currPair := leafQueue[0]
		leafQueue = leafQueue[1:]

		if currPair.n.Left == nil && currPair.n.Right == nil {
			rightmost = currPair.n
			rightmostParent = currPair.parent
		}

		if currPair.n.Left != nil {
			leafQueue = append(leafQueue, nodePair{n: currPair.n.Left, parent: currPair.n})
		}
		if currPair.n.Right != nil {
			leafQueue = append(leafQueue, nodePair{n: currPair.n.Right, parent: currPair.n})
		}
	}

	if rightmost != nil && rightmost != target {
		// Заменяем данные
		target.Data = rightmost.Data

		// Удаляем самый правый лист и его брата
		if rightmostParent != nil {
			rightmostParent.Left = nil
			rightmostParent.Right = nil
			t.size -= 2
		}
	}
}

// IsFullBinaryTree проверяет корректность структуры.
func (t *FullBinaryTree[T]) IsFullBinaryTree() bool {
	return isFullBinaryTreeHelper(t.root)
}

func isFullBinaryTreeHelper[T any](n *node[T]) bool {
	if n == nil {
		return true
	}
	// XOR: (Left==nil) != (Right==nil) означает, что есть только один ребенок
	if (n.Left == nil) != (n.Right == nil) {
		return false
	}
	return isFullBinaryTreeHelper(n.Left) && isFullBinaryTreeHelper(n.Right)
}

// Print выводит содержимое (BFS).
func (t *FullBinaryTree[T]) Print() {
	if t.root == nil {
		fmt.Println("Empty tree")
		return
	}
	fmt.Print("Level-order traversal: ")
	queue := []*node[T]{t.root}
	for len(queue) > 0 {
		curr := queue[0]
		queue = queue[1:]
		fmt.Printf("%v ", curr.Data)

		if curr.Left != nil {
			queue = append(queue, curr.Left)
		}
		if curr.Right != nil {
			queue = append(queue, curr.Right)
		}
	}
	fmt.Println()
}

// PrintInOrder выводит содержимое (In-Order).
func (t *FullBinaryTree[T]) PrintInOrder() {
	fmt.Print("In-order traversal: ")
	printInOrderHelper(t.root)
	fmt.Println()
}

func printInOrderHelper[T any](n *node[T]) {
	if n != nil {
		printInOrderHelper(n.Left)
		fmt.Printf("%v ", n.Data)
		printInOrderHelper(n.Right)
	}
}

// ==========================================
// Сериализация (Binary / Gob)
// ==========================================

// Serialize сохраняет дерево в бинарном формате (используя gob).
func (t *FullBinaryTree[T]) Serialize(out io.Writer) error {
	enc := gob.NewEncoder(out)
	// Сохраняем размер для быстрой проверки, хотя gob восстановит структуру и так
	if err := enc.Encode(t.size); err != nil {
		return err
	}
	// Сохраняем структуру рекурсивно через gob
	// Gob умеет кодировать графы ссылок, но для простоты и соответствия C++
	// мы можем просто закодировать корень, так как структура дерева определена жестко.
	if err := enc.Encode(t.root); err != nil {
		return err
	}
	return nil
}

// Deserialize восстанавливает дерево из бинарного формата.
func (t *FullBinaryTree[T]) Deserialize(in io.Reader) error {
	t.Clear()
	dec := gob.NewDecoder(in)

	var newSize int
	if err := dec.Decode(&newSize); err != nil {
		return err
	}
	t.size = newSize

	// Так как root - это указатель, gob корректно восстановит дерево,
	// если оно было записано целиком.
	// Однако, если поток содержит "nil", decode может ничего не записать в указатель,
	// поэтому убедимся, что мы читаем в переменную.
	var newRoot *node[T]
	if err := dec.Decode(&newRoot); err != nil {
		// EOF допустим если дерево пустое, но gob обычно кидает ошибку если данные ожидались
		return err
	}
	t.root = newRoot
	return nil
}

// ==========================================
// Сериализация (Text)
// ==========================================

// SerializeText сохраняет дерево в текстовом формате.
func (t *FullBinaryTree[T]) SerializeText(out io.Writer) error {
	if _, err := fmt.Fprintln(out, t.size); err != nil {
		return err
	}
	return serializeTextHelper(t.root, out)
}

func serializeTextHelper[T any](n *node[T], out io.Writer) error {
	if n == nil {
		_, err := fmt.Fprint(out, "null ")
		return err
	}
	if _, err := fmt.Fprintf(out, "%v ", n.Data); err != nil {
		return err
	}
	if err := serializeTextHelper(n.Left, out); err != nil {
		return err
	}
	if err := serializeTextHelper(n.Right, out); err != nil {
		return err
	}
	return nil
}

// DeserializeText восстанавливает дерево из текстового формата.
func (t *FullBinaryTree[T]) DeserializeText(in io.Reader) error {
	t.Clear()

	// Читаем все содержимое для парсинга токенов
	// В Go нет прямого аналога `in >> token`, поэтому читаем всё и разбиваем.
	// Для больших данных это неэффективно, но соответствует логике C++ потока.
	buf := new(bytes.Buffer)
	if _, err := buf.ReadFrom(in); err != nil {
		return err
	}

	tokens := strings.Fields(buf.String())
	if len(tokens) == 0 {
		return errors.New("empty input")
	}

	// Парсинг размера
	var newSize int
	_, err := fmt.Sscanf(tokens[0], "%d", &newSize)
	if err != nil {
		return fmt.Errorf("failed to parse size: %w", err)
	}
	t.size = newSize

	// Рекурсивный парсинг дерева
	tokenIndex := 1
	var parseErr error
	t.root, tokenIndex, parseErr = deserializeTextHelper[T](tokens, tokenIndex)
	return parseErr
}

func deserializeTextHelper[T comparable](tokens []string, index int) (*node[T], int, error) {
	if index >= len(tokens) {
		return nil, index, nil // Или ошибка, если ожидались данные
	}

	token := tokens[index]
	index++

	if token == "null" {
		return nil, index, nil
	}

	var value T
	// Sscanf требует указатель на конкретный тип.
	// Для Generic T это немного сложнее через fmt.
	// Используем временный интерфейс или reflection, но fmt.Sscan работает с any.
	n, err := fmt.Sscan(token, &value)
	if err != nil || n == 0 {
		return nil, index, fmt.Errorf("failed to parse value '%s': %v", token, err)
	}

	newNode := &node[T]{Data: value}
	
	var errL, errR error
	newNode.Left, index, errL = deserializeTextHelper[T](tokens, index)
	if errL != nil { return nil, index, errL }
	
	newNode.Right, index, errR = deserializeTextHelper[T](tokens, index)
	if errR != nil { return nil, index, errR }

	return newNode, index, nil
}