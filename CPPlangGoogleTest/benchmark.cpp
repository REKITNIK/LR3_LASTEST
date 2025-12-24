/**
 * @file 
 * @brief Программа для бенчмаркинга и сравнения производительности различных структур данных.
 *
 * Файл содержит набор тестов производительности для массива, списков, очередей,
 * стека, хеш-таблицы и бинарного дерева.
 * Результаты выводятся в консоль и дублируются в файл benchmark_results.txt.
 */

#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <iomanip>
#include <sstream>
#include "Array.h"
#include "ForwardList.h"
#include "DoubleList.h"
#include "Queue.h"
#include "Stack.h"
#include "HashTable.h"
#include "FullBinaryTree.h"

/**
 * @brief Глобальный поток вывода в файл.
 * Инициализируется при запуске программы.
 */
std::ofstream resultsFile("benchmark_results.txt");

/**
 * @brief Класс для измерения времени выполнения операций.
 *
 * Использует std::chrono::high_resolution_clock для точного замера интервалов.
 */
class BenchmarkTimer {
private:
    std::chrono::high_resolution_clock::time_point start_time; ///< Время начала отсчета.

public:
    /**
     * @brief Запускает или перезапускает таймер.
     */
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    /**
     * @brief Останавливает таймер и возвращает прошедшее время.
     * @return Время выполнения в миллисекундах.
     */
    double stop() {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
        return duration.count() / 1000.0;
    }
};

/**
 * @brief Выводит заголовок секции бенчмарка в консоль и файл.
 * @param structure_name Название тестируемой структуры данных.
 */
void print_header(const std::string& structure_name) {
    // Вывод в консоль
    std::cout << "\n=== " << structure_name << " BENCHMARK ===" << std::endl;
    std::cout << std::setw(15) << "Operation" << std::setw(15) << "Time (ms)" << std::setw(15) << "Ops/sec" << std::endl;
    std::cout << std::string(45, '-') << std::endl;

    // Вывод в файл
    if (resultsFile.is_open()) {
        resultsFile << "\n=== " << structure_name << " BENCHMARK ===" << std::endl;
        resultsFile << std::setw(15) << "Operation" << std::setw(15) << "Time (ms)" << std::setw(15) << "Ops/sec" << std::endl;
        resultsFile << std::string(45, '-') << std::endl;
    }
}

/**
 * @brief Форматированно выводит результаты теста в консоль и файл.
 * @param operation Название операции (например, "Insert", "Find").
 * @param time_ms Затраченное время в миллисекундах.
 * @param operations Количество выполненных операций.
 */
void print_result(const std::string& operation, double time_ms, int operations) {
    double ops_per_sec = operations / (time_ms / 1000.0);
    
    // Вывод в консоль
    std::cout << std::setw(15) << operation
              << std::setw(15) << std::fixed << std::setprecision(3) << time_ms
              << std::setw(15) << std::fixed << std::setprecision(0) << ops_per_sec << std::endl;

    // Вывод в файл
    if (resultsFile.is_open()) {
        resultsFile << std::setw(15) << operation
                    << std::setw(15) << std::fixed << std::setprecision(3) << time_ms
                    << std::setw(15) << std::fixed << std::setprecision(0) << ops_per_sec << std::endl;
    }
}

/**
 * @brief Тестирование производительности динамического массива (Array).
 *
 * Проверяет операции вставки, случайного доступа, линейного поиска и удаления.
 */
void benchmark_array() {
    print_header("ARRAY");

    const int N = 10000;
    BenchmarkTimer timer;

    // Вставка в динамический массив
    Array<int> arr;
    timer.start();
    for (int i = 0; i < N; ++i) {
        arr.add(i);
    }
    double insert_time = timer.stop();
    print_result("Insert", insert_time, N);

    // Случайный доступ по индексу
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, N - 1);

    timer.start();
    volatile int sum = 0;
    for (int i = 0; i < N; ++i) {
        sum += arr.get(dis(gen));
    }
    double access_time = timer.stop();
    print_result("Random Access", access_time, N);

    // Линейный поиск
    timer.start();
    int found_count = 0;
    for (int i = 0; i < 1000; ++i) {
        for (size_t j = 0; j < arr.getSize(); ++j) {
            if (arr.get(j) == i) {
                found_count++;
                break;
            }
        }
    }
    double find_time = timer.stop();
    print_result("Find", find_time, 1000);

    // Удаление с конца (быстро)
    timer.start();
    for (int i = 0; i < 1000 && arr.getSize() > 0; ++i) {
        arr.remove(arr.getSize() - 1);
    }
    double remove_time = timer.stop();
    print_result("Remove", remove_time, 1000);
}

/**
 * @brief Тестирование производительности односвязного списка (ForwardList).
 *
 * Проверяет вставку в начало, последовательный доступ, поиск и удаление из начала.
 */
void benchmark_forward_list() {
    print_header("FORWARD LIST");

    const int N = 10000;
    BenchmarkTimer timer;

    // Вставка в голову
    ForwardList<int> list;
    timer.start();
    for (int i = 0; i < N; ++i) {
        list.pushFront(i);
    }
    double insert_time = timer.stop();
    print_result("Insert Front", insert_time, N);

    // Последовательный доступ
    timer.start();
    volatile int sum = 0;
    for (size_t i = 0; i < list.getSize() && i < 1000; ++i) {
        sum += list.get(i);
    }
    double access_time = timer.stop();
    print_result("Sequential Access", access_time, 1000);

    // Поиск значения
    timer.start();
    int found_count = 0;
    for (int i = 0; i < 1000; ++i) {
        if (list.find(i)) {
            found_count++;
        }
    }
    double find_time = timer.stop();
    print_result("Find", find_time, 1000);

    // Удаление с головы
    timer.start();
    for (int i = 0; i < 1000 && list.getSize() > 0; ++i) {
        list.popFront();
    }
    double remove_time = timer.stop();
    print_result("Remove Front", remove_time, 1000);
}

/**
 * @brief Тестирование производительности двусвязного списка (DoubleList).
 *
 * Проверяет вставку в конец, последовательный доступ, поиск и удаление с конца.
 */
void benchmark_double_list() {
    print_header("DOUBLE LIST");

    const int N = 10000;
    BenchmarkTimer timer;

    // Вставка в хвост
    DoubleList<int> list;
    timer.start();
    for (int i = 0; i < N; ++i) {
        list.pushBack(i);
    }
    double insert_time = timer.stop();
    print_result("Insert Back", insert_time, N);

    // Последовательный доступ
    timer.start();
    volatile int sum = 0;
    for (size_t i = 0; i < list.getSize() && i < 1000; ++i) {
        sum += list.get(i);
    }
    double access_time = timer.stop();
    print_result("Sequential Access", access_time, 1000);

    // Поиск значения
    timer.start();
    int found_count = 0;
    for (int i = 0; i < 1000; ++i) {
        if (list.find(i)) {
            found_count++;
        }
    }
    double find_time = timer.stop();
    print_result("Find", find_time, 1000);

    // Удаление с хвоста
    timer.start();
    for (int i = 0; i < 1000 && list.getSize() > 0; ++i) {
        list.popBack();
    }
    double remove_time = timer.stop();
    print_result("Remove Back", remove_time, 1000);
}

/**
 * @brief Тестирование производительности очереди (Queue).
 *
 * Проверяет операции enqueue, dequeue и доступ к элементам.
 */
void benchmark_queue() {
    print_header("QUEUE");

    const int N = 10000;
    BenchmarkTimer timer;

    // Добавление в очередь
    Queue<int> queue;
    timer.start();
    for (int i = 0; i < N; ++i) {
        queue.enqueue(i);
    }
    double enqueue_time = timer.stop();
    print_result("Enqueue", enqueue_time, N);

    // Доступ к голове/хвосту
    timer.start();
    volatile int sum = 0;
    for (int i = 0; i < 1000; ++i) {
        sum += queue.front();
        sum += queue.back();
    }
    double access_time = timer.stop();
    print_result("Access", access_time, 2000);

    // Извлечение из очереди
    timer.start();
    for (int i = 0; i < 1000 && queue.getSize() > 0; ++i) {
        queue.dequeue();
    }
    double dequeue_time = timer.stop();
    print_result("Dequeue", dequeue_time, 1000);
}

/**
 * @brief Тестирование производительности стека (Stack).
 *
 * Проверяет операции push, pop и доступ к вершине.
 */
void benchmark_stack() {
    print_header("STACK");

    const int N = 10000;
    BenchmarkTimer timer;

    // Помещение в стек
    Stack<int> stack;
    timer.start();
    for (int i = 0; i < N; ++i) {
        stack.push(i);
    }
    double push_time = timer.stop();
    print_result("Push", push_time, N);

    // Доступ к вершине
    timer.start();
    volatile int sum = 0;
    for (int i = 0; i < 1000; ++i) {
        sum += stack.top();
    }
    double access_time = timer.stop();
    print_result("Top Access", access_time, 1000);

    // Снятие со стека
    timer.start();
    for (int i = 0; i < 1000 && stack.getSize() > 0; ++i) {
        stack.pop();
    }
    double pop_time = timer.stop();
    print_result("Pop", pop_time, 1000);
}

/**
 * @brief Тестирование производительности хеш-таблицы (HashTable).
 *
 * Проверяет вставку, поиск, доступ по ключу и удаление.
 * Использует try-catch для обработки отсутствующих ключей при случайном доступе.
 */
void benchmark_hash_table() {
    print_header("HASH TABLE");

    const int N = 10000;
    BenchmarkTimer timer;

    // Вставка пар ключ-значение
    HashTable<int, int> table;
    timer.start();
    for (int i = 0; i < N; ++i) {
        table.insert(i, i * 2);
    }
    double insert_time = timer.stop();
    print_result("Insert", insert_time, N);

    // Проверка наличия случайных ключей
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, N - 1);

    timer.start();
    int found_count = 0;
    for (int i = 0; i < N; ++i) {
        if (table.find(dis(gen))) {
            found_count++;
        }
    }
    double find_time = timer.stop();
    print_result("Find", find_time, N);

    // Доступ по ключу
    timer.start();
    volatile int sum = 0;
    for (int i = 0; i < 1000; ++i) {
        try {
            sum += table.get(dis(gen));
        } catch (...) {
        }
    }
    double access_time = timer.stop();
    print_result("Access", access_time, 1000);

    // Удаление первых N значений (часть может быть отсутствовать)
    timer.start();
    for (int i = 0; i < 1000; ++i) {
        try {
            table.remove(i);
        } catch (...) {
        }
    }
    double remove_time = timer.stop();
    print_result("Remove", remove_time, 1000);
}

/**
 * @brief Тестирование производительности полного бинарного дерева (FullBinaryTree).
 *
 * Проверяет вставку, поиск, проверку инварианта и удаление.
 * Размер выборки меньше (N=1000) из-за сложности операций с деревом.
 */
void benchmark_full_binary_tree() {
    print_header("FULL BINARY TREE");

    const int N = 1000; // Меньшее N для операций с деревом
    BenchmarkTimer timer;

    // Вставка с поддержанием полноты
    FullBinaryTree<int> tree;
    timer.start();
    for (int i = 0; i < N; ++i) {
        tree.insert(i);
    }
    double insert_time = timer.stop();
    print_result("Insert", insert_time, N);

    // Поиск случайных значений
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, N - 1);

    timer.start();
    int found_count = 0;
    for (int i = 0; i < N; ++i) {
        if (tree.find(dis(gen))) {
            found_count++;
        }
    }
    double find_time = timer.stop();
    print_result("Find", find_time, N);

    // Проверка инварианта
    timer.start();
    bool is_full = tree.isFullBinaryTree();
    double invariant_time = timer.stop();
    print_result("Invariant Check", invariant_time, 1);

    std::cout << "Tree is full binary tree: " << (is_full ? "YES" : "NO") << std::endl;
    std::cout << "Tree size: " << tree.getSize() << std::endl;
    
    // Дублирование в файл
    if (resultsFile.is_open()) {
        resultsFile << "Tree is full binary tree: " << (is_full ? "YES" : "NO") << std::endl;
        resultsFile << "Tree size: " << tree.getSize() << std::endl;
    }

    // Удаление первых 100 значений
    timer.start();
    for (int i = 0; i < 100 && tree.getSize() > 0; ++i) {
        tree.remove(i);
    }
    double remove_time = timer.stop();
    print_result("Remove", remove_time, 100);
}

/**
 * @brief Тестирование механизмов сериализации и десериализации.
 *
 * Проверяет сохранение и восстановление состояния для Array, HashTable и FullBinaryTree.
 */
void benchmark_serialization() {
    print_header("SERIALIZATION");

    BenchmarkTimer timer;

    // Сериализация/десериализация массива
    Array<int> arr;
    for (int i = 0; i < 1000; ++i) {
        arr.add(i);
    }

    timer.start();
    std::stringstream ss1;
    arr.serialize(ss1);
    double array_serialize_time = timer.stop();
    print_result("Array Serialize", array_serialize_time, 1);

    timer.start();
    Array<int> arr2;
    arr2.deserialize(ss1);
    double array_deserialize_time = timer.stop();
    print_result("Array Deserialize", array_deserialize_time, 1);

    // Сериализация/десериализация хэш-таблицы
    HashTable<int, int> table;
    for (int i = 0; i < 1000; ++i) {
        table.insert(i, i * 2);
    }

    timer.start();
    std::stringstream ss2;
    table.serialize(ss2);
    double table_serialize_time = timer.stop();
    print_result("HashTable Serialize", table_serialize_time, 1);

    timer.start();
    HashTable<int, int> table2;
    table2.deserialize(ss2);
    double table_deserialize_time = timer.stop();
    print_result("HashTable Deserialize", table_deserialize_time, 1);

    // Сериализация/десериализация полного бинарного дерева
    FullBinaryTree<int> tree;
    for (int i = 0; i < 100; ++i) {
        tree.insert(i);
    }

    timer.start();
    std::stringstream ss3;
    tree.serialize(ss3);
    double tree_serialize_time = timer.stop();
    print_result("Tree Serialize", tree_serialize_time, 1);

    timer.start();
    FullBinaryTree<int> tree2;
    tree2.deserialize(ss3);
    double tree_deserialize_time = timer.stop();
    print_result("Tree Deserialize", tree_deserialize_time, 1);
}

/**
 * @brief Выводит сводную таблицу сравнения структур данных.
 */
void print_comparison_summary() {
    std::cout << "\n=== PERFORMANCE COMPARISON SUMMARY ===" << std::endl;
    std::cout << "Data Structure    | Best Use Case" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    std::cout << "Array             | Random access, cache-friendly operations" << std::endl;
    std::cout << "ForwardList       | Frequent front insertions, memory efficiency" << std::endl;
    std::cout << "DoubleList        | Bidirectional traversal, front/back operations" << std::endl;
    std::cout << "Queue             | FIFO operations, producer-consumer patterns" << std::endl;
    std::cout << "Stack             | LIFO operations, recursion simulation" << std::endl;
    std::cout << "HashTable         | Fast key-value lookups, O(1) average access" << std::endl;
    std::cout << "FullBinaryTree    | Hierarchical data with full binary constraint" << std::endl;

    // Дублирование в файл
    if (resultsFile.is_open()) {
        resultsFile << "\n=== PERFORMANCE COMPARISON SUMMARY ===" << std::endl;
        resultsFile << "Data Structure    | Best Use Case" << std::endl;
        resultsFile << std::string(50, '-') << std::endl;
        resultsFile << "Array             | Random access, cache-friendly operations" << std::endl;
        resultsFile << "ForwardList       | Frequent front insertions, memory efficiency" << std::endl;
        resultsFile << "DoubleList        | Bidirectional traversal, front/back operations" << std::endl;
        resultsFile << "Queue             | FIFO operations, producer-consumer patterns" << std::endl;
        resultsFile << "Stack             | LIFO operations, recursion simulation" << std::endl;
        resultsFile << "HashTable         | Fast key-value lookups, O(1) average access" << std::endl;
        resultsFile << "FullBinaryTree    | Hierarchical data with full binary constraint" << std::endl;
    }
}

/**
 * @brief Точка входа в программу.
 *
 * Запускает последовательность бенчмарков и выводит результаты в консоль и файл.
 * @return Код возврата (0 при успехе).
 */
int main() {
    std::cout << "Starting comprehensive performance benchmarks..." << std::endl;
    std::cout << "Note: Times may vary based on system performance" << std::endl;
    
    if (resultsFile.is_open()) {
        resultsFile << "Starting comprehensive performance benchmarks..." << std::endl;
        resultsFile << "Note: Times may vary based on system performance" << std::endl;
    } else {
        std::cerr << "Warning: Could not open benchmark_results.txt for writing." << std::endl;
    }

    benchmark_array();
    benchmark_forward_list();
    benchmark_double_list();
    benchmark_queue();
    benchmark_stack();
    benchmark_hash_table();
    benchmark_full_binary_tree();
    benchmark_serialization();

    print_comparison_summary();

    std::cout << "\nBenchmark completed successfully!" << std::endl;
    if (resultsFile.is_open()) {
        resultsFile << "\nBenchmark completed successfully!" << std::endl;
    }

    return 0;
}