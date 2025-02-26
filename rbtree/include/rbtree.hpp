#include <iostream>
#include <memory>
#include <stack>

template<typename T>
class RBTree {
private:
    enum Color { RED, BLACK };
    
    struct Node {
        T data;
        Color color;
        std::shared_ptr<Node> left, right, parent;
        
        Node(T data) : data(data), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    std::shared_ptr<Node> root;

    void rotateLeft(std::shared_ptr<Node>& node);
    void rotateRight(std::shared_ptr<Node>& node);
    void fixInsert(std::shared_ptr<Node>& node);
    void fixDelete(std::shared_ptr<Node>& node);
    void inOrderHelper(std::shared_ptr<Node> node, RBTree& result) const;
    void unionHelper(std::shared_ptr<Node> node, RBTree& result) const;
    bool search(T data);
    
public:
    RBTree() : root(nullptr) {}
    
    void insert(T data);
    void remove(T data);
    void inOrderPrint() const;
    
    // Operators
    RBTree operator+(const RBTree& other) const;
    RBTree intersection(const RBTree& other) const;
};
