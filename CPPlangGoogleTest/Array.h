#pragma once
#include <iostream>
#include <stdexcept>

/**
 * @brief Класс динамического массива с автоматическим изменением ёмкости.
 * 
 * Реализует последовательный контейнер с доступом по индексу.
 * Управляет памятью вручную через new/delete.
 * 
 * @tparam T Тип элементов массива. Должен быть копируемым и конструируемым по умолчанию.
 * @warning Бинарная сериализация корректна только для тривиально копируемых типов (POD).
 */
template<typename T>
class Array {
private:
    T* data;         ///< Указатель на буфер данных
    size_t capacity; ///< Текущая выделенная ёмкость
    size_t size;     ///< Текущее количество элементов

    /**
     * @brief Изменяет ёмкость массива.
     * 
     * Выделяет новый буфер, копирует существующие элементы и освобождает старый буфер.
     * Если new_capacity меньше текущего размера, массив усекается.
     * 
     * @param new_capacity Новая ёмкость буфера.
     */
    void resize(size_t new_capacity);

public:
    /**
     * @brief Конструктор по умолчанию.
     * Создает пустой массив с нулевой ёмкостью.
     */
    Array();

    /**
     * @brief Конструктор с предварительным выделением памяти.
     * 
     * @param initial_capacity Начальная ёмкость массива.
     */
    explicit Array(size_t initial_capacity);

    /**
     * @brief Копирующий конструктор.
     * Выполняет глубокое копирование данных из другого массива.
     * 
     * @param other Массив-источник.
     */
    Array(const Array& other);

    /**
     * @brief Оператор присваивания.
     * Выполняет глубокое копирование с гарантией безопасности исключений (strong exception guarantee).
     * Сначала выделяется новая память, затем освобождается старая.
     * 
     * @param other Массив-источник.
     * @return Ссылка на текущий объект.
     */
    Array& operator=(const Array& other);

    /**
     * @brief Деструктор.
     * Освобождает выделенную память.
     */
    ~Array();

    /**
     * @brief Добавляет элемент в конец массива.
     * Если ёмкости недостаточно, происходит перевыделение памяти.
     * 
     * @param element Элемент для добавления.
     */
    void add(const T& element);

    /**
     * @brief Вставляет элемент по указанному индексу.
     * Элементы, начиная с index, сдвигаются вправо.
     * 
     * @param index Позиция вставки (от 0 до size).
     * @param element Элемент для вставки.
     * @throw std::out_of_range Если index > size.
     */
    void insert(size_t index, const T& element);

    /**
     * @brief Удаляет элемент по индексу.
     * Элементы после index сдвигаются влево.
     * 
     * @param index Индекс удаляемого элемента.
     * @throw std::out_of_range Если index >= size.
     */
    void remove(size_t index);

    /**
     * @brief Возвращает ссылку на элемент по индексу (с проверкой).
     * 
     * @param index Индекс элемента.
     * @return T& Ссылка на элемент.
     * @throw std::out_of_range Если index >= size.
     */
    T& get(size_t index);

    /**
     * @brief Возвращает константную ссылку на элемент по индексу (с проверкой).
     * 
     * @param index Индекс элемента.
     * @return const T& Константная ссылка на элемент.
     * @throw std::out_of_range Если index >= size.
     */
    const T& get(size_t index) const;

    /**
     * @brief Перезаписывает элемент по индексу.
     * 
     * @param index Индекс элемента.
     * @param element Новое значение.
     * @throw std::out_of_range Если index >= size.
     */
    void set(size_t index, const T& element);

    /**
     * @brief Возвращает текущее количество элементов.
     * @return size_t Размер массива.
     */
    size_t getSize() const;

    /**
     * @brief Возвращает текущую ёмкость буфера.
     * @return size_t Ёмкость.
     */
    size_t getCapacity() const;

    /**
     * @brief Проверяет, пуст ли массив.
     * @return true Если массив пуст.
     * @return false Если в массиве есть элементы.
     */
    bool isEmpty() const;

    /**
     * @brief Полностью очищает массив и освобождает память.
     * Размер и ёмкость становятся равными 0.
     */
    void clear();

    /**
     * @brief Выводит содержимое массива в стандартный поток вывода.
     * Формат: [elem1, elem2, ...]
     */
    void print() const;

    /**
     * @brief Сериализация (обертка).
     * По умолчанию вызывает serializeBinary.
     * 
     * @param out Поток вывода.
     */
    void serialize(std::ostream& out) const;

    /**
     * @brief Десериализация (обертка).
     * По умолчанию вызывает deserializeBinary.
     * 
     * @param in Поток ввода.
     */
    void deserialize(std::istream& in);

    /**
     * @brief Бинарная сериализация.
     * Записывает размер (size_t) и сырые байты данных.
     * 
     * @warning Небезопасно для типов с указателями, виртуальными методами и нетривиальной структурой.
     * @param out Поток вывода.
     */
    void serializeBinary(std::ostream& out) const;

    /**
     * @brief Бинарная десериализация.
     * Очищает массив, читает размер и восстанавливает данные.
     * 
     * @warning Перезаписывает память напрямую. Корректно только для POD-типов.
     * @param in Поток ввода.
     */
    void deserializeBinary(std::istream& in);

    /**
     * @brief Текстовая сериализация.
     * Формат: <размер> <elem1> <elem2> ...
     * Элементы разделяются пробелом.
     * 
     * @param out Поток вывода.
     */
    void serializeText(std::ostream& out) const;

    /**
     * @brief Текстовая десериализация.
     * Ожидает формат, совместимый с serializeText.
     * 
     * @param in Поток ввода.
     */
    void deserializeText(std::istream& in);

    /**
     * @brief Оператор доступа по индексу.
     * 
     * @note Выполняет проверку границ (вызывает get()).
     * @param index Индекс элемента.
     * @return T& Ссылка на элемент.
     * @throw std::out_of_range Если index >= size.
     */
    T& operator[](size_t index);

    /**
     * @brief Оператор доступа по индексу (const).
     * 
     * @note Выполняет проверку границ (вызывает get()).
     * @param index Индекс элемента.
     * @return const T& Константная ссылка на элемент.
     * @throw std::out_of_range Если index >= size.
     */
    const T& operator[](size_t index) const;
};

template<typename T>
Array<T>::Array() : data(nullptr), capacity(0), size(0) {}

template<typename T>
Array<T>::Array(size_t initial_capacity) : capacity(initial_capacity), size(0) {
    data = capacity > 0 ? new T[capacity] : nullptr;
}

template<typename T>
Array<T>::Array(const Array& other) : capacity(other.capacity), size(other.size) {
    data = capacity > 0 ? new T[capacity] : nullptr;
    for (size_t i = 0; i < size; ++i) {
        data[i] = other.data[i];
    }
}

template<typename T>
Array<T>& Array<T>::operator=(const Array& other) {
    if (this != &other) {
        // Сначала выделяем новую память
        T* new_data = other.capacity > 0 ? new T[other.capacity] : nullptr;
        
        // Копируем данные
        for (size_t i = 0; i < other.size; ++i) {
            new_data[i] = other.data[i];
        }

        // Если все прошло успешно, удаляем старую и подменяем
        delete[] data;
        data = new_data;
        capacity = other.capacity;
        size = other.size;
    }
    return *this;
}

template<typename T>
Array<T>::~Array() {
    delete[] data;
}

template<typename T>
void Array<T>::resize(size_t new_capacity) {
    T* new_data = new_capacity > 0 ? new T[new_capacity] : nullptr;
    size_t copy_size = size < new_capacity ? size : new_capacity;
    for (size_t i = 0; i < copy_size; ++i) {
        new_data[i] = data[i];
    }
    delete[] data;
    data = new_data;
    capacity = new_capacity;
    if (size > capacity) size = capacity;
}

template<typename T>
void Array<T>::add(const T& element) {
    if (size >= capacity) {
        resize(capacity == 0 ? 1 : capacity * 2);
    }
    data[size++] = element;
}

template<typename T>
void Array<T>::insert(size_t index, const T& element) {
    if (index > size) {
        throw std::out_of_range("Index out of range");
    }
    if (size >= capacity) {
        resize(capacity == 0 ? 1 : capacity * 2);
    }
    for (size_t i = size; i > index; --i) {
        data[i] = data[i - 1];
    }
    data[index] = element;
    ++size;
}

template<typename T>
void Array<T>::remove(size_t index) {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    for (size_t i = index; i < size - 1; ++i) {
        data[i] = data[i + 1];
    }
    --size;
}

template<typename T>
T& Array<T>::get(size_t index) {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

template<typename T>
const T& Array<T>::get(size_t index) const {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

template<typename T>
void Array<T>::set(size_t index, const T& element) {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    data[index] = element;
}

template<typename T>
size_t Array<T>::getSize() const {
    return size;
}

template<typename T>
size_t Array<T>::getCapacity() const {
    return capacity;
}

template<typename T>
bool Array<T>::isEmpty() const {
    return size == 0;
}

template<typename T>
void Array<T>::clear() {
    // Освобождаем буфер и сбрасываем состояние контейнера
    delete[] data;
    data = nullptr;
    capacity = 0;
    size = 0;
}

template<typename T>
void Array<T>::print() const {
    std::cout << "[";
    for (size_t i = 0; i < size; ++i) {
        std::cout << data[i];
        if (i < size - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

template<typename T>
void Array<T>::serialize(std::ostream& out) const {
    serializeBinary(out);
}

template<typename T>
void Array<T>::deserialize(std::istream& in) {
    deserializeBinary(in);
}

// Важно: бинарная сериализация корректна только для тривиально копируемых типов
// Нетривиальные типы следует сериализовать текстово или вручную
template<typename T>
void Array<T>::serializeBinary(std::ostream& out) const {
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (size_t i = 0; i < size; ++i) {
        out.write(reinterpret_cast<const char*>(&data[i]), sizeof(T));
    }
}

// Важно: бинарная десериализация корректна только для тривиально копируемых типов
template<typename T>
void Array<T>::deserializeBinary(std::istream& in) {
    clear();
    size_t new_size;
    in.read(reinterpret_cast<char*>(&new_size), sizeof(new_size));
    if (new_size > capacity) {
        resize(new_size);
    }
    size = new_size;
    for (size_t i = 0; i < size; ++i) {
        in.read(reinterpret_cast<char*>(&data[i]), sizeof(T));
    }
}

template<typename T>
void Array<T>::serializeText(std::ostream& out) const {
    out << size << std::endl;
    for (size_t i = 0; i < size; ++i) {
        out << data[i];
        if (i < size - 1) out << " ";
    }
    out << std::endl;
}

template<typename T>
void Array<T>::deserializeText(std::istream& in) {
    clear();
    size_t new_size;
    in >> new_size;
    if (new_size > capacity) {
        resize(new_size);
    }
    size = new_size;
    for (size_t i = 0; i < size; ++i) {
        in >> data[i];
    }
}

template<typename T>
T& Array<T>::operator[](size_t index) {
    return get(index);
}

template<typename T>
const T& Array<T>::operator[](size_t index) const {
    return get(index);
}