#pragma once
#include <iostream>
#include <stdexcept>
#include <string>  // Явно включено для поддержки std::string
#include <utility> // Для std::swap

/**
 * @brief Шаблонный класс Очереди (Queue).
 *
 * Реализует структуру данных FIFO (First In, First Out) на основе односвязного списка.
 * Элементы добавляются в "хвост" (back) и извлекаются из "головы" (front).
 *
 * @tparam T Тип хранимых данных.
 */
template<typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* front_node; ///< Указатель на начало очереди (отсюда забираем)
    Node* back_node;  ///< Указатель на конец очереди (сюда добавляем)
    size_t size;

public:
    /**
     * @brief Конструктор по умолчанию. Создает пустую очередь.
     */
    Queue();

    /**
     * @brief Конструктор копирования.
     * Создает глубокую копию очереди, сохраняя порядок элементов.
     * @param other Очередь для копирования.
     */
    Queue(const Queue& other);

    /**
     * @brief Оператор присваивания.
     * Обеспечивает строгую гарантию безопасности исключений.
     * @param other Очередь, содержимое которой присваивается.
     * @return Ссылка на текущий объект.
     */
    Queue& operator=(const Queue& other);

    /**
     * @brief Деструктор. Освобождает память всех узлов.
     */
    ~Queue();

    /**
     * @brief Добавляет элемент в конец очереди (enqueue).
     * @param element Значение для вставки.
     */
    void enqueue(const T& element);

    /**
     * @brief Удаляет элемент из начала очереди (dequeue).
     * @throw std::runtime_error Если очередь пуста.
     */
    void dequeue();

    /**
     * @brief Возвращает ссылку на первый элемент очереди.
     * @return Ссылка на элемент в голове (front).
     * @throw std::runtime_error Если очередь пуста.
     */
    T& front();

    /**
     * @brief Возвращает константную ссылку на первый элемент очереди.
     * @return Константная ссылка на элемент в голове.
     * @throw std::runtime_error Если очередь пуста.
     */
    const T& front() const;

    /**
     * @brief Возвращает ссылку на последний элемент очереди.
     * @return Ссылка на элемент в хвосте (back).
     * @throw std::runtime_error Если очередь пуста.
     */
    T& back();

    /**
     * @brief Возвращает константную ссылку на последний элемент очереди.
     * @return Константная ссылка на элемент в хвосте.
     * @throw std::runtime_error Если очередь пуста.
     */
    const T& back() const;

    /**
     * @brief Возвращает текущий размер очереди.
     * @return Количество элементов.
     */
    size_t getSize() const;

    /**
     * @brief Проверяет, пуста ли очередь.
     * @return true, если элементов нет, иначе false.
     */
    bool isEmpty() const;

    /**
     * @brief Полностью очищает очередь.
     * Удаляет все узлы и сбрасывает указатели.
     */
    void clear();

    /**
     * @brief Выводит содержимое очереди в консоль от начала к концу.
     */
    void print() const;

    /**
     * @brief Универсальная сериализация.
     * По умолчанию вызывает бинарную сериализацию.
     * @param out Поток вывода.
     */
    void serialize(std::ostream& out) const;

    /**
     * @brief Универсальная десериализация.
     * По умолчанию вызывает бинарную десериализацию.
     * @param in Поток ввода.
     */
    void deserialize(std::istream& in);

    /**
     * @brief Бинарная сериализация.
     * Сохраняет данные в бинарном виде для максимальной производительности.
     * @note Корректно работает только с тривиально копируемыми типами (POD).
     * @param out Поток вывода.
     */
    void serializeBinary(std::ostream& out) const;

    /**
     * @brief Бинарная десериализация.
     * Восстанавливает очередь из бинарного формата.
     * @param in Поток ввода.
     */
    void deserializeBinary(std::istream& in);

    /**
     * @brief Текстовая сериализация.
     * Сохраняет элементы через пробел в читаемом виде.
     * @param out Поток вывода.
     */
    void serializeText(std::ostream& out) const;

    /**
     * @brief Текстовая десериализация.
     * Считывает элементы из текстового потока.
     * @param in Поток ввода.
     */
    void deserializeText(std::istream& in);
};

template<typename T>
Queue<T>::Queue() : front_node(nullptr), back_node(nullptr), size(0) {}

template<typename T>
Queue<T>::Queue(const Queue& other) : front_node(nullptr), back_node(nullptr), size(0) {
    Node* current = other.front_node;
    while (current) {
        enqueue(current->data);
        current = current->next;
    }
}

template<typename T>
Queue<T>& Queue<T>::operator=(const Queue& other) {
    if (this != &other) {
        // Использование идиомы Copy-and-Swap.
        // 1. Создаем временную копию. Если вылетит исключение, 'this' не пострадает.
        Queue<T> temp(other);
        
        // 2. Меняем внутреннее состояние текущего объекта и временного.
        std::swap(front_node, temp.front_node);
        std::swap(back_node, temp.back_node);
        std::swap(size, temp.size);
        
        // 3. temp удалится автоматически вместе со старыми данными 'this'.
    }
    return *this;
}

template<typename T>
Queue<T>::~Queue() {
    clear();
}

template<typename T>
void Queue<T>::enqueue(const T& element) {
    Node* newNode = new Node(element);
    if (!back_node) {
        front_node = back_node = newNode;
    } else {
        back_node->next = newNode;
        back_node = newNode;
    }
    ++size;
}

template<typename T>
void Queue<T>::dequeue() {
    if (!front_node) {
        throw std::runtime_error("Queue is empty");
    }
    Node* temp = front_node;
    front_node = front_node->next;
    if (!front_node) {
        back_node = nullptr;
    }
    delete temp;
    --size;
}

template<typename T>
T& Queue<T>::front() {
    if (!front_node) {
        throw std::runtime_error("Queue is empty");
    }
    return front_node->data;
}

template<typename T>
const T& Queue<T>::front() const {
    if (!front_node) {
        throw std::runtime_error("Queue is empty");
    }
    return front_node->data;
}

template<typename T>
T& Queue<T>::back() {
    if (!back_node) {
        throw std::runtime_error("Queue is empty");
    }
    return back_node->data;
}

template<typename T>
const T& Queue<T>::back() const {
    if (!back_node) {
        throw std::runtime_error("Queue is empty");
    }
    return back_node->data;
}

template<typename T>
size_t Queue<T>::getSize() const {
    return size;
}

template<typename T>
bool Queue<T>::isEmpty() const {
    return size == 0;
}

template<typename T>
void Queue<T>::clear() {
    while (front_node) {
        Node* temp = front_node;
        front_node = front_node->next;
        delete temp;
    }
    back_node = nullptr;
    size = 0;
}

template<typename T>
void Queue<T>::print() const {
    std::cout << "Front -> [";
    Node* current = front_node;
    while (current) {
        std::cout << current->data;
        if (current->next) std::cout << ", ";
        current = current->next;
    }
    std::cout << "] <- Back" << std::endl;
}

template<typename T>
void Queue<T>::serialize(std::ostream& out) const {
    serializeBinary(out);
}

template<typename T>
void Queue<T>::deserialize(std::istream& in) {
    deserializeBinary(in);
}

// Важно: бинарная сериализация корректна только для тривиально копируемых типов
template<typename T>
void Queue<T>::serializeBinary(std::ostream& out) const {
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    Node* current = front_node;
    while (current) {
        out.write(reinterpret_cast<const char*>(&current->data), sizeof(T));
        current = current->next;
    }
}

// Важно: бинарная десериализация корректна только для тривиально копируемых типов
template<typename T>
void Queue<T>::deserializeBinary(std::istream& in) {
    clear();
    size_t new_size;
    in.read(reinterpret_cast<char*>(&new_size), sizeof(new_size));
    for (size_t i = 0; i < new_size; ++i) {
        T value;
        in.read(reinterpret_cast<char*>(&value), sizeof(T));
        enqueue(value);
    }
}

template<typename T>
void Queue<T>::serializeText(std::ostream& out) const {
    out << size << std::endl;
    Node* current = front_node;
    while (current) {
        out << current->data;
        if (current->next) out << " ";
        current = current->next;
    }
    out << std::endl;
}

template<typename T>
void Queue<T>::deserializeText(std::istream& in) {
    clear();
    size_t new_size;
    in >> new_size;
    for (size_t i = 0; i < new_size; ++i) {
        T value;
        in >> value;
        enqueue(value);
    }
}