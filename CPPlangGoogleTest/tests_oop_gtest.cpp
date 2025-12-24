#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include "Array.h"
#include "ForwardList.h"
#include "DoubleList.h"
#include "Queue.h"
#include "Stack.h"
#include "HashTable.h"
#include "FullBinaryTree.h"

// ==============================
// Array Tests
// ==============================
TEST(ArrayTest, DefaultConstructor) {
    Array<int> arr;
    EXPECT_EQ(arr.getSize(), 0);
    EXPECT_TRUE(arr.isEmpty());
}

TEST(ArrayTest, AddAndGet) {
    Array<int> arr;
    arr.add(10);
    arr.add(20);
    EXPECT_EQ(arr.getSize(), 2);
    EXPECT_EQ(arr.get(0), 10);
    EXPECT_EQ(arr.get(1), 20);
}

TEST(ArrayTest, Insert) {
    Array<int> arr;
    arr.add(10);
    arr.add(30);
    arr.insert(1, 20);
    EXPECT_EQ(arr.getSize(), 3);
    EXPECT_EQ(arr.get(0), 10);
    EXPECT_EQ(arr.get(1), 20);
    EXPECT_EQ(arr.get(2), 30);
}

TEST(ArrayTest, Remove) {
    Array<int> arr;
    arr.add(10);
    arr.add(20);
    arr.add(30);
    arr.remove(1);
    EXPECT_EQ(arr.getSize(), 2);
    EXPECT_EQ(arr.get(0), 10);
    EXPECT_EQ(arr.get(1), 30);
}

TEST(ArrayTest, CopyConstructor) {
    Array<int> arr1;
    arr1.add(1);
    arr1.add(2);
    Array<int> arr2(arr1);
    EXPECT_EQ(arr2.getSize(), 2);
    EXPECT_EQ(arr2.get(0), 1);
    EXPECT_EQ(arr2.get(1), 2);
}

TEST(ArrayTest, AssignmentOperator) {
    Array<int> arr1;
    arr1.add(1);
    arr1.add(2);
    Array<int> arr2;
    arr2 = arr1;
    EXPECT_EQ(arr2.getSize(), 2);
    EXPECT_EQ(arr2.get(1), 2);
}

TEST(ArrayTest, Serialization) {
    Array<int> arr;
    arr.add(1);
    arr.add(2);
    arr.add(3);

    std::stringstream ss;
    arr.serialize(ss);

    Array<int> arr2;
    arr2.deserialize(ss);
    EXPECT_EQ(arr2.getSize(), 3);
    EXPECT_EQ(arr2.get(0), 1);
    EXPECT_EQ(arr2.get(2), 3);
}

TEST(ArrayTest, OutOfRangeException) {
    Array<int> arr;
    EXPECT_THROW(arr.get(0), std::out_of_range);
    arr.add(10);
    EXPECT_NO_THROW(arr.get(0));
    EXPECT_THROW(arr.get(1), std::out_of_range);
}

// ==============================
// ForwardList Tests
// ==============================
TEST(ForwardListTest, PushFront) {
    ForwardList<int> list;
    list.pushFront(10);
    list.pushFront(20);
    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.front(), 20);
}

TEST(ForwardListTest, PushBack) {
    ForwardList<int> list;
    list.pushBack(10);
    list.pushBack(20);
    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
}

TEST(ForwardListTest, Find) {
    ForwardList<int> list;
    list.pushBack(10);
    list.pushBack(20);
    EXPECT_TRUE(list.find(10));
    EXPECT_FALSE(list.find(30));
}

TEST(ForwardListTest, RemoveValue) {
    ForwardList<int> list;
    list.pushBack(10);
    list.pushBack(20);
    list.pushBack(10);
    list.removeValue(10);
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list.front(), 20);
}

// ==============================
// DoubleList Tests
// ==============================
TEST(DoubleListTest, PushFrontAndBack) {
    DoubleList<int> list;
    list.pushFront(10);
    list.pushBack(20);
    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.front(), 10);
    EXPECT_EQ(list.back(), 20);
}

TEST(DoubleListTest, PopFrontAndBack) {
    DoubleList<int> list;
    list.pushBack(10);
    list.pushBack(20);
    list.pushBack(30);
    list.popFront();
    EXPECT_EQ(list.front(), 20);
    list.popBack();
    EXPECT_EQ(list.back(), 20);
}

TEST(DoubleListTest, ReverseTraversal) {
    DoubleList<int> list;
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    // Проверка через get (оптимизированный доступ)
    EXPECT_EQ(list.get(0), 1);
    EXPECT_EQ(list.get(2), 3);
}

// ==============================
// Queue Tests
// ==============================
TEST(QueueTest, EnqueueDequeue) {
    Queue<int> queue;
    queue.enqueue(10);
    queue.enqueue(20);
    EXPECT_EQ(queue.getSize(), 2);
    EXPECT_EQ(queue.front(), 10);
    EXPECT_EQ(queue.back(), 20);
    
    queue.dequeue();
    EXPECT_EQ(queue.front(), 20);
    EXPECT_EQ(queue.getSize(), 1);
}

TEST(QueueTest, CopyConstructor) {
    Queue<int> q1;
    q1.enqueue(1);
    q1.enqueue(2);
    Queue<int> q2(q1);
    EXPECT_EQ(q2.getSize(), 2);
    EXPECT_EQ(q2.front(), 1);
    EXPECT_EQ(q2.back(), 2);
}

TEST(QueueTest, Clear) {
    Queue<int> queue;
    queue.enqueue(10);
    queue.clear();
    EXPECT_TRUE(queue.isEmpty());
    EXPECT_THROW(queue.front(), std::runtime_error);
}

// ==============================
// Stack Tests
// ==============================
TEST(StackTest, PushPop) {
    Stack<int> stack;
    stack.push(10);
    stack.push(20);
    EXPECT_EQ(stack.getSize(), 2);
    EXPECT_EQ(stack.top(), 20);
    
    stack.pop();
    EXPECT_EQ(stack.top(), 10);
    EXPECT_EQ(stack.getSize(), 1);
}

TEST(StackTest, CopyConstructor) {
    Stack<int> s1;
    s1.push(1);
    s1.push(2);
    Stack<int> s2(s1);
    EXPECT_EQ(s2.getSize(), 2);
    EXPECT_EQ(s2.top(), 2);
}

TEST(StackTest, Serialization) {
    Stack<int> stack;
    stack.push(1);
    stack.push(2);
    
    std::stringstream ss;
    stack.serialize(ss);
    
    Stack<int> stack2;
    stack2.deserialize(ss);
    EXPECT_EQ(stack2.getSize(), 2);
    EXPECT_EQ(stack2.top(), 2);
}

// ==============================
// HashTable Tests
// ==============================
TEST(HashTableTest, InsertAndGet) {
    HashTable<int, std::string> table;
    table.insert(1, "one");
    table.insert(2, "two");
    EXPECT_EQ(table.getSize(), 2);
    EXPECT_EQ(table.get(1), "one");
    EXPECT_EQ(table.get(2), "two");
}

TEST(HashTableTest, UpdateValue) {
    HashTable<int, std::string> table;
    table.insert(1, "old");
    table.insert(1, "new");
    EXPECT_EQ(table.getSize(), 1);
    EXPECT_EQ(table.get(1), "new");
}

TEST(HashTableTest, FindAndRemove) {
    HashTable<int, std::string> table;
    table.insert(1, "one");
    EXPECT_TRUE(table.find(1));
    EXPECT_FALSE(table.find(99));
    
    table.remove(1);
    EXPECT_FALSE(table.find(1));
    EXPECT_THROW(table.get(1), std::runtime_error);
}

TEST(HashTableTest, OperatorBracket) {
    HashTable<int, int> table;
    table[5] = 50;
    EXPECT_EQ(table[5], 50);
    table[5] = 100;
    EXPECT_EQ(table[5], 100);
}

TEST(HashTableTest, Rehashing) {
    HashTable<int, int> table;
    for (int i = 0; i < 1000; i++) {
        table.insert(i, i * 10);
    }
    EXPECT_EQ(table.getSize(), 1000);
    EXPECT_EQ(table.get(500), 5000);
}

// ==============================
// FullBinaryTree Tests
// ==============================
TEST(FullBinaryTreeTest, InsertAndFind) {
    FullBinaryTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    EXPECT_EQ(tree.getSize(), 3); // root + 2 children
    EXPECT_TRUE(tree.find(10));
    EXPECT_TRUE(tree.find(20));
    EXPECT_TRUE(tree.isFullBinaryTree());
}

TEST(FullBinaryTreeTest, Remove) {
    FullBinaryTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    size_t size_before = tree.getSize();
    tree.remove(20);
    EXPECT_LT(tree.getSize(), size_before);
    EXPECT_TRUE(tree.isFullBinaryTree());
}

TEST(FullBinaryTreeTest, CopyConstructor) {
    FullBinaryTree<int> tree1;
    tree1.insert(10);
    tree1.insert(20);
    FullBinaryTree<int> tree2(tree1);
    EXPECT_EQ(tree2.getSize(), tree1.getSize());
    EXPECT_TRUE(tree2.isFullBinaryTree());
}

TEST(FullBinaryTreeTest, Serialization) {
    FullBinaryTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    
    std::stringstream ss;
    tree.serialize(ss);
    
    FullBinaryTree<int> tree2;
    tree2.deserialize(ss);
    EXPECT_EQ(tree2.getSize(), tree.getSize());
    EXPECT_TRUE(tree2.isFullBinaryTree());
}

TEST(FullBinaryTreeTest, InvariantMaintenance) {
    FullBinaryTree<int> tree;
    for (int i = 1; i <= 10; i++) {
        tree.insert(i);
        EXPECT_TRUE(tree.isFullBinaryTree()) << "Failed at insert " << i;
    }
}

// ==============================
// File Serialization Tests
// ==============================
TEST(FileSerializationTest, ArrayFile) {
    Array<int> arr;
    arr.add(1);
    arr.add(2);
    arr.add(3);
    
    std::ofstream out("test_array.bin", std::ios::binary);
    arr.serialize(out);
    out.close();
    
    Array<int> arr2;
    std::ifstream in("test_array.bin", std::ios::binary);
    arr2.deserialize(in);
    in.close();
    
    EXPECT_EQ(arr2.getSize(), 3);
    EXPECT_EQ(arr2.get(0), 1);
    
    std::remove("test_array.bin");
}

TEST(FileSerializationTest, HashTableFile) {
    HashTable<int, int> table;
    for (int i = 0; i < 10; i++) {
        table.insert(i, i * 10);
    }
    
    std::ofstream out("test_table.bin", std::ios::binary);
    table.serialize(out);
    out.close();
    
    HashTable<int, int> table2;
    std::ifstream in("test_table.bin", std::ios::binary);
    table2.deserialize(in);
    in.close();
    
    EXPECT_EQ(table2.getSize(), 10);
    EXPECT_EQ(table2.get(5), 50);
    
    std::remove("test_table.bin");
}

// ==============================
// Main function for Google Test
// ==============================
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
