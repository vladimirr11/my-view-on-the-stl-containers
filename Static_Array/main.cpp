#include <iostream>
#include <iomanip>
#include <fstream>
#include <array>

#include "Array.h"

struct Point3D {
    Point3D() : _x(0.0f), _y(0.0f), _z(0.0f) {
        _memoryBlock = new int[10];
    }

    Point3D(float scalar) : _x(scalar), _y(scalar), _z(scalar) {
        _memoryBlock = new int[10];
    }

    Point3D(float x, float y, float z) : _x(x), _y(y), _z(z) {
        _memoryBlock = new int[10];
    }

    Point3D(const Point3D& source) : _x(source._x), _y(source._y), _z(source._z) {
        _memoryBlock = new int[10];
    }

    Point3D& operator=(const Point3D& right) {
        if (this != &right) {
            _x = right._x;
            _y = right._y;
            _z = right._z;

            _memoryBlock = new int[10];
        }

        return *this;
    }

    Point3D(Point3D&& source) : _x(source._x), _y(source._y), _z(source._z) {
        _memoryBlock = source._memoryBlock;

        source._memoryBlock = nullptr;
        source._x = 0.0f;
        source._y = 0.0f;
        source._z = 0.0f;
    }

    Point3D& operator=(Point3D&& right) {
        if (this != &right) {
            _x = right._x;
            _y = right._y;
            _z = right._z;
            _memoryBlock = right._memoryBlock;
            
            right._memoryBlock = nullptr;
            right._x = 0.0f;
            right._y = 0.0f;
            right._z = 0.0f;
        }

        return *this;
    }

    ~Point3D() {
        delete[] _memoryBlock;
    }

private:
    friend std::ostream& operator<<(std::ostream& out, const Point3D& point3d);

    float _x, _y, _z;
    int* _memoryBlock = nullptr;
};

std::ostream& operator<<(std::ostream& out, const Point3D& point3d) {
    return out << "x=" << point3d._x << ", "  << "y=" << point3d._y << ", " << "z=" << point3d._z;
}

template<typename T, size_t N>
void writeArray(const Array<T, N>& arr, std::ofstream& tFile) {
    tFile << "\nArray::size() = " << arr.size() << "\n" << std::endl;
        
    tFile << "--------start-printing-array---------" << std::endl;    
    for (auto it = arr.cbegin(); it != arr.cend(); it++) {
        tFile << *it << std::endl;
    }
    tFile << "--------stop-printing-array----------" << std::endl;
}

int main() {
    std::ofstream myArrayTestFile("ArrayTests.txt", std::ofstream::out | std::ios::trunc);

    {
        myArrayTestFile << "ARRAY ELEMENT ACCESS\n" << std::endl;

        const size_t ARR_SIZE = 10;

        Array<Point3D, ARR_SIZE> arr;

        myArrayTestFile << "\nFill Array with Point3D elements from Point3D(0) to Point3D(9)" << std::endl;
        for (size_t i = 0; i < ARR_SIZE; i++) {
            arr[i] = Point3D(i);
        }
        writeArray(arr, myArrayTestFile);

        myArrayTestFile << "\nFront element: " << arr.front() << std::endl;
        myArrayTestFile << "Back element: " << arr.back() << std::endl;
        myArrayTestFile << "Second element: " << arr[2] << std::endl;
        myArrayTestFile << "Third element: " << arr.at(3) << std::endl;

        myArrayTestFile << "\nPrint first (Array::size() / 2) elements\n" << std::endl;
        for (auto it = arr.data(); it != arr.data() + (arr.size() / 2); it++) {
            myArrayTestFile << *it << std::endl;
        }
    }

    // Array iterators
    {
        myArrayTestFile << "\nARRAY ITERATORS" << std::endl;
        
        const size_t ARR_SIZE = 20;

        Array<Point3D, ARR_SIZE> arr;
        
        myArrayTestFile << "\nFill Array with Point3D elements from Point3D(0) to Point3D(9)" << std::endl;
        for (size_t i = 0; i < ARR_SIZE; i++) {
            arr[i] = Point3D(i);
        }
        writeArray(arr, myArrayTestFile);

        Array<Point3D, ARR_SIZE>::iterator beg = arr.begin();
        Array<Point3D, ARR_SIZE>::iterator end = arr.end();
        Array<Point3D, ARR_SIZE>::reverse_iterator rbeg = arr.rbegin();
        Array<Point3D, ARR_SIZE>::reverse_iterator rend = arr.rend();

        myArrayTestFile << "\nArray element at begin() pos: " << *beg << std::endl;
        myArrayTestFile << "Array element at end() pos: " << *end << std::endl;

        myArrayTestFile << "Array element at rbegin() pos: " << *rbeg << std::endl;
        myArrayTestFile << "Array element at rend() pos: " << *rend << std::endl;

        myArrayTestFile << "\nPrinting Array elements from begin() to end() ...\n" << std::endl;
        for (; beg != end; beg++) {
            myArrayTestFile << *beg << std::endl;
        }

        myArrayTestFile << "\nPrinting Array elements from rbegin() to rend() ...\n" << std::endl;
        for (; rbeg != rend; rbeg++) {
            myArrayTestFile << *rbeg << std::endl;
        }

        Array<Point3D, ARR_SIZE>::const_iterator cbeg = arr.cbegin();
        Array<Point3D, ARR_SIZE>::const_iterator cend = arr.cend();
        Array<Point3D, ARR_SIZE>::const_reverse_iterator crbeg = arr.crbegin();
        Array<Point3D, ARR_SIZE>::const_reverse_iterator crend = arr.crend();

        myArrayTestFile << "\nArray element at cbegin() pos: " << *cbeg << std::endl;
        myArrayTestFile << "Array element at cend() pos: " << *cend << std::endl;

        myArrayTestFile << "Array element at crbegin() pos: " << *crbeg << std::endl;
        myArrayTestFile << "Array element at crend() pos: " << *crend << std::endl;

        myArrayTestFile << "\nPrinting Array elements from cbegin() to cend() ...\n" << std::endl;
        for (; cbeg != cend; cbeg++) {
            myArrayTestFile << *cbeg << std::endl;
        }

        myArrayTestFile << "\nPrinting Array elements from crbegin() to crend() ...\n" << std::endl;
        for (; crbeg != crend; crbeg++) {
            myArrayTestFile << *crbeg << std::endl;
        }
    }

    // Array capacity and operations
    {
        myArrayTestFile << "\nARRAY CAPACITY AND OPERATIONS" << std::endl;

        // test arr1
        const size_t ARR_SIZE = 10;
        Array<Point3D, ARR_SIZE> arr;

        myArrayTestFile << "\nPrint Array with 10 Point3D default elements" << std::endl;
        writeArray(arr, myArrayTestFile);

        arr.empty() ? myArrayTestFile << "\nThe Array is empty" << std::endl : 
                      myArrayTestFile << "\nThe Array is not empty" << std::endl;
        
        // test arr2
        const size_t NEW_ARR_SIZE = 10;
        Array<Point3D, NEW_ARR_SIZE> arr2;

        myArrayTestFile << "\nPrint Array with 10 Point3D(10) elements" << std::endl;
        arr2.fill(Point3D(10));
        writeArray(arr2, myArrayTestFile);

        arr2.empty() ? myArrayTestFile << "\nThe Array is empty" << std::endl : 
                      myArrayTestFile << "\nThe Array is not empty" << std::endl;

        myArrayTestFile << "\nSwap test arr1 and test arr2" << std::endl;
        arr.swap(arr2);
        myArrayTestFile << "\nTest arr1" << std::endl;
        writeArray(arr, myArrayTestFile);
        myArrayTestFile << "\nTest arr2" << std::endl;
        writeArray(arr2, myArrayTestFile);
    }

    return 0;
}
