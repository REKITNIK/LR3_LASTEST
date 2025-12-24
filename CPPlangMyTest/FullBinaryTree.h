#pragma once
#include <iostream>
#include <stdexcept>
#include <queue>
#include <sstream>
#include <string> // Явно включено для поддержки std::string

/**
 * @brief Шаблонный класс полного бинарного дерева (Full Binary Tree).
 *
 * Реализует структуру данных, где каждый узел имеет либо 0, либо 2 потомка.
 * Вставка новых элементов происходит таким образом, чтобы дерево заполнялось равномерно,
 * поддерживая свойство полноты.
 *
 * @tparam T Тип хранимых данных.
 */
template<typename T>
class FullBinaryTree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        Node(const T& value) : data(value), left(nullptr), right(nullptr) {}
    };

    Node* root;
    size_t size;

    void destroyTree(Node* node);
    Node* copyTree(Node* node);
    bool isFullBinaryTreeHelper(Node* node) const;
    void printInOrderHelper(Node* node) const;
    void serializeHelper(Node* node, std::ostream& out) const;
    Node* deserializeHelper(std::istream& in);
    void serializeBinaryHelper(Node* node, std::ostream& out) const;
    Node* deserializeBinaryHelper(std::istream& in);

public:
    /**
     * @brief Конструктор по умолчанию. Создает пустое дерево.
     */
    FullBinaryTree();

    /**
     * @brief Конструктор копирования.
     * Создает полную (глубокую) копию переданного дерева.
     * @param other Дерево, из которого производится копирование.
     */
    FullBinaryTree(const FullBinaryTree& other);

    /**
     * @brief Оператор присваивания.
     * Выполняет глубокое копирование содержимого.
     * Гарантирует строгую безопасность исключений: если копирование не удастся,
     * исходное дерево останется неизменным.
     * @param other Дерево для присваивания.
     * @return Ссылка на текущий объект.
     */
    FullBinaryTree& operator=(const FullBinaryTree& other);

    /**
     * @brief Деструктор.
     * Освобождает всю память, выделенную под узлы дерева.
     */
    ~FullBinaryTree();

    /**
     * @brief Вставляет значение в дерево.
     * Алгоритм ищет первый свободный лист (в порядке обхода в ширину) и добавляет
     * к нему два дочерних узла с переданным значением, сохраняя инвариант полноты дерева.
     * @param value Значение для вставки.
     */
    void insert(const T& value);

    /**
     * @brief Удаляет значение из дерева.
     * Если удаляемый узел — лист, удаляется также его "брат".
     * Если узел внутренний, его значение замещается значением самого правого листа,
     * после чего удаляется этот лист и его брат.
     * @param value Значение для удаления.
     */
    void remove(const T& value);

    /**
     * @brief Ищет значение в дереве.
     * @param value Искомое значение.
     * @return true, если значение найдено, иначе false.
     */
    bool find(const T& value) const;

    /**
     * @brief Проверяет корректность структуры полного бинарного дерева.
     * @return true, если у каждого узла либо 0, либо 2 потомка.
     */
    bool isFullBinaryTree() const;

    /**
     * @brief Возвращает текущее количество узлов.
     * @return Размер дерева.
     */
    size_t getSize() const;

    /**
     * @brief Проверяет, пусто ли дерево.
     * @return true, если в дереве нет узлов.
     */
    bool isEmpty() const;

    /**
     * @brief Очищает дерево.
     * Удаляет все узлы и сбрасывает размер до 0.
     */
    void clear();

    /**
     * @brief Выводит содержимое дерева в стандартный поток вывода (обход в ширину).
     */
    void print() const;

    /**
     * @brief Выводит содержимое дерева (симметричный обход / In-Order).
     */
    void printInOrder() const;

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
     * Обеспечивает максимальную производительность и компактность за счет
     * прямого дампа памяти узлов.
     * @note Предназначена для тривиально копируемых типов (POD).
     * @param out Поток вывода.
     */
    void serializeBinary(std::ostream& out) const;

    /**
     * @brief Бинарная десериализация.
     * Восстанавливает дерево из бинарного формата.
     * @note Ожидает данные тривиально копируемых типов (POD).
     * @param in Поток ввода.
     */
    void deserializeBinary(std::istream& in);

    /**
     * @brief Текстовая сериализация.
     * Сохраняет структуру дерева в читаемом текстовом формате.
     * @param out Поток вывода.
     */
    void serializeText(std::ostream& out) const;

    /**
     * @brief Текстовая десериализация.
     * Восстанавливает дерево из текстового представления.
     * @param in Поток ввода.
     */
    void deserializeText(std::istream& in);
};

template<typename T>
FullBinaryTree<T>::FullBinaryTree() : root(nullptr), size(0) {}

template<typename T>
FullBinaryTree<T>::FullBinaryTree(const FullBinaryTree& other) : root(nullptr), size(other.size) {
    root = copyTree(other.root);
}

template<typename T>
FullBinaryTree<T>& FullBinaryTree<T>::operator=(const FullBinaryTree& other) {
    if (this != &other) {
        // Сначала пытаемся создать копию нового дерева.
        // Если здесь произойдет исключение (например, нехватка памяти),
        // текущий объект (this) останется в валидном состоянии.
        Node* newRoot = copyTree(other.root);
        
        // Если копирование прошло успешно, освобождаем старую память
        clear();
        
        // Применяем новые данные
        root = newRoot;
        size = other.size;
    }
    return *this;
}

template<typename T>
FullBinaryTree<T>::~FullBinaryTree() {
    clear();
}

template<typename T>
void FullBinaryTree<T>::destroyTree(Node* node) {
    if (node) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

template<typename T>
typename FullBinaryTree<T>::Node* FullBinaryTree<T>::copyTree(Node* node) {
    if (!node) return nullptr;

    Node* newNode = new Node(node->data);
    newNode->left = copyTree(node->left);
    newNode->right = copyTree(node->right);
    return newNode;
}

template<typename T>
void FullBinaryTree<T>::insert(const T& value) {
    if (!root) {
        // Первая вставка: создаем только корень как лист (0 потомков)
        root = new Node(value);
        size = 1;
        return;
    }

    // Находим первый лист (0 потомков), используя обход в ширину
    std::queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        // Если узел является листом (нет детей), добавляем двух детей для сохранения полноты дерева
        if (!current->left && !current->right) {
            current->left = new Node(value);
            current->right = new Node(value);
            size += 2;
            return;
        }

        // Продолжаем обход только по узлам, у которых есть дети
        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }
}

template<typename T>
void FullBinaryTree<T>::remove(const T& value) {
    if (!root) return;

    // Находим узел для удаления и его родителя
    Node* parent = nullptr;
    Node* target = nullptr;
    std::queue<std::pair<Node*, Node*>> q; // пара (узел, родитель)
    q.push({root, nullptr});

    while (!q.empty() && !target) {
        auto [current, par] = q.front();
        q.pop();

        if (current->data == value) {
            target = current;
            parent = par;
            break;
        }

        if (current->left) q.push({current->left, current});
        if (current->right) q.push({current->right, current});
    }

    if (!target) return; // Значение не найдено

    // Если удаляемый узел — лист, нужно удалить и его брата для сохранения полноты дерева
    if (!target->left && !target->right) {
        if (parent) {
            // Удаляем обоих детей родителя
            delete parent->left;
            delete parent->right;
            parent->left = parent->right = nullptr;
            size -= 2;
        } else {
            // Удаление корня (который является листом)
            delete root;
            root = nullptr;
            size = 0;
        }
    } else if (target->left && target->right) {
        // Если у цели два ребенка, находим самый правый лист и заменяем данные цели
        Node* rightmostParent = nullptr;
        Node* rightmost = nullptr;

        std::queue<std::pair<Node*, Node*>> leafQueue;
        leafQueue.push({root, nullptr});

        while (!leafQueue.empty()) {
            auto [current, par] = leafQueue.front();
            leafQueue.pop();

            if (!current->left && !current->right) {
                rightmost = current;
                rightmostParent = par;
            }

            if (current->left) leafQueue.push({current->left, current});
            if (current->right) leafQueue.push({current->right, current});
        }

        if (rightmost && rightmost != target) {
            // Заменяем данные цели данными самого правого листа
            target->data = rightmost->data;
            // Удаляем самый правый лист и его брата
            if (rightmostParent) {
                delete rightmostParent->left;
                delete rightmostParent->right;
                rightmostParent->left = rightmostParent->right = nullptr;
                size -= 2;
            }
        }
    }
    // Примечание: Если у узла только один ребенок, это нарушает свойство полного бинарного дерева.
    // Этот случай не должен возникать в корректно поддерживаемом дереве.
}

template<typename T>
bool FullBinaryTree<T>::find(const T& value) const {
    if (!root) return false;

    std::queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        if (current->data == value) {
            return true;
        }

        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }

    return false;
}

template<typename T>
bool FullBinaryTree<T>::isFullBinaryTreeHelper(Node* node) const {
    if (!node) return true;

    // У узла в полном бинарном дереве должно быть либо 0, либо 2 потомка
    if ((!node->left && node->right) || (node->left && !node->right)) {
        return false;
    }

    return isFullBinaryTreeHelper(node->left) && isFullBinaryTreeHelper(node->right);
}

template<typename T>
bool FullBinaryTree<T>::isFullBinaryTree() const {
    return isFullBinaryTreeHelper(root);
}

template<typename T>
size_t FullBinaryTree<T>::getSize() const {
    return size;
}

template<typename T>
bool FullBinaryTree<T>::isEmpty() const {
    return size == 0;
}

template<typename T>
void FullBinaryTree<T>::clear() {
    destroyTree(root);
    root = nullptr;
    size = 0;
}

template<typename T>
void FullBinaryTree<T>::print() const {
    if (!root) {
        std::cout << "Empty tree" << std::endl;
        return;
    }

    std::cout << "Level-order traversal: ";
    std::queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        std::cout << current->data << " ";

        if (current->left) q.push(current->left);
        if (current->right) q.push(current->right);
    }
    std::cout << std::endl;
}

template<typename T>
void FullBinaryTree<T>::printInOrderHelper(Node* node) const {
    if (node) {
        printInOrderHelper(node->left);
        std::cout << node->data << " ";
        printInOrderHelper(node->right);
    }
}

template<typename T>
void FullBinaryTree<T>::printInOrder() const {
    std::cout << "In-order traversal: ";
    printInOrderHelper(root);
    std::cout << std::endl;
}

template<typename T>
void FullBinaryTree<T>::serializeHelper(Node* node, std::ostream& out) const {
    if (!node) {
        out << "null ";
        return;
    }

    out << node->data << " ";
    serializeHelper(node->left, out);
    serializeHelper(node->right, out);
}

template<typename T>
void FullBinaryTree<T>::serialize(std::ostream& out) const {
    // По умолчанию используется бинарная сериализация
    serializeBinary(out);
}

template<typename T>
void FullBinaryTree<T>::deserialize(std::istream& in) {
    // По умолчанию используется бинарная десериализация
    deserializeBinary(in);
}

// Важно: бинарная сериализация корректна только для тривиально копируемых типов
template<typename T>
void FullBinaryTree<T>::serializeBinary(std::ostream& out) const {
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    serializeBinaryHelper(root, out);
}

// Важно: бинарная десериализация корректна только для тривиально копируемых типов
template<typename T>
void FullBinaryTree<T>::deserializeBinary(std::istream& in) {
    clear();
    
    size_t new_size;
    in.read(reinterpret_cast<char*>(&new_size), sizeof(new_size));
    size = new_size;
    
    root = deserializeBinaryHelper(in);
}

template<typename T>
void FullBinaryTree<T>::serializeText(std::ostream& out) const {
    out << size << std::endl;
    serializeHelper(root, out);
    out << std::endl;
}

template<typename T>
void FullBinaryTree<T>::deserializeText(std::istream& in) {
    clear();

    size_t new_size;
    in >> new_size;
    size = new_size;

    root = deserializeHelper(in);
}

template<typename T>
typename FullBinaryTree<T>::Node* FullBinaryTree<T>::deserializeHelper(std::istream& in) {
    std::string token;
    if (!(in >> token) || token == "null") {
        return nullptr;
    }

    std::istringstream iss(token);
    T value;
    iss >> value;

    Node* node = new Node(value);
    node->left = deserializeHelper(in);
    node->right = deserializeHelper(in);

    return node;
}

template<typename T>
void FullBinaryTree<T>::serializeBinaryHelper(Node* node, std::ostream& out) const {
    if (!node) {
        bool is_null = true;
        out.write(reinterpret_cast<const char*>(&is_null), sizeof(is_null));
        return;
    }

    bool is_null = false;
    out.write(reinterpret_cast<const char*>(&is_null), sizeof(is_null));
    out.write(reinterpret_cast<const char*>(&node->data), sizeof(T));
    serializeBinaryHelper(node->left, out);
    serializeBinaryHelper(node->right, out);
}

template<typename T>
typename FullBinaryTree<T>::Node* FullBinaryTree<T>::deserializeBinaryHelper(std::istream& in) {
    bool is_null;
    in.read(reinterpret_cast<char*>(&is_null), sizeof(is_null));
    
    if (is_null) {
        return nullptr;
    }

    T value;
    in.read(reinterpret_cast<char*>(&value), sizeof(T));

    Node* node = new Node(value);
    node->left = deserializeBinaryHelper(in);
    node->right = deserializeBinaryHelper(in);

    return node;
}