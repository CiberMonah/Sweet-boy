#include "rbtree.hpp"
#include <iostream>

template <typename T>
void RBTree<T>::insert(const T& data) {
    auto node = std::make_shared<Node<T>>(data);
    insert(node);
}

template <typename T>
void RBTree<T>::insert(std::shared_ptr<Node<T>> node) {
    std::shared_ptr<Node<T>> y = nullptr;
    auto x = root;

    while (x) {
        y = x;
        if (node->data < x->data) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    node->parent = y;
    if (!y) {
        root = node;
    } else if (node->data < y->data) {
        y->left = node;
    } else {
        y->right = node;
    }

    node->color = Color::RED;
    fixInsert(node);
}

template <typename T>
void RBTree<T>::fixInsert(std::shared_ptr<Node<T>> node) {
    while (node->parent && node->parent->color == Color::RED) {
        if (node->parent == node->parent->parent->left) {
            auto y = node->parent->parent->right;
            if (y && y->color == Color::RED) {
                node->parent->color = Color::BLACK;
                y->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    rotateLeft(node);
                }
                node->parent->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                rotateRight(node->parent->parent);
            }
        } else {
            auto y = node->parent->parent->left;
            if (y && y->color == Color::RED) {
                node->parent->color = Color::BLACK;
                y->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rotateRight(node);
                }
                node->parent->color = Color::BLACK;
                node->parent->parent->color = Color::RED;
                rotateLeft(node->parent->parent);
            }
        }
    }
    root->color = Color::BLACK;
}

template <typename T>
void RBTree<T>::rotateLeft(std::shared_ptr<Node<T>> node) {
    auto y = node->right;
    node->right = y->left;
    if (y->left) {
        y->left->parent = node;
    }
    y->parent = node->parent;
    if (!node->parent) {
        root = y;
    } else if (node == node->parent->left) {
        node->parent->left = y;
    } else {
        node->parent->right = y;
    }
    y->left = node;
    node->parent = y;
}

template <typename T>
void RBTree<T>::rotateRight(std::shared_ptr<Node<T>> node) {
    auto y = node->left;
    node->left = y->right;
    if (y->right) {
        y->right->parent = node;
    }
    y->parent = node->parent;
    if (!node->parent) {
        root = y;
    } else if (node == node->parent->right) {
        node->parent->right = y;
    } else {
        node->parent->left = y;
    }
    y->right = node;
    node->parent = y;
}

template <typename T>
void RBTree<T>::remove(const T& data) {
    auto node = find(data);
    if (node) {
        remove(node);
    }
}

template <typename T>
void RBTree<T>::remove(std::shared_ptr<Node<T>> node) {
    auto y = node;
    auto yOriginalColor = y->color;
    std::shared_ptr<Node<T>> x;

    if (!node->left) {
        x = node->right;
        transplant(node, node->right);
    } else if (!node->right) {
        x = node->left;
        transplant(node, node->left);
    } else {
        y = minimum(node->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == node) {
            if (x) x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = node->right;
            y->right->parent = y;
        }
        transplant(node, y);
        y->left = node->left;
        y->left->parent = y;
        y->color = node->color;
    }

    if (yOriginalColor == Color::BLACK) {
        fixRemove(x);
    }
}

template <typename T>
void RBTree<T>::fixRemove(std::shared_ptr<Node<T>> node) {
    while (node != root && (!node || node->color == Color::BLACK)) {
        if (node == node->parent->left) {
            auto w = node->parent->right;
            if (w->color == Color::RED) {
                w->color = Color::BLACK;
                node->parent->color = Color::RED;
                rotateLeft(node->parent);
                w = node->parent->right;
            }
            if ((!w->left || w->left->color == Color::BLACK) &&
                (!w->right || w->right->color == Color::BLACK)) {
                w->color = Color::RED;
                node = node->parent;
            } else {
                if (!w->right || w->right->color == Color::BLACK) {
                    if (w->left) w->left->color = Color::BLACK;
                    w->color = Color::RED;
                    rotateRight(w);
                    w = node->parent->right;
                }
                w->color = node->parent->color;
                node->parent->color = Color::BLACK;
                if (w->right) w->right->color = Color::BLACK;
                rotateLeft(node->parent);
                node = root;
            }
        } else {
            auto w = node->parent->left;
            if (w->color == Color::RED) {
                w->color = Color::BLACK;
                node->parent->color = Color::RED;
                rotateRight(node->parent);
                w = node->parent->left;
            }
            if ((!w->right || w->right->color == Color::BLACK) &&
                (!w->left || w->left->color == Color::BLACK)) {
                w->color = Color::RED;
                node = node->parent;
            } else {
                if (!w->left || w->left->color == Color::BLACK) {
                    if (w->right) w->right->color = Color::BLACK;
                    w->color = Color::RED;
                    rotateLeft(w);
                    w = node->parent->left;
                }
                w->color = node->parent->color;
                node->parent->color = Color::BLACK;
                if (w->left) w->left->color = Color::BLACK;
                rotateRight(node->parent);
                node = root;
            }
        }
    }
    if (node) node->color = Color::BLACK;
}

template <typename T>
std::shared_ptr<Node<T>> RBTree<T>::find(const T& data) const {
    auto current = root;
    while (current) {
        if (data == current->data) {
            return current;
        } else if (data < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return nullptr;
}

template <typename T>
std::vector<T> RBTree<T>::inOrder() const {
    std::vector<T> result;
    inOrder(root, result);
    return result;
}

template <typename T>
void RBTree<T>::inOrder(std::shared_ptr<Node<T>> node, std::vector<T>& result) const {
    if (node) {
        inOrder(node->left, result);
        result.push_back(node->data);
        inOrder(node->right, result);
    }
}

template <typename T>
RBTree<T> RBTree<T>::operator+(const RBTree<T>& other) const {
    RBTree<T> result = *this;
    for (const auto& item : other.inOrder()) {
        result.insert(item);
    }
    return result;
}

template <typename T>
RBTree<T> RBTree<T>::operator&(const RBTree<T>& other) const {
    RBTree<T> result;
    for (const auto& item : inOrder()) {
        if (other.find(item)) {
            result.insert(item);
        }
    }
    return result;
}

template <typename T>
std::shared_ptr<Node<T>> RBTree<T>::minimum(std::shared_ptr<Node<T>> node) const {
    while (node->left) {
        node = node->left;
    }
    return node;
}

template <typename T>
void RBTree<T>::transplant(std::shared_ptr<Node<T>> u, std::shared_ptr<Node<T>> v) {
    if (!u->parent) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v) {
        v->parent = u->parent;
    }
}

template class RBTree<int>; 