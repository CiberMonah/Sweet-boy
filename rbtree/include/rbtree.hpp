#pragma once
#include <iostream>
#include <stdexcept>

template<typename T>
class RBTree {
private:
    enum Color { RED, BLACK };

    struct Node {
        T data;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(T data) : data(data), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node* root;

    void rotateLeft(Node*& node);
    void rotateRight(Node*& node);
    void fixInsertion(Node*& node);
    void fixDeletion(Node*& node);
    void inOrderHelper(Node* node) const;

public:
    RBTree() : root(nullptr) {}
    ~RBTree();

    void insert(const T& data);
    void remove(const T& data);
    void clear();
    bool find(const T& data) const;
    void inOrder() const;

    RBTree operator+(const RBTree& other) const;
    RBTree& operator+=(const T& value);
};
