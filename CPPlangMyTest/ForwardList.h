#pragma once
#include <iostream>
#include <stdexcept>

/**
 * @brief Класс односвязного списка.
 * 
 * Реализует последовательный контейнер с эффективной вставкой/удалением в начале (O(1)).
 * Доступ к произвольным элементам и вставка в конец выполняются за O(N).
 * 
 * @tparam T Тип элементов списка. Должен быть копируемым и конструируемым по умолчанию.
 * @warning Бинарная сериализация корректна только для тривиально копируемых типов (POD).
 */
template<typename T>
class ForwardList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;  ///< Указатель на начало списка
    size_t size; ///< Текущее количество элементов

public:
    /**
     * @brief Конструктор по умолчанию.
     * Создает пустой список.
     */
    ForwardList();

    /**
     * @brief Копирующий конструктор.
     * Создает глубокую копию списка.
     * Оптимизирован для работы за O(N).
     * 
     * @param other Список-источник.
     */
    ForwardList(const ForwardList& other);

    /**
     * @brief Оператор присваивания.
     * Выполняет глубокое копирование с гарантией безопасности исключений (strong exception guarantee).
     * 
     * @param other Список-источник.
     * @return Ссылка на текущий объект.
     */
    ForwardList& operator=(const ForwardList& other);

    /**
     * @brief Деструктор.
     * Освобождает память всех узлов.
     */
    ~ForwardList();

    /**
     * @brief Добавляет элемент в начало списка.
     * Сложность: O(1).
     * 
     * @param element Добавляемое значение.
     */
    void pushFront(const T& element);

    /**
     * @brief Добавляет элемент в конец списка.
     * Сложность: O(N), так как требуется проход до последнего элемента.
     * 
     * @param element Добавляемое значение.
     */
    void pushBack(const T& element);

    /**
     * @brief Вставляет элемент по указанному индексу.
     * Сложность: O(N).
     * 
     * @param index Позиция вставки.
     * @param element Добавляемое значение.
     * @throw std::out_of_range Если index > size.
     */
    void insert(size_t index, const T& element);

    /**
     * @brief Удаляет первый элемент списка.
     * Сложность: O(1).
     * 
     * @throw std::runtime_error Если список пуст.
     */
    void popFront();

    /**
     * @brief Удаляет элемент по индексу.
     * Сложность: O(N).
     * 
     * @param index Индекс удаляемого элемента.
     * @throw std::out_of_range Если index >= size.
     */
    void remove(size_t index);

    /**
     * @brief Удаляет все вхождения заданного значения.
     * Проходит по всему списку.
     * 
     * @param value Значение для удаления.
     */
    void removeValue(const T& value);

    /**
     * @brief Возвращает ссылку на элемент по индексу.
     * Линейный поиск от головы списка.
     * 
     * @param index Индекс элемента.
     * @return T& Ссылка на элемент.
     * @throw std::out_of_range Если index >= size.
     */
    T& get(size_t index);

    /**
     * @brief Возвращает константную ссылку на элемент по индексу.
     * 
     * @param index Индекс элемента.
     * @return const T& Константная ссылка на элемент.
     * @throw std::out_of_range Если index >= size.
     */
    const T& get(size_t index) const;

    /**
     * @brief Возвращает первый элемент.
     * @return T& Ссылка на первый элемент.
     * @throw std::runtime_error Если список пуст.
     */
    T& front();

    /**
     * @brief Возвращает первый элемент (const).
     * @return const T& Константная ссылка на первый элемент.
     * @throw std::runtime_error Если список пуст.
     */
    const T& front() const;

    /**
     * @brief Возвращает текущее количество элементов.
     * @return size_t Размер списка.
     */
    size_t getSize() const;

    /**
     * @brief Проверяет, пуст ли список.
     * @return true Если список пуст.
     * @return false Если в списке есть элементы.
     */
    bool isEmpty() const;

    /**
     * @brief Полностью очищает список.
     */
    void clear();

    /**
     * @brief Проверяет наличие значения в списке.
     * 
     * @param value Искомое значение.
     * @return true Если значение найдено.
     */
    bool find(const T& value) const;

    /**
     * @brief Выводит элементы списка в поток вывода.
     * Формат: [e1 -> e2 -> e3]
     */
    void print() const;

    /**
     * @brief Сериализация (обертка).
     * По умолчанию вызывает serializeBinary.
     * @param out Поток вывода.
     */
    void serialize(std::ostream& out) const;

    /**
     * @brief Десериализация (обертка).
     * По умолчанию вызывает deserializeBinary.
     * @param in Поток ввода.
     */
    void deserialize(std::istream& in);

    /**
     * @brief Бинарная сериализация.
     * Сохраняет размер и данные узлов.
     * 
     * @warning Только для POD-типов.
     * @param out Поток вывода.
     */
    void serializeBinary(std::ostream& out) const;

    /**
     * @brief Бинарная десериализация.
     * Очищает список и восстанавливает элементы.
     * Оптимизирована для работы за O(N).
     * 
     * @param in Поток ввода.
     */
    void deserializeBinary(std::istream& in);

    /**
     * @brief Текстовая сериализация.
     * Формат: <размер>\n<значения через пробел>\n
     * 
     * @param out Поток вывода.
     */
    void serializeText(std::ostream& out) const;

    /**
     * @brief Текстовая десериализация.
     * Оптимизирована для работы за O(N).
     * 
     * @param in Поток ввода.
     */
    void deserializeText(std::istream& in);
};

template<typename T>
ForwardList<T>::ForwardList() : head(nullptr), size(0) {}

template<typename T>
ForwardList<T>::ForwardList(const ForwardList& other) : head(nullptr), size(0) {
    if (!other.head) return;

    // Оптимизированное копирование O(N) без pushBack
    head = new Node(other.head->data);
    Node* current = head;
    Node* otherCurrent = other.head->next;
    
    while (otherCurrent) {
        current->next = new Node(otherCurrent->data);
        current = current->next;
        otherCurrent = otherCurrent->next;
    }
    size = other.size;
}

template<typename T>
ForwardList<T>& ForwardList<T>::operator=(const ForwardList& other) {
    if (this != &other) {
        // Идиома copy-and-swap
        ForwardList temp(other);
        
        Node* tHead = head; head = temp.head; temp.head = tHead;
        size_t tSize = size; size = temp.size; temp.size = tSize;
    }
    return *this;
}

template<typename T>
ForwardList<T>::~ForwardList() {
    clear();
}

template<typename T>
void ForwardList<T>::pushFront(const T& element) {
    Node* newNode = new Node(element);
    newNode->next = head;
    head = newNode;
    ++size;
}

template<typename T>
void ForwardList<T>::pushBack(const T& element) {
    Node* newNode = new Node(element);
    if (!head) {
        head = newNode;
    } else {
        Node* current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
    ++size;
}

template<typename T>
void ForwardList<T>::insert(size_t index, const T& element) {
    if (index > size) {
        throw std::out_of_range("Index out of range");
    }
    if (index == 0) {
        pushFront(element);
        return;
    }

    Node* newNode = new Node(element);
    Node* current = head;
    for (size_t i = 0; i < index - 1; ++i) {
        current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
    ++size;
}

template<typename T>
void ForwardList<T>::popFront() {
    if (!head) {
        throw std::runtime_error("List is empty");
    }
    Node* temp = head;
    head = head->next;
    delete temp;
    --size;
}

template<typename T>
void ForwardList<T>::remove(size_t index) {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    if (index == 0) {
        popFront();
        return;
    }

    Node* current = head;
    for (size_t i = 0; i < index - 1; ++i) {
        current = current->next;
    }
    Node* temp = current->next;
    current->next = temp->next;
    delete temp;
    --size;
}

template<typename T>
void ForwardList<T>::removeValue(const T& value) {
    while (head && head->data == value) {
        popFront();
    }

    if (!head) return;

    Node* current = head;
    while (current->next) {
        if (current->next->data == value) {
            Node* temp = current->next;
            current->next = temp->next;
            delete temp;
            --size;
        } else {
            current = current->next;
        }
    }
}

template<typename T>
T& ForwardList<T>::get(size_t index) {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    Node* current = head;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

template<typename T>
const T& ForwardList<T>::get(size_t index) const {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    Node* current = head;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

template<typename T>
T& ForwardList<T>::front() {
    if (!head) {
        throw std::runtime_error("List is empty");
    }
    return head->data;
}

template<typename T>
const T& ForwardList<T>::front() const {
    if (!head) {
        throw std::runtime_error("List is empty");
    }
    return head->data;
}

template<typename T>
size_t ForwardList<T>::getSize() const {
    return size;
}

template<typename T>
bool ForwardList<T>::isEmpty() const {
    return size == 0;
}

template<typename T>
void ForwardList<T>::clear() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    head = nullptr;
    size = 0;
}

template<typename T>
bool ForwardList<T>::find(const T& value) const {
    Node* current = head;
    while (current) {
        if (current->data == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

template<typename T>
void ForwardList<T>::print() const {
    std::cout << "[";
    Node* current = head;
    while (current) {
        std::cout << current->data;
        if (current->next) std::cout << " -> ";
        current = current->next;
    }
    std::cout << "]" << std::endl;
}

template<typename T>
void ForwardList<T>::serialize(std::ostream& out) const {
    serializeBinary(out);
}

template<typename T>
void ForwardList<T>::deserialize(std::istream& in) {
    deserializeBinary(in);
}

// Важно: бинарная сериализация корректна только для тривиально копируемых типов
template<typename T>
void ForwardList<T>::serializeBinary(std::ostream& out) const {
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    Node* current = head;
    while (current) {
        out.write(reinterpret_cast<const char*>(&current->data), sizeof(T));
        current = current->next;
    }
}

// Важно: бинарная десериализация корректна только для тривиально копируемых типов
template<typename T>
void ForwardList<T>::deserializeBinary(std::istream& in) {
    clear();
    size_t new_size;
    in.read(reinterpret_cast<char*>(&new_size), sizeof(new_size));
    if (new_size == 0) return;

    // Читаем первый элемент
    T value;
    in.read(reinterpret_cast<char*>(&value), sizeof(T));
    head = new Node(value);
    size = 1;

    // Читаем остальные, поддерживая указатель на хвост
    Node* current = head;
    for (size_t i = 1; i < new_size; ++i) {
        in.read(reinterpret_cast<char*>(&value), sizeof(T));
        current->next = new Node(value);
        current = current->next;
        size++;
    }
}

template<typename T>
void ForwardList<T>::serializeText(std::ostream& out) const {
    out << size << std::endl;
    Node* current = head;
    while (current) {
        out << current->data;
        if (current->next) out << " ";
        current = current->next;
    }
    out << std::endl;
}

template<typename T>
void ForwardList<T>::deserializeText(std::istream& in) {
    clear();
    size_t new_size;
    in >> new_size;
    if (new_size == 0) return;

    // Читаем первый элемент
    T value;
    in >> value;
    head = new Node(value);
    size = 1;

    // Читаем остальные
    Node* current = head;
    for (size_t i = 1; i < new_size; ++i) {
        in >> value;
        current->next = new Node(value);
        current = current->next;
        size++;
    }
}