#include "rbtree.hpp"

template<typename T>
void RBTree<T>::rotateLeft(Node*& node) {
    Node* newParent = node->right;
    node->right = newParent->left;
    if (newParent->left != nullptr)
        newParent->left->parent = node;
    newParent->parent = node->parent;
    if (node->parent == nullptr)
        root = newParent;
    else if (node == node->parent->left)
        node->parent->left = newParent;
    else
        node->parent->right = newParent;
    newParent->left = node;
    node->parent = newParent;
}

template<typename T>
void RBTree<T>::rotateRight(Node*& node) {
    Node* newParent = node->left;
    node->left = newParent->right;
    if (newParent->right != nullptr)
        newParent->right->parent = node;
    newParent->parent = node->parent;
    if (node->parent == nullptr)
        root = newParent;
    else if (node == node->parent->right)
        node->parent->right = newParent;
    else
        node->parent->left = newParent;
    newParent->right = node;
    node->parent = newParent;
}

template<typename T>
void RBTree<T>::fixInsertion(Node*& node) {
    Node* parent = nullptr;
    Node* grandparent = nullptr;

    while (node != root && node->color != BLACK && node->parent->color == RED) {
        parent = node->parent;
        grandparent = node->parent->parent;
        
        if (parent == grandparent->left) {
            Node* uncle = grandparent->right;

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
            Node* uncle = grandparent->left;

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
void RBTree<T>::fixDeletion(Node*& node) {

    while (node != root && node->color == BLACK) {
        if (node == node->parent->left) {
            Node* sibling = node->parent->right;
            if (sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                rotateLeft(node->parent);
                sibling = node->parent->right;
            }
            if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
                sibling->color = RED;
                node = node->parent;
            } else {
                if (sibling->right->color == BLACK) {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    rotateRight(sibling);
                    sibling = node->parent->right;
                }
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->right->color = BLACK;
                rotateLeft(node->parent);
                node = root;
            }
        } else {
            Node* sibling = node->parent->left;
            if (sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                rotateRight(node->parent);
                sibling = node->parent->left;
            }
            if (sibling->right->color == BLACK && sibling->left->color == BLACK) {
                sibling->color = RED;
                node = node->parent;
            } else {
                if (sibling->left->color == BLACK) {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    rotateLeft(sibling);
                    sibling = node->parent->left;
                }
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->left->color = BLACK;
                rotateRight(node->parent);
                node = root;
            }
        }
    }
    node->color = BLACK;
}

template<typename T>
void RBTree<T>::inOrderHelper(Node* node) const {
    if (node == nullptr)
        return;
    inOrderHelper(node->left);
    std::cout << node->data << " ";
    inOrderHelper(node->right);
}

// Вставка элемента
template<typename T>
void RBTree<T>::insert(const T& data) {
    Node* node = new Node(data);
    if (root == nullptr) {
        node->color = BLACK;
        root = node;
    } else {
        Node* temp = root;
        Node* parent = nullptr;
        while (temp != nullptr) {
            parent = temp;
            if (node->data < temp->data)
                temp = temp->left;
            else if (node->data > temp->data)
                temp = temp->right;
            else
                throw std::logic_error("Duplicate data");
        }
        node->parent = parent;
        if (node->data < parent->data)
            parent->left = node;
        else
            parent->right = node;

        fixInsertion(node);
    }
}

// Удаление элемента
template<typename T>
void RBTree<T>::remove(const T& data) {
    Node* node = root;
    Node* toReplace = nullptr;
    Node* child = nullptr;

    while (node != nullptr && node->data != data) {
        if (data < node->data)
            node = node->left;
        else
            node = node->right;
    }

    if (node == nullptr)
        throw std::logic_error("Data not found");

    if (node->left == nullptr || node->right == nullptr)
        toReplace = node;
    else {
        toReplace = node->right;
        while (toReplace->left != nullptr)
            toReplace = toReplace->left;
    }

    if (toReplace->left != nullptr)
        child = toReplace->left;
    else
        child = toReplace->right;

    if (child != nullptr)
        child->parent = toReplace->parent;

    if (toReplace->parent == nullptr)
        root = child;
    else if (toReplace == toReplace->parent->left)
        toReplace->parent->left = child;
    else
        toReplace->parent->right = child;

    if (toReplace != node)
        node->data = toReplace->data;

    if (toReplace->color == BLACK && child != nullptr)
        fixDeletion(child);

    delete toReplace;
}

template<typename T>
void RBTree<T>::inOrder() const {
    inOrderHelper(root);
    std::cout << std::endl;
}

template<typename T>
bool RBTree<T>::find(const T& data) const {
    Node* current = root;
    while (current != nullptr) {
        if (data == current->data)
            return true;
        else if (data < current->data)
            current = current->left;
        else
            current = current->right;
    }
    return false;
}

template<typename T>
void RBTree<T>::clear() {
    while (root != nullptr) {
        remove(root->data);
    }
}

template<typename T>
RBTree<T>::~RBTree() {
    clear();
}

// Оператор +=
template<typename T>
RBTree<T>& RBTree<T>::operator+=(const T& value) {
    insert(value);
    return *this;
}

// Оператор +
template<typename T>
RBTree<T> RBTree<T>::operator+(const RBTree& other) const {
    RBTree<T> result(*this);
    inOrderHelper(other.root);
    return result;
}
