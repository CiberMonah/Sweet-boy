#include <iostream>

template <typename T>
struct node {
    T val;
    node* next;
};

template <typename T>
class Stack final {
private:
    node<T>* Ph;
public:
    Stack() { Ph = nullptr; }
    ~Stack(); 
    void push(T x);
    void pop();
    void print_stack();
};

template <typename T>
Stack<T>::~Stack() {
    while (Ph != nullptr) {
        pop();
    }
}

template <typename T>
void Stack<T>::push(T x) {
    node<T>* Pn = new node<T>;
    Pn->val = x;
    Pn->next = Ph;
    Ph = Pn;
}

template <typename T>
void Stack<T>::pop() {
    if (Ph == nullptr) {
        std::cout << "No elements in stack\n";
        return;
    }
    node<T>* Pt = Ph;
    Ph = Ph->next;
    delete Pt;
}

template <typename T>
void Stack<T>::print_stack() {
    node<T>* Pt = Ph;
    while (Pt != nullptr) {
        std::cout << Pt->val << std::endl;
        Pt = Pt->next;
    }
}

int main() {
    Stack<int> S;
    int i;

    std::cout << "Start stack\n";

    for (i = 1; i <= 5; i++)
        S.push(23 * i);
    S.print_stack();

    std::cout << std::endl;

    S.pop();
    S.print_stack();

    std::cout << std::endl;

    for (i = 1; i <= 4; i++)
        S.pop();
    S.print_stack();

    std::cout << std::endl;

    for (i = 1; i <= 3; i++)
        S.push(43 * i);
    S.print_stack();

    return 0;
}
