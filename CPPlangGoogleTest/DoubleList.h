#pragma once
#include <iostream>
#include <stdexcept>

/**
 * @brief Класс двусвязного списка.
 * 
 * Поддерживает вставку и удаление элементов с обоих концов за O(1),
 * а также доступ и модификацию по индексу за O(N).
 * 
 * @tparam T Тип элементов списка. Должен быть копируемым и конструируемым по умолчанию.
 * @warning Бинарная сериализация корректна только для тривиально копируемых типов (POD).
 */
template<typename T>
class DoubleList {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
    };

    Node* head; ///< Указатель на первый элемент
    Node* tail; ///< Указатель на последний элемент
    size_t size; ///< Текущее количество элементов

public:
    /**
     * @brief Конструктор по умолчанию.
     * Создает пустой список.
     */
    DoubleList();

    /**
     * @brief Копирующий конструктор.
     * Создает глубокую копию списка.
     * 
     * @param other Список-источник.
     */
    DoubleList(const DoubleList& other);

    /**
     * @brief Оператор присваивания.
     * Выполняет глубокое копирование с гарантией безопасности исключений (strong exception guarantee).
     * 
     * @param other Список-источник.
     * @return Ссылка на текущий объект.
     */
    DoubleList& operator=(const DoubleList& other);

    /**
     * @brief Деструктор.
     * Освобождает всю память, занятую узлами списка.
     */
    ~DoubleList();

    /**
     * @brief Добавляет элемент в начало списка.
     * Сложность: O(1).
     * 
     * @param element Добавляемое значение.
     */
    void pushFront(const T& element);

    /**
     * @brief Добавляет элемент в конец списка.
     * Сложность: O(1).
     * 
     * @param element Добавляемое значение.
     */
    void pushBack(const T& element);

    /**
     * @brief Вставляет элемент по указанному индексу.
     * Оптимизирован: выбирает направление перебора (от начала или от конца) в зависимости от индекса.
     * 
     * @param index Позиция вставки (0 - перед первым, size - после последнего).
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
     * @brief Удаляет последний элемент списка.
     * Сложность: O(1).
     * 
     * @throw std::runtime_error Если список пуст.
     */
    void popBack();

    /**
     * @brief Удаляет элемент по индексу.
     * Оптимизирован: выбирает направление перебора (от начала или от конца).
     * 
     * @param index Индекс удаляемого элемента.
     * @throw std::out_of_range Если index >= size.
     */
    void remove(size_t index);

    /**
     * @brief Удаляет все элементы, равные заданному значению.
     * Проходит по всему списку.
     * 
     * @param value Значение для удаления.
     */
    void removeValue(const T& value);

    /**
     * @brief Возвращает ссылку на элемент по индексу.
     * Оптимизирован: выбирает направление перебора (от начала или от конца).
     * 
     * @param index Индекс элемента.
     * @return T& Ссылка на элемент.
     * @throw std::out_of_range Если index >= size.
     */
    T& get(size_t index);

    /**
     * @brief Возвращает константную ссылку на элемент по индексу.
     * Оптимизирован: выбирает направление перебора (от начала или от конца).
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
     * @brief Возвращает последний элемент.
     * @return T& Ссылка на последний элемент.
     * @throw std::runtime_error Если список пуст.
     */
    T& back();

    /**
     * @brief Возвращает последний элемент (const).
     * @return const T& Константная ссылка на последний элемент.
     * @throw std::runtime_error Если список пуст.
     */
    const T& back() const;

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
     * @brief Полностью очищает список, удаляя все узлы.
     */
    void clear();

    /**
     * @brief Проверяет наличие значения в списке.
     * 
     * @param value Искомое значение.
     * @return true Если значение найдено.
     * @return false Если значение не найдено.
     */
    bool find(const T& value) const;

    /**
     * @brief Выводит элементы списка в поток вывода (от головы к хвосту).
     * Формат: [e1 <-> e2 <-> e3]
     */
    void print() const;

    /**
     * @brief Выводит элементы списка в поток вывода в обратном порядке (от хвоста к голове).
     * Формат: [e3 <-> e2 <-> e1]
     */
    void printReverse() const;

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
     * Сохраняет размер и данные узлов. Структура связей не сохраняется, пересоздается при чтении.
     * 
     * @warning Только для POD-типов.
     * @param out Поток вывода.
     */
    void serializeBinary(std::ostream& out) const;

    /**
     * @brief Бинарная десериализация.
     * Очищает список и восстанавливает элементы.
     * 
     * @warning Только для POD-типов. Требует конструктора по умолчанию для T.
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
     * 
     * @param in Поток ввода.
     */
    void deserializeText(std::istream& in);
};

template<typename T>
DoubleList<T>::DoubleList() : head(nullptr), tail(nullptr), size(0) {}

template<typename T>
DoubleList<T>::DoubleList(const DoubleList& other) : head(nullptr), tail(nullptr), size(0) {
    Node* current = other.head;
    while (current) {
        pushBack(current->data);
        current = current->next;
    }
}

template<typename T>
DoubleList<T>& DoubleList<T>::operator=(const DoubleList& other) {
    if (this != &other) {
        // Создаем копию во временном объекте (Copy-and-Swap idiom)
        // Если здесь вылетит исключение, текущий объект (this) не пострадает
        DoubleList temp(other);
        
        // Меняем местами указатели и размер текущего объекта и временного
        Node* tHead = head; head = temp.head; temp.head = tHead;
        Node* tTail = tail; tail = temp.tail; temp.tail = tTail;
        size_t tSize = size; size = temp.size; temp.size = tSize;
        
        // Деструктор temp очистит старые данные this
    }
    return *this;
}

template<typename T>
DoubleList<T>::~DoubleList() {
    clear();
}

template<typename T>
void DoubleList<T>::pushFront(const T& element) {
    Node* newNode = new Node(element);
    if (!head) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    ++size;
}

template<typename T>
void DoubleList<T>::pushBack(const T& element) {
    Node* newNode = new Node(element);
    if (!tail) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    ++size;
}

template<typename T>
void DoubleList<T>::insert(size_t index, const T& element) {
    if (index > size) {
        throw std::out_of_range("Index out of range");
    }
    if (index == 0) {
        pushFront(element);
        return;
    }
    if (index == size) {
        pushBack(element);
        return;
    }

    Node* newNode = new Node(element);
    Node* current;

    if (index <= size / 2) {
        current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
    } else {
        current = tail;
        // Идем с конца: нам нужен элемент, который СЕЙЧАС стоит на позиции index.
        // tail имеет индекс size-1. Чтобы попасть на index, нужно сделать (size - 1 - index) шагов назад.
        for (size_t i = 0; i < size - 1 - index; ++i) {
            current = current->prev;
        }
    }

    newNode->next = current;
    newNode->prev = current->prev;
    current->prev->next = newNode;
    current->prev = newNode;
    ++size;
}

template<typename T>
void DoubleList<T>::popFront() {
    if (!head) {
        throw std::runtime_error("List is empty");
    }
    Node* temp = head;
    if (head == tail) {
        head = tail = nullptr;
    } else {
        head = head->next;
        head->prev = nullptr;
    }
    delete temp;
    --size;
}

template<typename T>
void DoubleList<T>::popBack() {
    if (!tail) {
        throw std::runtime_error("List is empty");
    }
    Node* temp = tail;
    if (head == tail) {
        head = tail = nullptr;
    } else {
        tail = tail->prev;
        tail->next = nullptr;
    }
    delete temp;
    --size;
}

template<typename T>
void DoubleList<T>::remove(size_t index) {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    if (index == 0) {
        popFront();
        return;
    }
    if (index == size - 1) {
        popBack();
        return;
    }

    Node* current;
    if (index <= size / 2) {
        current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
    } else {
        current = tail;
        for (size_t i = 0; i < size - 1 - index; ++i) {
            current = current->prev;
        }
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;
    delete current;
    --size;
}

template<typename T>
void DoubleList<T>::removeValue(const T& value) {
    Node* current = head;
    while (current) {
        if (current->data == value) {
            Node* temp = current;
            current = current->next;

            if (temp == head && temp == tail) {
                head = tail = nullptr;
            } else if (temp == head) {
                head = head->next;
                head->prev = nullptr;
            } else if (temp == tail) {
                tail = tail->prev;
                tail->next = nullptr;
            } else {
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
            }
            delete temp;
            --size;
        } else {
            current = current->next;
        }
    }
}

template<typename T>
T& DoubleList<T>::get(size_t index) {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    Node* current;
    if (index <= size / 2) {
        current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
    } else {
        current = tail;
        for (size_t i = 0; i < size - 1 - index; ++i) {
            current = current->prev;
        }
    }
    return current->data;
}

template<typename T>
const T& DoubleList<T>::get(size_t index) const {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    Node* current;
    if (index <= size / 2) {
        current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
    } else {
        current = tail;
        for (size_t i = 0; i < size - 1 - index; ++i) {
            current = current->prev;
        }
    }
    return current->data;
}

template<typename T>
T& DoubleList<T>::front() {
    if (!head) {
        throw std::runtime_error("List is empty");
    }
    return head->data;
}

template<typename T>
const T& DoubleList<T>::front() const {
    if (!head) {
        throw std::runtime_error("List is empty");
    }
    return head->data;
}

template<typename T>
T& DoubleList<T>::back() {
    if (!tail) {
        throw std::runtime_error("List is empty");
    }
    return tail->data;
}

template<typename T>
const T& DoubleList<T>::back() const {
    if (!tail) {
        throw std::runtime_error("List is empty");
    }
    return tail->data;
}

template<typename T>
size_t DoubleList<T>::getSize() const {
    return size;
}

template<typename T>
bool DoubleList<T>::isEmpty() const {
    return size == 0;
}

template<typename T>
void DoubleList<T>::clear() {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
    head = tail = nullptr;
    size = 0;
}

template<typename T>
bool DoubleList<T>::find(const T& value) const {
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
void DoubleList<T>::print() const {
    std::cout << "[";
    Node* current = head;
    while (current) {
        std::cout << current->data;
        if (current->next) std::cout << " <-> ";
        current = current->next;
    }
    std::cout << "]" << std::endl;
}

template<typename T>
void DoubleList<T>::printReverse() const {
    std::cout << "[";
    Node* current = tail;
    while (current) {
        std::cout << current->data;
        if (current->prev) std::cout << " <-> ";
        current = current->prev;
    }
    std::cout << "]" << std::endl;
}

template<typename T>
void DoubleList<T>::serialize(std::ostream& out) const {
    serializeBinary(out);
}

template<typename T>
void DoubleList<T>::deserialize(std::istream& in) {
    deserializeBinary(in);
}

// Важно: бинарная сериализация корректна только для тривиально копируемых типов
template<typename T>
void DoubleList<T>::serializeBinary(std::ostream& out) const {
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    Node* current = head;
    while (current) {
        out.write(reinterpret_cast<const char*>(&current->data), sizeof(T));
        current = current->next;
    }
}

// Важно: бинарная десериализация корректна только для тривиально копируемых типов
template<typename T>
void DoubleList<T>::deserializeBinary(std::istream& in) {
    clear();
    size_t new_size;
    in.read(reinterpret_cast<char*>(&new_size), sizeof(new_size));
    for (size_t i = 0; i < new_size; ++i) {
        T value;
        in.read(reinterpret_cast<char*>(&value), sizeof(T));
        pushBack(value);
    }
}

template<typename T>
void DoubleList<T>::serializeText(std::ostream& out) const {
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
void DoubleList<T>::deserializeText(std::istream& in) {
    clear();
    size_t new_size;
    in >> new_size;
    for (size_t i = 0; i < new_size; ++i) {
        T value;
        in >> value;
        pushBack(value);
    }
}