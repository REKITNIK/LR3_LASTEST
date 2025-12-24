#pragma once
#include <iostream>
#include <stdexcept>
#include <string>  // Явно включено для поддержки std::string
#include <utility> // Для std::swap

/**
 * @brief Шаблонный класс Стека (Stack).
 *
 * Реализует структуру данных LIFO (Last In, First Out) на основе односвязного списка.
 * Элементы добавляются на "вершину" и извлекаются с "вершины".
 *
 * @tparam T Тип хранимых данных. Должен иметь конструктор по умолчанию (для внутренних операций копирования).
 */
template<typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* top_node; ///< Указатель на верхний элемент стека
    size_t size;

public:
    /**
     * @brief Конструктор по умолчанию. Создает пустой стек.
     */
    Stack();

    /**
     * @brief Конструктор копирования.
     * Создает глубокую копию стека, сохраняя исходный порядок элементов.
     * @param other Стек для копирования.
     */
    Stack(const Stack& other);

    /**
     * @brief Оператор присваивания.
     * Обеспечивает строгую гарантию безопасности исключений.
     * @param other Стек, содержимое которого присваивается.
     * @return Ссылка на текущий объект.
     */
    Stack& operator=(const Stack& other);

    /**
     * @brief Деструктор. Освобождает память.
     */
    ~Stack();

    /**
     * @brief Помещает элемент на вершину стека (push).
     * @param element Значение для вставки.
     */
    void push(const T& element);

    /**
     * @brief Удаляет элемент с вершины стека (pop).
     * @throw std::runtime_error Если стек пуст.
     */
    void pop();

    /**
     * @brief Возвращает ссылку на элемент на вершине стека.
     * @return Ссылка на верхний элемент.
     * @throw std::runtime_error Если стек пуст.
     */
    T& top();

    /**
     * @brief Возвращает константную ссылку на элемент на вершине стека.
     * @return Константная ссылка на верхний элемент.
     * @throw std::runtime_error Если стек пуст.
     */
    const T& top() const;

    /**
     * @brief Возвращает текущее количество элементов.
     * @return Размер стека.
     */
    size_t getSize() const;

    /**
     * @brief Проверяет, пуст ли стек.
     * @return true, если элементов нет, иначе false.
     */
    bool isEmpty() const;

    /**
     * @brief Полностью очищает стек.
     * Удаляет все узлы.
     */
    void clear();

    /**
     * @brief Выводит содержимое стека в консоль (от вершины к дну).
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
     * Сохраняет элементы в порядке, позволяющем восстановить стек (от дна к вершине).
     * @note Корректно работает только с тривиально копируемыми типами (POD).
     * @param out Поток вывода.
     */
    void serializeBinary(std::ostream& out) const;

    /**
     * @brief Бинарная десериализация.
     * Восстанавливает стек из бинарного формата.
     * @param in Поток ввода.
     */
    void deserializeBinary(std::istream& in);

    /**
     * @brief Текстовая сериализация.
     * Сохраняет элементы через пробел.
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
Stack<T>::Stack() : top_node(nullptr), size(0) {}

template<typename T>
Stack<T>::Stack(const Stack& other) : top_node(nullptr), size(0) {
    if (other.top_node) {
        // Создаем временный массив для инвертирования порядка элементов.
        // Это необходимо, так как push добавляет в начало списка.
        // Чтобы копия соответствовала оригиналу (Top -> Bottom), нужно вставлять элементы, начиная с самого нижнего.
        T* temp = new T[other.size];
        Node* current = other.top_node;
        size_t index = other.size;
        while (current) {
            temp[--index] = current->data;
            current = current->next;
        }

        // Вставляем элементы в правильном порядке
        for (size_t i = 0; i < other.size; ++i) {
            push(temp[i]);
        }
        delete[] temp;
    }
}

template<typename T>
Stack<T>& Stack<T>::operator=(const Stack& other) {
    if (this != &other) {
        // Идиома Copy-and-Swap.
        // 1. Создаем копию через конструктор копирования.
        // Если здесь произойдет исключение (например, нехватка памяти при new T[]),
        // текущий объект останется нетронутым.
        Stack<T> temp(other);

        // 2. Обмениваем ресурсы.
        std::swap(top_node, temp.top_node);
        std::swap(size, temp.size);
        
        // 3. Деструктор temp очистит старую память.
    }
    return *this;
}

template<typename T>
Stack<T>::~Stack() {
    clear();
}

template<typename T>
void Stack<T>::push(const T& element) {
    Node* newNode = new Node(element);
    newNode->next = top_node;
    top_node = newNode;
    ++size;
}

template<typename T>
void Stack<T>::pop() {
    if (!top_node) {
        throw std::runtime_error("Stack is empty");
    }
    Node* temp = top_node;
    top_node = top_node->next;
    delete temp;
    --size;
}

template<typename T>
T& Stack<T>::top() {
    if (!top_node) {
        throw std::runtime_error("Stack is empty");
    }
    return top_node->data;
}

template<typename T>
const T& Stack<T>::top() const {
    if (!top_node) {
        throw std::runtime_error("Stack is empty");
    }
    return top_node->data;
}

template<typename T>
size_t Stack<T>::getSize() const {
    return size;
}

template<typename T>
bool Stack<T>::isEmpty() const {
    return size == 0;
}

template<typename T>
void Stack<T>::clear() {
    while (top_node) {
        Node* temp = top_node;
        top_node = top_node->next;
        delete temp;
    }
    size = 0;
}

template<typename T>
void Stack<T>::print() const {
    std::cout << "Top -> [";
    Node* current = top_node;
    while (current) {
        std::cout << current->data;
        if (current->next) std::cout << ", ";
        current = current->next;
    }
    std::cout << "] <- Bottom" << std::endl;
}

template<typename T>
void Stack<T>::serialize(std::ostream& out) const {
    serializeBinary(out);
}

template<typename T>
void Stack<T>::deserialize(std::istream& in) {
    deserializeBinary(in);
}

template<typename T>
void Stack<T>::serializeBinary(std::ostream& out) const {
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));

    // Сохраняем элементы в обратном порядке (от дна к вершине), 
    // чтобы при чтении (deserialize) последовательные вызовы push восстановили стек корректно.
    if (size > 0) {
        T* temp = new T[size];
        Node* current = top_node;
        size_t index = size;
        while (current) {
            temp[--index] = current->data;
            current = current->next;
        }

        for (size_t i = 0; i < size; ++i) {
            out.write(reinterpret_cast<const char*>(&temp[i]), sizeof(T));
        }
        delete[] temp;
    }
}

template<typename T>
void Stack<T>::deserializeBinary(std::istream& in) {
    clear();
    size_t new_size;
    in.read(reinterpret_cast<char*>(&new_size), sizeof(new_size));
    for (size_t i = 0; i < new_size; ++i) {
        T value;
        in.read(reinterpret_cast<char*>(&value), sizeof(T));
        push(value);
    }
}

template<typename T>
void Stack<T>::serializeText(std::ostream& out) const {
    out << size << std::endl;
    
    // Сохраняем элементы в обратном порядке для сохранения структуры стека при десериализации
    if (size > 0) {
        T* temp = new T[size];
        Node* current = top_node;
        size_t index = size;
        while (current) {
            temp[--index] = current->data;
            current = current->next;
        }

        for (size_t i = 0; i < size; ++i) {
            out << temp[i];
            if (i < size - 1) out << " ";
        }
        delete[] temp;
    }
    out << std::endl;
}

template<typename T>
void Stack<T>::deserializeText(std::istream& in) {
    clear();
    size_t new_size;
    in >> new_size;
    for (size_t i = 0; i < new_size; ++i) {
        T value;
        in >> value;
        push(value);
    }
}