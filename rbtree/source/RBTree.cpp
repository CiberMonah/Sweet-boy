#include "../include/rbtree.hpp"
#include <iostream>

template<typename T>
void RBTree<T>::rotateLeft(std::shared_ptr<Node>& node) {
    auto rightChild = node->right;
    node->right = rightChild->left;

    if (rightChild->left != nullptr) {
        rightChild->left->parent = node;
    }

    rightChild->parent = node->parent;

    if (node->parent == nullptr) {
        root = rightChild;
    } else if (node == node->parent->left) {
        node->parent->left = rightChild;
    } else {
        node->parent->right = rightChild;
    }

    rightChild->left = node;
    node->parent = rightChild;
}

template<typename T>
void RBTree<T>::rotateRight(std::shared_ptr<Node>& node) {
    auto leftChild = node->left;
    node->left = leftChild->right;

    if (leftChild->right != nullptr) {
        leftChild->right->parent = node;
    }

    leftChild->parent = node->parent;

    if (node->parent == nullptr) {
        root = leftChild;
    } else if (node == node->parent->left) {
        node->parent->left = leftChild;
    } else {
        node->parent->right = leftChild;
    }

    leftChild->right = node;
    node->parent = leftChild;
}

template<typename T>
void RBTree<T>::fixInsert(std::shared_ptr<Node>& node) {
    std::shared_ptr<Node> parent = nullptr;
    std::shared_ptr<Node> grandparent = nullptr;

    while (node != root && node->color == RED && node->parent->color == RED) {
        parent = node->parent;
        grandparent = parent->parent;

        if (parent == grandparent->left) {
            auto uncle = grandparent->right;

            if (uncle != nullptr && uncle->color == RED) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                node = grandparent;
            } else {
                if (node == parent->right) {
                    rotateLeft(parent);
                    node = parent;
                    parent = node->parent;
                }
                rotateRight(grandparent);
                std::swap(parent->color, grandparent->color);
                node = parent;
            }
        } else {
            auto uncle = grandparent->left;

            if (uncle != nullptr && uncle->color == RED) {
                grandparent->color = RED;
                parent->color = BLACK;
                uncle->color = BLACK;
                node = grandparent;
            } else {
                if (node == parent->left) {
                    rotateRight(parent);
                    node = parent;
                    parent = node->parent;
                }
                rotateLeft(grandparent);
                std::swap(parent->color, grandparent->color);
                node = parent;
            }
        }
    }
    root->color = BLACK;
}

template<typename T>
void RBTree<T>::insert(T data) {
    auto newNode = std::make_shared<Node>(data);
    if (root == nullptr) {
        newNode->color = BLACK;
        root = newNode;
    } else {
        std::shared_ptr<Node> current = root;
        std::shared_ptr<Node> parent = nullptr;

        while (current != nullptr) {
            parent = current;
            if (data < current->data) {
                current = current->left;
            } else if (data > current->data) {
                current = current->right;
            } else {
                return;  // Дублирования не допускаются
            }
        }

        newNode->parent = parent;
        if (data < parent->data) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }

        fixInsert(newNode);
    }
}

template<typename T>
void RBTree<T>::fixDelete(std::shared_ptr<Node>& node) {
    while (node != root && node->color == BLACK) {
        if (node == node->parent->left) {
            auto sibling = node->parent->right;

            if (sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                rotateLeft(node->parent);
                sibling = node->parent->right;
            }
            if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
                (sibling->right == nullptr || sibling->right->color == BLACK)) {
                sibling->color = RED;
                node = node->parent;
            } else {
                if (sibling->right == nullptr || sibling->right->color == BLACK) {
                    if (sibling->left != nullptr) sibling->left->color = BLACK;
                    sibling->color = RED;
                    rotateRight(sibling);
                    sibling = node->parent->right;
                }
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                if (sibling->right != nullptr) sibling->right->color = BLACK;
                rotateLeft(node->parent);
                node = root;
            }
        } else {
            auto sibling = node->parent->left;
            if (sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                rotateRight(node->parent);
                sibling = node->parent->left;
            }
            if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
                (sibling->right == nullptr || sibling->right->color == BLACK)) {
                sibling->color = RED;
                node = node->parent;
            } else {
                if (sibling->left == nullptr || sibling->left->color == BLACK) {
                    if (sibling->right != nullptr) sibling->right->color = BLACK;
                    sibling->color = RED;
                    rotateLeft(sibling);
                    sibling = node->parent->left;
                }
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                if (sibling->left != nullptr) sibling->left->color = BLACK;
                rotateRight(node->parent);
                node = root;
            }
        }
    }
    node->color = BLACK;
}

template<typename T>
void RBTree<T>::remove(T data) {
    auto node = root;
    std::shared_ptr<Node> toDelete = nullptr;
    std::shared_ptr<Node> child;
    
    while (node != nullptr) {
        if (node->data == data) {
            toDelete = node;
            break;
        }
        node = data < node->data ? node->left : node->right;
    }

    if (toDelete == nullptr) {
        return;  // Узел с данным значением не найден
    }

    auto y = toDelete;
    auto yOriginalColor = y->color;
    if (toDelete->left == nullptr) {
        child = toDelete->right;
        if (child) child->parent = toDelete->parent;
        if (toDelete->parent == nullptr) {
            root = child;
        } else if (toDelete == toDelete->parent->left) {
            toDelete->parent->left = child;
        } else {
            toDelete->parent->right = child;
        }
    } else if (toDelete->right == nullptr) {
        child = toDelete->left;
        if (child) child->parent = toDelete->parent;
        if (toDelete->parent == nullptr) {
            root = child;
        } else if (toDelete == toDelete->parent->left) {
            toDelete->parent->left = child;
        } else {
            toDelete->parent->right = child;
        }
    } else {
        y = toDelete->right;
        while (y->left != nullptr) {
            y = y->left;
        }
        yOriginalColor = y->color;
        child = y->right;
        if (child && y->parent == toDelete) {
            child->parent = y;
        } else if (y->parent) {
            y->parent->left = child;
            if (child) child->parent = y->parent;
        }
        if (toDelete->parent == nullptr) {
            root = y;
        } else if (toDelete == toDelete->parent->left) {
            toDelete->parent->left = y;
        } else {
            toDelete->parent->right = y;
        }

        y->left = toDelete->left;
        if (y->left) y->left->parent = y;
        y->right = toDelete->right;
        if (y->right) y->right->parent = y;
        y->parent = toDelete->parent;
        y->color = toDelete->color;
    }

    if (yOriginalColor == BLACK && child) {
        fixDelete(child);
    }
}


template<typename T>
void RBTree<T>::inOrderPrint() const {
    std::stack<std::shared_ptr<Node>> stack;
    std::shared_ptr<Node> current = root;
    
    while (current || !stack.empty()) {
        while (current) {
            stack.push(current);
            current = current->left;
        }
        current = stack.top();
        stack.pop();
        
        std::cout << current->data << " ";
        
        current = current->right;
    }
}

template<typename T>
void RBTree<T>::inOrderHelper(std::shared_ptr<Node> node, RBTree& result) const {
    if (node == nullptr) return;
    inOrderHelper(node->left, result);
    result.insert(node->data);
    inOrderHelper(node->right, result);
}


// Дополнительная простая функция поиск для unionHelper
// template<typename T>
// bool RBTree<T>::search(T data) const {
//     std::shared_ptr<Node> current = root;
//     while (current) {
//         if (data == current->data) return true;
//         current = (data < current->data) ? current->left : current->right;
//     }
//     return false;
// }

// template<typename T>
// void RBTree<T>::unionHelper(std::shared_ptr<Node> node, RBTree& result) const {
//     if (node == nullptr) return;
//     unionHelper(node->left, result);
//     if (result.search(node->data)) {
//         result.insert(node->data);
//     }
//     unionHelper(node->right, result);
// }

template<typename T>
RBTree<T> RBTree<T>::operator+(const RBTree& other) const {
    RBTree<T> result;
    inOrderHelper(this->root, result);
    inOrderHelper(other.root, result);
    return result;
}

// template<typename T>
// RBTree<T> RBTree<T>::intersection(const RBTree& other) const {
//     RBTree<T> result;
//     unionHelper(this->root, result);
//     return result;
// }

