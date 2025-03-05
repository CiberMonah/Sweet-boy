#include "../include/rbtree.hpp"
#include <iostream>


template class RBTree<int>; 

int main() {
    RBTree<int> tree1;
    tree1.insert(10);
    tree1.insert(20);
    tree1.insert(30);

    std::cout << "Tree 1 (in-order): ";
    for (const auto& item : tree1.inOrder()) {
        std::cout << item << " ";
    }
    std::cout << std::endl;

    // Удаляем элемент из первого дерева
    tree1.remove(20);
    std::cout << "Tree 1 after removing 20: ";
    for (const auto& item : tree1.inOrder()) {
        std::cout << item << " ";
    }
    std::cout << std::endl;

    // Создаем второе дерево
    RBTree<int> tree2;
    tree2.insert(20);
    tree2.insert(30);
    tree2.insert(40);

    std::cout << "Tree 2 (in-order): ";
    for (const auto& item : tree2.inOrder()) {
        std::cout << item << " ";
    }
    std::cout << std::endl;

    // Объединяем деревья (оператор +)
    auto unionTree = tree1 + tree2;
    std::cout << "Union of Tree 1 and Tree 2: ";
    for (const auto& item : unionTree.inOrder()) {
        std::cout << item << " ";
    }
    std::cout << std::endl;

    // Находим пересечение деревьев (оператор &)
    auto intersectionTree = tree1 & tree2;
    std::cout << "Intersection of Tree 1 and Tree 2: ";
    for (const auto& item : intersectionTree.inOrder()) {
        std::cout << item << " ";
    }
    std::cout << std::endl;

    return 0;
}