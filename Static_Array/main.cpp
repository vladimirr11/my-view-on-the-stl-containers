#include <iostream>
#include <array>

#include "Array.h"

constexpr int ARR_SIZE = 10;

template<typename T, size_t N>
void printArray(const Array<T, N>& arr) {
    for (auto it = arr.cbegin(); it != arr.cend(); it++) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "\n---------Array<Type, Size>----------\n" << std::endl;

    Array<int, ARR_SIZE> arr;

    // element access and capacity
    {
        for (size_t i = 0 ; i < ARR_SIZE; i++) {
            arr[i] = i * i;
            std::cout << arr[i] << " ";
        }

        std::cout << "\nArray::size = " << arr.size() << std::endl;

        std::cout << "\nElement at index 3: " << arr.at(3) << std::endl;
        std::cout << "\nElement at index 1: " << arr[1] << std::endl;

        std::cout << "\nFront element: " << arr.front() << std::endl;
        std::cout << "\nBack element: " << arr.back() << std::endl;
    }

    // iterators
    {   
        auto begIt = arr.begin();
        auto endIt = arr.end();

        std::cout << "\nForward iteration: ";
        for (; begIt != endIt; begIt++) {
            std::cout << *begIt << " ";
        }

        std::cout << std::endl;

        auto rbeg = arr.rbegin();
        auto rend = arr.rend();

        std::cout << "Reverse iteration: ";
        for (; rbeg != rend; rbeg++) {
            std::cout << *rbeg << " ";
        }

        std::cout << std::endl;
    }

    Array<int, ARR_SIZE> arr2;

    // operations
    {
        arr2.fill(ARR_SIZE);

        printArray(arr2);

        arr2.swap(arr);

        printArray(arr2);
    }

    return 0;
}
