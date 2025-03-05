#pragma once
#include <memory>
#include <vector>

enum class Color { RED, BLACK };

template <typename T>
struct Node {
    T data;
    Color color;
    std::shared_ptr<Node<T>> left, right, parent;

    Node(T data)
        : data(data), color(Color::RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

template <typename T>
class RBTree {
public:
    RBTree() : root(nullptr) {}

    void insert(const T& data);
    void remove(const T& data);

    RBTree<T> operator+(const RBTree<T>& other) const;
    RBTree<T> operator&(const RBTree<T>& other) const;

    std::vector<T> inOrder() const;

private:
    std::shared_ptr<Node<T>> root;

    void insert(std::shared_ptr<Node<T>> node);
    void remove(std::shared_ptr<Node<T>> node);
    std::shared_ptr<Node<T>> find(const T& data) const;

    void inOrder(std::shared_ptr<Node<T>> node, std::vector<T>& result) const;

    void fixInsert(std::shared_ptr<Node<T>> node);
    void fixRemove(std::shared_ptr<Node<T>> node);

    void rotateLeft(std::shared_ptr<Node<T>> node);
    void rotateRight(std::shared_ptr<Node<T>> node);

    std::shared_ptr<Node<T>> minimum(std::shared_ptr<Node<T>> node) const;
    void transplant(std::shared_ptr<Node<T>> u, std::shared_ptr<Node<T>> v);
};
