#include "../include/rbtree.hpp"
#include <iostream>


int main() {
    RBTree<int> tree1;
    RBTree<int> tree2;

    // Пример добавления узлов
    tree1.insert(10);
    tree1.insert(20);
    tree1.insert(30);

    tree2.insert(15);
    tree2.insert(25);
    // Пример вывода
    std::cout << "Tree1 in-order: ";
    tree1.inOrderPrint();
    std::cout << std::endl;

    std::cout << "Tree2 in-order: ";
    tree2.inOrderPrint();
    std::cout << std::endl;

    // Пример объединения
    RBTree<int> unionTree = tree1 + tree2;
    std::cout << "Union in-order: ";
    unionTree.inOrderPrint();
    std::cout << std::endl;
 
    // // Пример пересечения 
    // RBTree<int> intersectionTree = tree1.intersection(tree2);
    // std::cout << "Intersection in-order: ";
    // intersectionTree.inOrderPrint();
    // std::cout << std::endl;

    return 0;
}