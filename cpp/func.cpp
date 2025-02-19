#include <iostream>
#include <type_traits>

template<typename T, typename Func>
T apply(Func func, T x, int N) {
    for (int i = 0; i < N; ++i) {
        x = func(x);
    }
    return x; 
}

int increment(int x) {
    return x + 1; 
}


int main() {

    int result1 = apply<int>(increment, 0, 5);
    std::cout << "Result of applying increment 5 times to 0: " << result1 << std::endl;

    return 0;
}
