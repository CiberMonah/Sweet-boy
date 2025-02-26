#include <iostream>
#include <cstring>
#include <algorithm>

template <typename T>
T findMax(T* arr, size_t size) {
    if (size == 0) return T();
    T maxElement = arr[0];
    for (size_t i = 1; i < size; ++i) {
        if (arr[i] > maxElement) {
            maxElement = arr[i];
        }
    }
    return maxElement;
}

template <>
const char* findMax<const char*>(const char** arr, size_t size) {
    if (size == 0) return nullptr;
    const char* longestStr = arr[0];
    for (size_t i = 1; i < size; ++i) {
        if (std::strlen(arr[i]) > std::strlen(longestStr)) {
            longestStr = arr[i];
        }
    }
    return longestStr;
}

int main() {
    int numbers[] = {3, 7, 2, 9, 5};
    size_t sizeNum = sizeof(numbers) / sizeof(numbers[0]);
    std::cout << "Max integer: " << findMax(numbers, sizeNum) << std::endl;

    const char* strings[] = {"Andrey", "Artur", "Matvey", "Egor"};
    size_t sizeStr = sizeof(strings) / sizeof(strings[0]);
    std::cout << "Longest string: " << findMax(strings, sizeStr) << std::endl;

    return 0;
}
