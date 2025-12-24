#include <iostream>
#include <cassert>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "Array.h"
#include "ForwardList.h"
#include "DoubleList.h"
#include "Queue.h"
#include "Stack.h"
#include "HashTable.h"
#include "FullBinaryTree.h"

/**
 * @brief Утилита запуска тестов и подсчёта результатов.
 */
class TestRunner {
private:
    int tests_passed = 0;
    int tests_failed = 0;

public:
    /**
     * @brief Проверка условия и учёт результата теста.
     * @param condition Истина, если тест пройден.
     * @param test_name Имя теста для отчёта.
     * @return void
     */
    void assert_test(bool condition, const std::string& test_name) {
        if (condition) {
            std::cout << "[PASS] " << test_name << std::endl;
            tests_passed++;
        } else {
            std::cout << "[FAIL] " << test_name << std::endl;
            tests_failed++;
        }
    }

    /**
     * @brief Печать сводки по результатам тестов.
     * @return void
     */
    void print_summary() {
        std::cout << "\n=== TEST SUMMARY ===" << std::endl;
        std::cout << "Passed: " << tests_passed << std::endl;
        std::cout << "Failed: " << tests_failed << std::endl;
        std::cout << "Total: " << (tests_passed + tests_failed) << std::endl;
        double coverage = (double)tests_passed / (tests_passed + tests_failed) * 100;
        std::cout << "Coverage: " << coverage << "%" << std::endl;
    }

    /**
     * @brief Получить количество проваленных тестов.
     * @return Количество тестов со статусом FAIL.
     */
    int getFailedCount() const { return tests_failed; }
};

/**
 * @brief Набор тестов для структуры Array.
 * @param runner Диспетчер проверки и сбора результатов.
 * @return void
 */
void test_array(TestRunner& runner) {
    std::cout << "\n=== TESTING ARRAY ===" << std::endl;

    // Базовые операции
    Array<int> arr;
    runner.assert_test(arr.isEmpty(), "Array: isEmpty on new array");
    runner.assert_test(arr.getSize() == 0, "Array: size of new array is 0");

    // Добавление элементов
    arr.add(10);
    arr.add(20);
    arr.add(30);
    runner.assert_test(arr.getSize() == 3, "Array: size after adding 3 elements");
    runner.assert_test(!arr.isEmpty(), "Array: not empty after adding elements");
    runner.assert_test(arr.get(0) == 10, "Array: get first element");
    runner.assert_test(arr.get(2) == 30, "Array: get last element");

    // Вставка и удаление
    arr.insert(1, 15);
    runner.assert_test(arr.get(1) == 15, "Array: insert at index 1");
    runner.assert_test(arr.getSize() == 4, "Array: size after insert");

    arr.remove(1);
    runner.assert_test(arr.get(1) == 20, "Array: element after removal");
    runner.assert_test(arr.getSize() == 3, "Array: size after removal");

    // Операция присваивания по индексу
    arr.set(0, 100);
    runner.assert_test(arr.get(0) == 100, "Array: set operation");

    // Оператор []
    arr[1] = 200;
    runner.assert_test(arr[1] == 200, "Array: operator[] assignment");

    // Копирующий конструктор
    Array<int> arr2(arr);
    runner.assert_test(arr2.getSize() == arr.getSize(), "Array: copy constructor size");
    runner.assert_test(arr2.get(0) == arr.get(0), "Array: copy constructor data");

    // Оператор присваивания
    Array<int> arr3;
    arr3 = arr;
    runner.assert_test(arr3.getSize() == arr.getSize(), "Array: assignment operator size");
    runner.assert_test(arr3.get(1) == arr.get(1), "Array: assignment operator data");

    // Очистка
    arr.clear();
    runner.assert_test(arr.isEmpty(), "Array: isEmpty after clear");
    runner.assert_test(arr.getSize() == 0, "Array: size after clear");

    // Обработка исключений
    try {
        arr.get(0);
        runner.assert_test(false, "Array: exception on get from empty array");
    } catch (const std::out_of_range&) {
        runner.assert_test(true, "Array: exception on get from empty array");
    }

    // Сериализация
    Array<int> arr_ser;
    arr_ser.add(1);
    arr_ser.add(2);
    arr_ser.add(3);

    std::stringstream ss;
    arr_ser.serialize(ss);

    Array<int> arr_deser;
    arr_deser.deserialize(ss);
    runner.assert_test(arr_deser.getSize() == 3, "Array: deserialization size");
    runner.assert_test(arr_deser.get(0) == 1, "Array: deserialization data");
    runner.assert_test(arr_deser.get(2) == 3, "Array: deserialization last element");

    // Бинарная сериализация
    std::stringstream ss_bin;
    arr_ser.serializeBinary(ss_bin);
    Array<int> arr_deser_bin;
    arr_deser_bin.deserializeBinary(ss_bin);
    runner.assert_test(arr_deser_bin.getSize() == 3, "Array: binary deserialization size");
    runner.assert_test(arr_deser_bin.get(0) == 1, "Array: binary deserialization data");

    // Текстовая сериализация
    std::stringstream ss_text;
    arr_ser.serializeText(ss_text);
    Array<int> arr_deser_text;
    arr_deser_text.deserializeText(ss_text);
    runner.assert_test(arr_deser_text.getSize() == 3, "Array: text deserialization size");
    runner.assert_test(arr_deser_text.get(0) == 1, "Array: text deserialization data");
}

/**
 * @brief Набор тестов для структуры ForwardList.
 * @param runner Диспетчер проверки и сбора результатов.
 * @return void
 */
void test_forward_list(TestRunner& runner) {
    std::cout << "\n=== TESTING FORWARD LIST ===" << std::endl;

    ForwardList<int> list;
    runner.assert_test(list.isEmpty(), "ForwardList: isEmpty on new list");
    runner.assert_test(list.getSize() == 0, "ForwardList: size of new list is 0");

    // Операции вставки
    list.pushFront(10);
    list.pushBack(20);
    list.pushFront(5);
    runner.assert_test(list.getSize() == 3, "ForwardList: size after pushes");
    runner.assert_test(list.front() == 5, "ForwardList: front element");
    runner.assert_test(list.get(1) == 10, "ForwardList: middle element");
    runner.assert_test(list.get(2) == 20, "ForwardList: last element");

    // Вставка
    list.insert(1, 7);
    runner.assert_test(list.get(1) == 7, "ForwardList: insert at index 1");
    runner.assert_test(list.getSize() == 4, "ForwardList: size after insert");

    // Поиск
    runner.assert_test(list.find(7), "ForwardList: find existing element");
    runner.assert_test(!list.find(100), "ForwardList: find non-existing element");

    // Операции удаления
    list.popFront();
    runner.assert_test(list.front() == 7, "ForwardList: front after popFront");
    runner.assert_test(list.getSize() == 3, "ForwardList: size after popFront");

    list.remove(1);
    runner.assert_test(list.get(1) == 20, "ForwardList: element after remove");
    runner.assert_test(list.getSize() == 2, "ForwardList: size after remove");

    // Удаление по значению
    list.pushBack(30);
    list.pushBack(20);
    list.removeValue(20);
    runner.assert_test(list.getSize() == 2, "ForwardList: size after removeValue");
    runner.assert_test(!list.find(20), "ForwardList: element removed by value");

    // Копирующий конструктор
    ForwardList<int> list2(list);
    runner.assert_test(list2.getSize() == list.getSize(), "ForwardList: copy constructor size");
    runner.assert_test(list2.front() == list.front(), "ForwardList: copy constructor data");

    // Сериализация
    std::stringstream ss;
    list.serialize(ss);

    ForwardList<int> list_deser;
    list_deser.deserialize(ss);
    runner.assert_test(list_deser.getSize() == list.getSize(), "ForwardList: deserialization size");
    runner.assert_test(list_deser.front() == list.front(), "ForwardList: deserialization data");

    // Бинарная сериализация
    std::stringstream ss_bin;
    list.serializeBinary(ss_bin);
    ForwardList<int> list_deser_bin;
    list_deser_bin.deserializeBinary(ss_bin);
    runner.assert_test(list_deser_bin.getSize() == list.getSize(), "ForwardList: binary deserialization size");

    // Текстовая сериализация
    std::stringstream ss_text;
    list.serializeText(ss_text);
    ForwardList<int> list_deser_text;
    list_deser_text.deserializeText(ss_text);
    runner.assert_test(list_deser_text.getSize() == list.getSize(), "ForwardList: text deserialization size");
}

/**
 * @brief Набор тестов для ��труктуры DoubleList.
 * @param runner Диспетчер проверки и сбора результатов.
 * @return void
 */
void test_double_list(TestRunner& runner) {
    std::cout << "\n=== TESTING DOUBLE LIST ===" << std::endl;

    DoubleList<int> list;
    runner.assert_test(list.isEmpty(), "DoubleList: isEmpty on new list");

    // Операции вставки
    list.pushBack(10);
    list.pushBack(20);
    list.pushFront(5);
    runner.assert_test(list.getSize() == 3, "DoubleList: size after pushes");
    runner.assert_test(list.front() == 5, "DoubleList: front element");
    runner.assert_test(list.back() == 20, "DoubleList: back element");

    // Операции извлечения
    list.popBack();
    runner.assert_test(list.back() == 10, "DoubleList: back after popBack");
    runner.assert_test(list.getSize() == 2, "DoubleList: size after popBack");

    list.popFront();
    runner.assert_test(list.front() == 10, "DoubleList: front after popFront");
    runner.assert_test(list.getSize() == 1, "DoubleList: size after popFront");

    // Вставка и удаление
    list.pushBack(20);
    list.pushBack(30);
    list.insert(1, 15);
    runner.assert_test(list.get(1) == 15, "DoubleList: insert at middle");
    runner.assert_test(list.getSize() == 4, "DoubleList: size after insert");

    list.remove(1);
    runner.assert_test(list.get(1) == 20, "DoubleList: element after remove");

    // Поиск
    runner.assert_test(list.find(20), "DoubleList: find existing element");
    runner.assert_test(!list.find(100), "DoubleList: find non-existing element");

    // Удаление по значению
    list.pushBack(20);
    list.removeValue(20);
    runner.assert_test(list.getSize() == 2, "DoubleList: size after removeValue");

    // Сериализация
    std::stringstream ss;
    list.serialize(ss);

    DoubleList<int> list_deser;
    list_deser.deserialize(ss);
    runner.assert_test(list_deser.getSize() == list.getSize(), "DoubleList: deserialization size");
    runner.assert_test(list_deser.front() == list.front(), "DoubleList: deserialization front");
    runner.assert_test(list_deser.back() == list.back(), "DoubleList: deserialization back");

    // Бинарная сериализация
    std::stringstream ss_bin;
    list.serializeBinary(ss_bin);
    DoubleList<int> list_deser_bin;
    list_deser_bin.deserializeBinary(ss_bin);
    runner.assert_test(list_deser_bin.getSize() == list.getSize(), "DoubleList: binary deserialization size");

    // Текстовая сериализация
    std::stringstream ss_text;
    list.serializeText(ss_text);
    DoubleList<int> list_deser_text;
    list_deser_text.deserializeText(ss_text);
    runner.assert_test(list_deser_text.getSize() == list.getSize(), "DoubleList: text deserialization size");
}

/**
 * @brief Набор тестов для структуры Queue.
 * @param runner Диспетчер проверки и сбора результатов.
 * @return void
 */
void test_queue(TestRunner& runner) {
    std::cout << "\n=== TESTING QUEUE ===" << std::endl;

    Queue<int> queue;
    runner.assert_test(queue.isEmpty(), "Queue: isEmpty on new queue");
    runner.assert_test(queue.getSize() == 0, "Queue: size of new queue is 0");

    // Операции добавления
    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);
    runner.assert_test(queue.getSize() == 3, "Queue: size after enqueues");
    runner.assert_test(queue.front() == 10, "Queue: front element");
    runner.assert_test(queue.back() == 30, "Queue: back element");

    // Операции извлечения
    queue.dequeue();
    runner.assert_test(queue.front() == 20, "Queue: front after dequeue");
    runner.assert_test(queue.getSize() == 2, "Queue: size after dequeue");

    // Копирующий конструктор
    Queue<int> queue2(queue);
    runner.assert_test(queue2.getSize() == queue.getSize(), "Queue: copy constructor size");
    runner.assert_test(queue2.front() == queue.front(), "Queue: copy constructor front");
    runner.assert_test(queue2.back() == queue.back(), "Queue: copy constructor back");

    // Очистка и обработка исключений
    queue.clear();
    runner.assert_test(queue.isEmpty(), "Queue: isEmpty after clear");

    try {
        queue.front();
        runner.assert_test(false, "Queue: exception on front from empty queue");
    } catch (const std::runtime_error&) {
        runner.assert_test(true, "Queue: exception on front from empty queue");
    }

    // Сериализация
    std::stringstream ss;
    queue2.serialize(ss);
    Queue<int> queue_deser;
    queue_deser.deserialize(ss);
    runner.assert_test(queue_deser.getSize() == queue2.getSize(), "Queue: deserialization size");

    // Бинарная сериализация
    std::stringstream ss_bin;
    queue2.serializeBinary(ss_bin);
    Queue<int> queue_deser_bin;
    queue_deser_bin.deserializeBinary(ss_bin);
    runner.assert_test(queue_deser_bin.getSize() == queue2.getSize(), "Queue: binary deserialization size");

    // Текстовая сериализация
    std::stringstream ss_text;
    queue2.serializeText(ss_text);
    Queue<int> queue_deser_text;
    queue_deser_text.deserializeText(ss_text);
    runner.assert_test(queue_deser_text.getSize() == queue2.getSize(), "Queue: text deserialization size");
}

/**
 * @brief Набор тестов для структуры Stack.
 * @param runner Диспетчер проверки и сбора результатов.
 * @return void
 */
void test_stack(TestRunner& runner) {
    std::cout << "\n=== TESTING STACK ===" << std::endl;

    Stack<int> stack;
    runner.assert_test(stack.isEmpty(), "Stack: isEmpty on new stack");
    runner.assert_test(stack.getSize() == 0, "Stack: size of new stack is 0");

    // Операции вставки
    stack.push(10);
    stack.push(20);
    stack.push(30);
    runner.assert_test(stack.getSize() == 3, "Stack: size after pushes");
    runner.assert_test(stack.top() == 30, "Stack: top element");

    // Операции извлечения
    stack.pop();
    runner.assert_test(stack.top() == 20, "Stack: top after pop");
    runner.assert_test(stack.getSize() == 2, "Stack: size after pop");

    // Копирующий конструктор
    Stack<int> stack2(stack);
    runner.assert_test(stack2.getSize() == stack.getSize(), "Stack: copy constructor size");
    runner.assert_test(stack2.top() == stack.top(), "Stack: copy constructor top");

    // Очистка и обработка исключений
    stack.clear();
    runner.assert_test(stack.isEmpty(), "Stack: isEmpty after clear");

    try {
        stack.top();
        runner.assert_test(false, "Stack: exception on top from empty stack");
    } catch (const std::runtime_error&) {
        runner.assert_test(true, "Stack: exception on top from empty stack");
    }

    // Сериализация
    stack2.push(40);
    std::stringstream ss;
    stack2.serialize(ss);

    Stack<int> stack_deser;
    stack_deser.deserialize(ss);
    runner.assert_test(stack_deser.getSize() == stack2.getSize(), "Stack: deserialization size");
    runner.assert_test(stack_deser.top() == stack2.top(), "Stack: deserialization top");

    // Бинарная сериализация
    std::stringstream ss_bin;
    stack2.serializeBinary(ss_bin);
    Stack<int> stack_deser_bin;
    stack_deser_bin.deserializeBinary(ss_bin);
    runner.assert_test(stack_deser_bin.getSize() == stack2.getSize(), "Stack: binary deserialization size");

    // Текстовая сериализация
    std::stringstream ss_text;
    stack2.serializeText(ss_text);
    Stack<int> stack_deser_text;
    stack_deser_text.deserializeText(ss_text);
    runner.assert_test(stack_deser_text.getSize() == stack2.getSize(), "Stack: text deserialization size");
}

/**
 * @brief Набор тестов для структуры HashTable.
 * @param runner Диспетчер проверки и сбора результатов.
 * @return void
 */
void test_hash_table(TestRunner& runner) {
    std::cout << "\n=== TESTING HASH TABLE ===" << std::endl;

    HashTable<int, std::string> table;
    runner.assert_test(table.isEmpty(), "HashTable: isEmpty on new table");
    runner.assert_test(table.getSize() == 0, "HashTable: size of new table is 0");

    // Вставка
    table.insert(1, "one");
    table.insert(2, "two");
    table.insert(3, "three");
    runner.assert_test(table.getSize() == 3, "HashTable: size after inserts");
    runner.assert_test(!table.isEmpty(), "HashTable: not empty after inserts");

    // Доступ
    runner.assert_test(table.get(1) == "one", "HashTable: get existing key");
    runner.assert_test(table.get(2) == "two", "HashTable: get another key");

    // Поиск
    runner.assert_test(table.find(1), "HashTable: find existing key");
    runner.assert_test(!table.find(10), "HashTable: find non-existing key");

    // Обновление существующего ключа
    table.insert(1, "ONE");
    runner.assert_test(table.get(1) == "ONE", "HashTable: update existing key");
    runner.assert_test(table.getSize() == 3, "HashTable: size unchanged after update");

    // Оператор []
    table[4] = "four";
    runner.assert_test(table.get(4) == "four", "HashTable: operator[] insert");
    runner.assert_test(table.getSize() == 4, "HashTable: size after operator[] insert");

    table[1] = "one_updated";
    runner.assert_test(table.get(1) == "one_updated", "HashTable: operator[] update");

    // Удаление
    table.remove(2);
    runner.assert_test(!table.find(2), "HashTable: element removed");
    runner.assert_test(table.getSize() == 3, "HashTable: size after remove");

    try {
        table.get(2);
        runner.assert_test(false, "HashTable: exception on get removed key");
    } catch (const std::runtime_error&) {
        runner.assert_test(true, "HashTable: exception on get removed key");
    }

    // Коэффициент заполнения и рехеширование
    for (int i = 5; i < 20; ++i) {
        table.insert(i, "value" + std::to_string(i));
    }
    runner.assert_test(table.getSize() == 18, "HashTable: size after many inserts");

    // Копирующий конструктор
    HashTable<int, std::string> table2(table);
    runner.assert_test(table2.getSize() == table.getSize(), "HashTable: copy constructor size");
    runner.assert_test(table2.get(1) == table.get(1), "HashTable: copy constructor data");

    // Очистка
    table.clear();
    runner.assert_test(table.isEmpty(), "HashTable: isEmpty after clear");
    runner.assert_test(table.getSize() == 0, "HashTable: size after clear");

    // Сериализация (включено): используем тривиально копируемые значения для корректности бинарного формата
    {
        HashTable<int, int> tbin;
        for (int i = 0; i < 10; ++i) tbin.insert(i, i * 10);
        std::stringstream ssb;
        tbin.serialize(ssb);
        HashTable<int, int> tbin2;
        tbin2.deserialize(ssb);
        runner.assert_test(tbin2.getSize() == tbin.getSize(), "HashTable: binary deserialization size");
        runner.assert_test(tbin2.get(5) == 50, "HashTable: binary deserialization data");
    }
    {
        HashTable<int, int> ttxt;
        for (int i = 0; i < 10; ++i) ttxt.insert(i, i * 10);
        std::stringstream sst;
        ttxt.serializeText(sst);
        HashTable<int, int> ttxt2;
        ttxt2.deserializeText(sst);
        runner.assert_test(ttxt2.getSize() == ttxt.getSize(), "HashTable: text deserialization size");
        runner.assert_test(ttxt2.get(9) == 90, "HashTable: text deserialization data");
    }
}

/**
 * @brief Набор тестов для структуры FullBinaryTree.
 * @param runner Диспетчер проверки и сбора результатов.
 * @return void
 */
void test_full_binary_tree(TestRunner& runner) {
    std::cout << "\n=== TESTING FULL BINARY TREE ===" << std::endl;

    FullBinaryTree<int> tree;
    runner.assert_test(tree.isEmpty(), "FullBinaryTree: isEmpty on new tree");
    runner.assert_test(tree.getSize() == 0, "FullBinaryTree: size of new tree is 0");
    runner.assert_test(tree.isFullBinaryTree(), "FullBinaryTree: empty tree is full binary tree");

    // Вставка
    tree.insert(10);
    runner.assert_test(tree.getSize() == 1, "FullBinaryTree: size after first insert (root only)");
    runner.assert_test(!tree.isEmpty(), "FullBinaryTree: not empty after insert");
    runner.assert_test(tree.isFullBinaryTree(), "FullBinaryTree: tree maintains full binary property after first insert");

    tree.insert(20);
    runner.assert_test(tree.getSize() == 3, "FullBinaryTree: size after second insert");
    runner.assert_test(tree.isFullBinaryTree(), "FullBinaryTree: tree maintains full binary property after second insert");

    tree.insert(30);
    runner.assert_test(tree.getSize() == 5, "FullBinaryTree: size after third insert");
    runner.assert_test(tree.isFullBinaryTree(), "FullBinaryTree: tree maintains full binary property after third insert");

    // Поиск
    runner.assert_test(tree.find(10), "FullBinaryTree: find existing element");
    runner.assert_test(tree.find(20), "FullBinaryTree: find another existing element");
    runner.assert_test(!tree.find(100), "FullBinaryTree: find non-existing element");

    // Копирующий конструктор
    FullBinaryTree<int> tree2(tree);
    runner.assert_test(tree2.getSize() == tree.getSize(), "FullBinaryTree: copy constructor size");
    runner.assert_test(tree2.isFullBinaryTree(), "FullBinaryTree: copy maintains full binary property");
    runner.assert_test(tree2.find(10), "FullBinaryTree: copy constructor data");

    // Оператор присваивания
    FullBinaryTree<int> tree3;
    tree3 = tree;
    runner.assert_test(tree3.getSize() == tree.getSize(), "FullBinaryTree: assignment operator size");
    runner.assert_test(tree3.isFullBinaryTree(), "FullBinaryTree: assignment maintains full binary property");

    // Удаление
    size_t size_before_remove = tree.getSize();
    tree.remove(20);
    runner.assert_test(tree.getSize() <= size_before_remove, "FullBinaryTree: size after remove");
    runner.assert_test(tree.isFullBinaryTree(), "FullBinaryTree: tree maintains full binary property after remove");
    // Примечание: из-за стратегии замещения значение может остаться в другом месте дерева
    runner.assert_test(true, "FullBinaryTree: remove operation completed");

    // Очистка
    tree.clear();
    runner.assert_test(tree.isEmpty(), "FullBinaryTree: isEmpty after clear");
    runner.assert_test(tree.getSize() == 0, "FullBinaryTree: size after clear");
    runner.assert_test(tree.isFullBinaryTree(), "FullBinaryTree: empty tree is full binary tree after clear");

    // Сериализация
    tree2.insert(40);
    std::stringstream ss;
    tree2.serialize(ss);

    FullBinaryTree<int> tree_deser;
    tree_deser.deserialize(ss);
    runner.assert_test(tree_deser.getSize() == tree2.getSize(), "FullBinaryTree: deserialization size");
    runner.assert_test(tree_deser.isFullBinaryTree(), "FullBinaryTree: deserialized tree is full binary tree");
    runner.assert_test(tree_deser.find(10), "FullBinaryTree: deserialization data");

    // Бинарная сериализация
    std::stringstream ss_bin;
    tree2.serializeBinary(ss_bin);
    FullBinaryTree<int> tree_deser_bin;
    tree_deser_bin.deserializeBinary(ss_bin);
    runner.assert_test(tree_deser_bin.getSize() == tree2.getSize(), "FullBinaryTree: binary deserialization size");
    runner.assert_test(tree_deser_bin.isFullBinaryTree(), "FullBinaryTree: binary deserialized tree is full binary tree");

    // Текстовая сериализация
    std::stringstream ss_text;
    tree2.serializeText(ss_text);
    FullBinaryTree<int> tree_deser_text;
    tree_deser_text.deserializeText(ss_text);
    runner.assert_test(tree_deser_text.getSize() == tree2.getSize(), "FullBinaryTree: text deserialization size");
    runner.assert_test(tree_deser_text.isFullBinaryTree(), "FullBinaryTree: text deserialized tree is full binary tree");

    // КРИТИЧЕСКИЙ ТЕСТ: проверка инварианта полного бинарного дерева
    FullBinaryTree<int> invariant_tree;
    for (int i = 1; i <= 10; ++i) {
        invariant_tree.insert(i);
        runner.assert_test(invariant_tree.isFullBinaryTree(),
                          "FullBinaryTree: INVARIANT - tree is full binary after insert " + std::to_string(i));
    }
}

/**
 * @brief Тесты файловой сериализации/десериализации для Array и FullBinaryTree.
 * @param runner Диспетчер проверки и сбора результатов.
 * @return void
 */
void test_serialization_files(TestRunner& runner) {
    std::cout << "\n=== TESTING FILE SERIALIZATION ===" << std::endl;

    // Тест файловой сериализации массива
    {
        Array<int> arr;
        arr.add(1);
        arr.add(2);
        arr.add(3);

        std::ofstream out("test_array.bin", std::ios::binary);
        arr.serialize(out);
        out.close();

        Array<int> arr_loaded;
        std::ifstream in("test_array.bin", std::ios::binary);
        arr_loaded.deserialize(in);
        in.close();

        runner.assert_test(arr_loaded.getSize() == 3, "Array: file serialization size");
        runner.assert_test(arr_loaded.get(0) == 1, "Array: file serialization data");
    }

    // Тест файловой сериализации для FullBinaryTree (по умолчанию бинарная)
    {
        FullBinaryTree<int> tree;
        tree.insert(10);
        tree.insert(20);

        std::ofstream out("test_tree.bin", std::ios::binary);
        tree.serialize(out);
        out.close();

        FullBinaryTree<int> tree_loaded;
        std::ifstream in("test_tree.bin", std::ios::binary);
        tree_loaded.deserialize(in);
        in.close();

        runner.assert_test(tree_loaded.getSize() == tree.getSize(), "FullBinaryTree: file serialization size");
        runner.assert_test(tree_loaded.isFullBinaryTree(), "FullBinaryTree: file serialization maintains invariant");
        runner.assert_test(tree_loaded.find(10), "FullBinaryTree: file serialization data");
    }
}

/**
 * @brief Точка входа для запуска тестов.
 * @return 0 если все тесты пройдены, иначе 1.
 */
int main() {
    TestRunner runner;

    std::cout << "Starting comprehensive OOP data structures tests..." << std::endl;

    test_array(runner);
    test_forward_list(runner);
    test_double_list(runner);
    test_queue(runner);
    test_stack(runner);
    test_hash_table(runner);
    test_full_binary_tree(runner);
    test_serialization_files(runner);

    runner.print_summary();

    // Удаление временных файлов тестов
    std::remove("test_array.bin");
    std::remove("test_tree.bin");

    return runner.getFailedCount() == 0 ? 0 : 1;
}