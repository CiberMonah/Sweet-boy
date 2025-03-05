#include <gtest/gtest.h>
#include "../include/rbtree.hpp"

TEST(RBTREETEST, INSERT_SINGLE_ELEMENT) {
    RBTree<int> tree;
    tree.insert(10);

    auto result = tree.inOrder();
    std::vector<int> expected = {10};

    EXPECT_EQ(result, expected);
}

TEST(RBTREETEST, INSERT_MULTIPLE_ELEMENTS) {
    RBTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    auto result = tree.inOrder();
    std::vector<int> expected = {10, 20, 30};

    EXPECT_EQ(result, expected);
}

TEST(RBTREETEST, REMOVE_ELEMENT) {
    RBTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    tree.remove(20);

    auto result = tree.inOrder();
    std::vector<int> expected = {10, 30};

    EXPECT_EQ(result, expected);
}


TEST(RBTREETEST, UNION_OF_TREES) {
    RBTree<int> tree1;
    tree1.insert(10);
    tree1.insert(20);

    RBTree<int> tree2;
    tree2.insert(30);
    tree2.insert(40);

    auto result = (tree1 + tree2).inOrder();
    std::vector<int> expected = {10, 20, 30, 40};

    EXPECT_EQ(result, expected);
}

TEST(RBTREETEST, INTERSECTION_OF_TREES) {
    RBTree<int> tree1;
    tree1.insert(10);
    tree1.insert(20);
    tree1.insert(30);

    RBTree<int> tree2;
    tree2.insert(20);
    tree2.insert(30);
    tree2.insert(40);

    auto result = (tree1 & tree2).inOrder();
    std::vector<int> expected = {20, 30};

    EXPECT_EQ(result, expected);
}

TEST(RBTREETEST, EMPTY_TREE) {
    RBTree<int> tree;

    auto result = tree.inOrder();
    std::vector<int> expected = {};

    EXPECT_EQ(result, expected);
}

TEST(RBTREETEST, REMOVE_NON_EXISTENT_ELEMENT) {
    RBTree<int> tree;
    tree.insert(10);
    tree.insert(20);

    tree.remove(30); // Попытка удалить несуществующий элемент

    auto result = tree.inOrder();
    std::vector<int> expected = {10, 20};

    EXPECT_EQ(result, expected);
}

TEST(RBTREETEST, INSERT_DUPLICATES) {
    RBTree<int> tree;
    tree.insert(10);
    tree.insert(10);
    
    auto result = tree.inOrder();
    std::vector<int> expected = {10, 10};

    EXPECT_EQ(result, expected);
}

TEST(RBTREETEST, REMOVE_ROOT_ELEMENT) {
    RBTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(5);

    tree.remove(10); // Удаляем корень

    auto result = tree.inOrder();
    std::vector<int> expected = {5, 20};

    EXPECT_EQ(result, expected);
}

TEST(RBTREETEST, REMOVE_ALL_ELEMENTS) {
    RBTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    tree.remove(10);
    tree.remove(20);
    tree.remove(30);

    auto result = tree.inOrder();
    std::vector<int> expected = {};

    EXPECT_EQ(result, expected);
}