#pragma once
#include <iostream>
#include <stdexcept>
#include <functional>
#include <string>  // Явно включено для поддержки std::string
#include <utility> // Для std::swap

/**
 * @brief Шаблонный класс Хэш-таблицы (Hash Table).
 *
 * Реализует словарь (ассоциативный массив) на основе массива списков (метод цепочек).
 * При возникновении коллизий элементы добавляются в начало связного списка соответствующего бакета.
 * Автоматически расширяется (rehashing) при превышении коэффициента заполнения.
 *
 * @tparam K Тип ключа. Должен поддерживать сравнение (operator==) и хеширование (std::hash).
 * @tparam V Тип значения.
 */
template<typename K, typename V>
class HashTable {
private:
    struct Entry {
        K key;
        V value;
        Entry* next;
        Entry(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };

    Entry** buckets;
    size_t bucket_count;
    size_t size;

    size_t hash(const K& key) const;
    void rehash();

public:
    /**
     * @brief Конструктор по умолчанию.
     * Создает таблицу с начальным размером 16 бакетов.
     */
    HashTable();

    /**
     * @brief Конструктор с заданным размером.
     * @param initial_bucket_count Начальное количество бакетов.
     */
    explicit HashTable(size_t initial_bucket_count);

    /**
     * @brief Конструктор копирования.
     * Создает глубокую копию таблицы. Порядок элементов в цепочках может меняться (инвертироваться)
     * относительно оригинала из-за особенностей вставки в начало списка.
     * @param other Таблица для копирования.
     */
    HashTable(const HashTable& other);

    /**
     * @brief Оператор присваивания.
     * Обеспечивает строгую гарантию безопасности исключений (Strong Exception Guarantee).
     * @param other Таблица, значения которой присваиваются.
     * @return Ссылка на текущий объект.
     */
    HashTable& operator=(const HashTable& other);

    /**
     * @brief Деструктор. Освобождает всю выделенную память.
     */
    ~HashTable();

    /**
     * @brief Вставляет или обновляет пару ключ-значение.
     * Если ключ уже существует, его значение обновляется.
     * Если коэффициент заполнения превышает 0.75, происходит рехеширование.
     * @param key Ключ.
     * @param value Значение.
     */
    void insert(const K& key, const V& value);

    /**
     * @brief Удаляет элемент по ключу.
     * @param key Ключ удаляемого элемента.
     * @throw std::runtime_error Если ключ не найден.
     */
    void remove(const K& key);

    /**
     * @brief Получает доступ к значению по ключу (для чтения и записи).
     * @param key Искомый ключ.
     * @return Ссылка на значение.
     * @throw std::runtime_error Если ключ не найден.
     */
    V& get(const K& key);

    /**
     * @brief Получает доступ к значению по ключу (только для чтения).
     * @param key Искомый ключ.
     * @return Константная ссылка на значение.
     * @throw std::runtime_error Если ключ не найден.
     */
    const V& get(const K& key) const;

    /**
     * @brief Проверяет наличие ключа в таблице.
     * @param key Искомый ключ.
     * @return true, если ключ найден, иначе false.
     */
    bool find(const K& key) const;

    /**
     * @brief Возвращает текущее количество элементов.
     * @return Число пар ключ-значение.
     */
    size_t getSize() const;

    /**
     * @brief Возвращает текущее количество бакетов (размер массива).
     * @return Число бакетов.
     */
    size_t getBucketCount() const;

    /**
     * @brief Проверяет, пуста ли таблица.
     * @return true, если таблица не содержит элементов.
     */
    bool isEmpty() const;

    /**
     * @brief Полностью очищает таблицу.
     * Удаляет все элементы, но сохраняет массив бакетов (не меняет bucket_count).
     */
    void clear();

    /**
     * @brief Вычисляет текущий коэффициент заполнения (Load Factor).
     * @return Отношение количества элементов к количеству бакетов (size / bucket_count).
     */
    double loadFactor() const;

    /**
     * @brief Выводит содержимое непустых бакетов в консоль.
     */
    void print() const;

    /**
     * @brief Универсальная сериализация.
     * По умолчанию делегирует вызов бинарной сериализации.
     * @param out Поток вывода.
     */
    void serialize(std::ostream& out) const;

    /**
     * @brief Универсальная десериализация.
     * По умолчанию делегирует вызов бинарной десериализации.
     * @param in Поток ввода.
     */
    void deserialize(std::istream& in);

    /**
     * @brief Бинарная сериализация.
     * Сохраняет данные путем побайтового копирования памяти.
     * @note Работает корректно только для тривиально копируемых типов (POD) ключей и значений.
     * Использование std::string или указателей приведет к некорректным данным.
     * @param out Поток вывода.
     */
    void serializeBinary(std::ostream& out) const;

    /**
     * @brief Бинарная десериализация.
     * Восстанавливает таблицу из бинарного формата.
     * @note Требует использования POD-типов.
     * @param in Поток ввода.
     */
    void deserializeBinary(std::istream& in);

    /**
     * @brief Текстовая сериализация.
     * Сохраняет таблицу в читаемом текстовом виде.
     * Формат: <размер> <кол-во бакетов>, затем строки вида <ключ> <значение>.
     * @param out Поток вывода.
     */
    void serializeText(std::ostream& out) const;

    /**
     * @brief Текстовая десериализация.
     * Восстанавливает таблицу из текстового формата.
     * @param in Поток ввода.
     */
    void deserializeText(std::istream& in);

    /**
     * @brief Оператор доступа по индексу (ключу).
     * Возвращает ссылку на значение по ключу. Если ключ отсутствует,
     * вставляет новую пару с дефолтным значением типа V.
     * @param key Ключ.
     * @return Ссылка на значение.
     */
    V& operator[](const K& key);
};

template<typename K, typename V>
HashTable<K, V>::HashTable() : bucket_count(16), size(0) {
    buckets = new Entry*[bucket_count];
    for (size_t i = 0; i < bucket_count; ++i) {
        buckets[i] = nullptr;
    }
}

template<typename K, typename V>
HashTable<K, V>::HashTable(size_t initial_bucket_count)
    : bucket_count(initial_bucket_count > 0 ? initial_bucket_count : 16), size(0) {
    buckets = new Entry*[bucket_count];
    for (size_t i = 0; i < bucket_count; ++i) {
        buckets[i] = nullptr;
    }
}

template<typename K, typename V>
HashTable<K, V>::HashTable(const HashTable& other)
    : bucket_count(other.bucket_count), size(0) {
    buckets = new Entry*[bucket_count];
    for (size_t i = 0; i < bucket_count; ++i) {
        buckets[i] = nullptr;
    }

    for (size_t i = 0; i < other.bucket_count; ++i) {
        Entry* current = other.buckets[i];
        while (current) {
            insert(current->key, current->value);
            current = current->next;
        }
    }
}

template<typename K, typename V>
HashTable<K, V>& HashTable<K, V>::operator=(const HashTable& other) {
    if (this != &other) {
        // Идиома Copy-and-Swap для гарантии безопасности исключений.
        // 1. Создаем временную копию. Если здесь произойдет исключение (bad_alloc),
        // текущий объект (this) останется нетронутым.
        HashTable temp(other);

        // 2. Меняем местами внутренние ресурсы this и временного объекта.
        // Операции swap не выбрасывают исключений.
        std::swap(buckets, temp.buckets);
        std::swap(bucket_count, temp.bucket_count);
        std::swap(size, temp.size);

        // 3. При выходе из if деструктор temp очистит старые ресурсы (которые теперь в temp).
    }
    return *this;
}

template<typename K, typename V>
HashTable<K, V>::~HashTable() {
    clear();
    delete[] buckets;
}

template<typename K, typename V>
size_t HashTable<K, V>::hash(const K& key) const {
    return std::hash<K>{}(key) % bucket_count;
}

template<typename K, typename V>
void HashTable<K, V>::rehash() {
    Entry** old_buckets = buckets;
    size_t old_bucket_count = bucket_count;

    bucket_count *= 2;
    buckets = new Entry*[bucket_count];
    for (size_t i = 0; i < bucket_count; ++i) {
        buckets[i] = nullptr;
    }

    size = 0;

    for (size_t i = 0; i < old_bucket_count; ++i) {
        Entry* current = old_buckets[i];
        while (current) {
            Entry* next = current->next;
            size_t new_index = hash(current->key);
            current->next = buckets[new_index];
            buckets[new_index] = current;
            ++size;
            current = next;
        }
    }

    delete[] old_buckets;
}

template<typename K, typename V>
void HashTable<K, V>::insert(const K& key, const V& value) {
    if (loadFactor() > 0.75) {
        rehash();
    }

    size_t index = hash(key);
    Entry* current = buckets[index];

    while (current) {
        if (current->key == key) {
            current->value = value;
            return;
        }
        current = current->next;
    }

    Entry* newEntry = new Entry(key, value);
    newEntry->next = buckets[index];
    buckets[index] = newEntry;
    ++size;
}

template<typename K, typename V>
void HashTable<K, V>::remove(const K& key) {
    size_t index = hash(key);
    Entry* current = buckets[index];
    Entry* prev = nullptr;

    while (current) {
        if (current->key == key) {
            if (prev) {
                prev->next = current->next;
            } else {
                buckets[index] = current->next;
            }
            delete current;
            --size;
            return;
        }
        prev = current;
        current = current->next;
    }

    throw std::runtime_error("Key not found");
}

template<typename K, typename V>
V& HashTable<K, V>::get(const K& key) {
    size_t index = hash(key);
    Entry* current = buckets[index];

    while (current) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }

    throw std::runtime_error("Key not found");
}

template<typename K, typename V>
const V& HashTable<K, V>::get(const K& key) const {
    size_t index = hash(key);
    Entry* current = buckets[index];

    while (current) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }

    throw std::runtime_error("Key not found");
}

template<typename K, typename V>
bool HashTable<K, V>::find(const K& key) const {
    size_t index = hash(key);
    Entry* current = buckets[index];

    while (current) {
        if (current->key == key) {
            return true;
        }
        current = current->next;
    }
    return false;
}

template<typename K, typename V>
size_t HashTable<K, V>::getSize() const {
    return size;
}

template<typename K, typename V>
size_t HashTable<K, V>::getBucketCount() const {
    return bucket_count;
}

template<typename K, typename V>
bool HashTable<K, V>::isEmpty() const {
    return size == 0;
}

template<typename K, typename V>
void HashTable<K, V>::clear() {
    for (size_t i = 0; i < bucket_count; ++i) {
        Entry* current = buckets[i];
        while (current) {
            Entry* temp = current;
            current = current->next;
            delete temp;
        }
        buckets[i] = nullptr;
    }
    size = 0;
}

template<typename K, typename V>
double HashTable<K, V>::loadFactor() const {
    return static_cast<double>(size) / bucket_count;
}

template<typename K, typename V>
void HashTable<K, V>::print() const {
    std::cout << "HashTable (size: " << size << ", buckets: " << bucket_count << "):" << std::endl;
    for (size_t i = 0; i < bucket_count; ++i) {
        if (buckets[i]) {
            std::cout << "  Bucket " << i << ": ";
            Entry* current = buckets[i];
            while (current) {
                std::cout << "[" << current->key << ":" << current->value << "]";
                if (current->next) std::cout << " -> ";
                current = current->next;
            }
            std::cout << std::endl;
        }
    }
}

template<typename K, typename V>
void HashTable<K, V>::serialize(std::ostream& out) const {
    serializeBinary(out);
}

template<typename K, typename V>
void HashTable<K, V>::deserialize(std::istream& in) {
    deserializeBinary(in);
}

// Важно: бинарная сериализация корректна только для тривиально копируемых типов ключа и значения
template<typename K, typename V>
void HashTable<K, V>::serializeBinary(std::ostream& out) const {
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    out.write(reinterpret_cast<const char*>(&bucket_count), sizeof(bucket_count));

    for (size_t i = 0; i < bucket_count; ++i) {
        Entry* current = buckets[i];
        while (current) {
            out.write(reinterpret_cast<const char*>(&current->key), sizeof(K));
            out.write(reinterpret_cast<const char*>(&current->value), sizeof(V));
            current = current->next;
        }
    }
}

// Важно: бинарная десериализация корректна только для тривиально копируемых типов ключа и значения
template<typename K, typename V>
void HashTable<K, V>::deserializeBinary(std::istream& in) {
    clear();
    delete[] buckets;

    size_t new_size, new_bucket_count;
    in.read(reinterpret_cast<char*>(&new_size), sizeof(new_size));
    in.read(reinterpret_cast<char*>(&new_bucket_count), sizeof(new_bucket_count));

    bucket_count = new_bucket_count;
    size = 0; 
    buckets = new Entry*[bucket_count];
    for (size_t i = 0; i < bucket_count; ++i) {
        buckets[i] = nullptr;
    }

    for (size_t i = 0; i < new_size; ++i) {
        K key;
        V value;
        in.read(reinterpret_cast<char*>(&key), sizeof(K));
        in.read(reinterpret_cast<char*>(&value), sizeof(V));
        insert(key, value);
    }
}

template<typename K, typename V>
void HashTable<K, V>::serializeText(std::ostream& out) const {
    out << size << " " << bucket_count << std::endl;

    for (size_t i = 0; i < bucket_count; ++i) {
        Entry* current = buckets[i];
        while (current) {
            out << current->key << " " << current->value << std::endl;
            current = current->next;
        }
    }
}

template<typename K, typename V>
void HashTable<K, V>::deserializeText(std::istream& in) {
    clear();
    delete[] buckets;

    size_t new_size, new_bucket_count;
    in >> new_size >> new_bucket_count;

    bucket_count = new_bucket_count;
    size = 0; 
    buckets = new Entry*[bucket_count];
    for (size_t i = 0; i < bucket_count; ++i) {
        buckets[i] = nullptr;
    }

    for (size_t i = 0; i < new_size; ++i) {
        K key;
        V value;
        in >> key >> value;
        insert(key, value);
    }
}

template<typename K, typename V>
V& HashTable<K, V>::operator[](const K& key) {
    size_t index = hash(key);
    Entry* current = buckets[index];

    while (current) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }

    insert(key, V{});
    return get(key);
}