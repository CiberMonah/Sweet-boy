#include <iostream>

template <typename T>
T* array_copy(T* const dst_begin, T* const dst_end, T* src_begin, T* const src_end) {
    T* dst = dst_begin;
    T* src = src_begin;

    while (dst != dst_end && src != src_end) {
        *dst = *src;
        ++dst;
        ++src;
    }

    return dst;
}

auto main() ->int  {
    const int size = 5;

    int src[size] = {1, 2, 3, 4, 5};
    int dst[size] = {0}; 

    int* result = array_copy(dst, dst + size, src, src + size);

    std::cout << "Array after copy: ";
    for (int i = 0; i < size; i++) {
        std::cout << dst[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Position of result (distance from dst_begin): "
              << (result - dst) << std::endl;

    return 0;
}
