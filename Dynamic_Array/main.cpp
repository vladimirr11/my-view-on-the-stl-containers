#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

#include "Vector.h"

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

template<typename T>
void writeVector(const Vector<T>& vec, std::ofstream& tFile) {
    tFile << "\nVector::size() = " << vec.size() << "\n" << std::endl;
        
    tFile << "--------start-printing-vector---------" << std::endl;    
    for (auto it = vec.cbegin(); it != vec.cend(); it++) {
        tFile << *it << std::endl;
    }
    tFile << "--------stop-printing-vector----------" << std::endl;
}

int main() {
    std::ofstream myVectorTestFile("VectorTests.txt", std::ofstream::out | std::ios::trunc);

    myVectorTestFile << "VECTOR CONSTRUCTION, CLEANUP, COPY, AND MOVE\n" << std::endl;

    // default Vector ctor
    {   
        myVectorTestFile << "* Default Vector ctor" << std::endl;

        Vector<Point3D> defVec;

        writeVector(defVec, myVectorTestFile);
    }

    // Vector with n default elements
    {
        myVectorTestFile << "\n* Vector ctor with 3 Point3D default elements" << std::endl;

        size_t n = 3;
        Vector<Point3D> defVec(n);

        writeVector(defVec, myVectorTestFile);
    }

    // Vector with n number of elements
    {
        myVectorTestFile << "\n* Vector ctor with 5 Point3D(10) elements" << std::endl;

        size_t n = 5;
        Point3D p3d(10);

        Vector<Point3D> vec(n, p3d);

        writeVector(vec, myVectorTestFile);
    }

    // Vector copy ctor
    {
        myVectorTestFile << "\n* Vector copy ctor with 7 Point3D(15) elements" << std::endl;

        size_t n = 7;
        Point3D p3d(15);

        Vector<Point3D> source(n, p3d);

        Vector<Point3D> vec(source);

        myVectorTestFile << "\nPrint lhs Vector" << std::endl;
        writeVector(vec, myVectorTestFile);

        myVectorTestFile << "\nPrint rhs Vector" << std::endl;
        writeVector(source, myVectorTestFile);
    }

    // Vector move ctor
    {
        myVectorTestFile << "\n* Vector move ctor with 9 Point3D(20) elements" << std::endl;

        size_t n = 9;
        Point3D p3d(20);

        Vector<Point3D> source(n, p3d);

        Vector<Point3D> vec(std::move(source));

        myVectorTestFile << "\nPrint lhs Vector" << std::endl;
        writeVector(vec, myVectorTestFile);

        myVectorTestFile << "\nPrint rhs Vector" << std::endl;
        writeVector(source, myVectorTestFile);
    }

    // Vector initializer_list ctor
    {
        myVectorTestFile << "\n* Vector initializer_list ctor with Point3D(10), Point3D(3, 4, 5)," 
        " and default Point3D elements" << std::endl;

        Point3D p3d1(10), p3d2(3, 4, 5), p3d3;

        Vector<Point3D> vec{ p3d1, p3d2, p3d3 };

        writeVector(vec, myVectorTestFile);
    }

    // Vector copy assignment operator
    {
        myVectorTestFile << "\n* Vector copy assignment operator" << std::endl;

        size_t n = 11;
        Point3D p3d(24);

        Vector<Point3D> right(n, p3d);

        Vector<Point3D> left(3);

        myVectorTestFile << "\nPrint lhs Vector with 3 default Point3D elements [before assignemt]" << std::endl;
        writeVector(left, myVectorTestFile);

        left = right;

        myVectorTestFile << "\nPrint lhs Vector after assignemt with 11 Point3D(24) elements" << std::endl;
        writeVector(left, myVectorTestFile);
    }

    // Vector move assignment operator
    {
        myVectorTestFile << "\n* Vector move assignment operator" << std::endl;

        size_t n = 14;
        Point3D p3d(28);

        Vector<Point3D> right(n, p3d);

        Vector<Point3D> left(5);

        myVectorTestFile << "\nPrint lhs Vector with 5 default Point3D elements [before move assignment]" << std::endl;
        writeVector(left, myVectorTestFile);

        left = std::move(right);

        myVectorTestFile << "\n* Print lhs Vector after move assignment with 14 Point3D(28) elements" << std::endl;
        writeVector(left, myVectorTestFile);

        myVectorTestFile << "\n* Print rhs Vector after its move" << std::endl;
        writeVector(right, myVectorTestFile);
    }

    // Vector element access
    {
        myVectorTestFile << "\nVECTOR ELEMENT ACCESS" << std::endl;

        Point3D p1(25, 35, 45), p2(500), p3, p4(9, 8, 7), p5(1, 1, 1);

        Vector<Point3D> vec;

        vec.push_back(p1);
        vec.push_back(p2);
        vec.push_back(p3);
        vec.push_back(p4);
        vec.push_back(p5);

        myVectorTestFile << "\nPrint Vector with Point3D(25, 35, 45), Point3D(500), default Point3D, Point3D(9, 8, 7),"
            " Point3D(1, 1, 1)" << std::endl;
        writeVector(vec, myVectorTestFile);

        myVectorTestFile << "\nFront element: " << vec.front() << std::endl;
        myVectorTestFile << "Back element: " << vec.back() << std::endl;
        myVectorTestFile << "Second element: " << vec[2] << std::endl;
        myVectorTestFile << "Third element: " << vec.at(3) << std::endl;
    }

    // Vector iterators
    {
        myVectorTestFile << "\nVECTOR ITERATORS" << std::endl;

        Point3D p1(25, 35, 45), p2(500), p3, p4(9, 8, 7), p5(1, 1, 1);

        Vector<Point3D> vec;

        vec.push_back(p1);
        vec.push_back(p2);
        vec.push_back(p3);
        vec.push_back(p4);
        vec.push_back(p5);

        myVectorTestFile << "\nPrint Vector with Point3D(25, 35, 45), Point3D(500), default Point3D, Point3D(9, 8, 7),"
            " Point3D(1, 1, 1)" << std::endl;
        writeVector(vec, myVectorTestFile);

        Vector<Point3D>::iterator beg = vec.begin();
        Vector<Point3D>::iterator end = vec.end();
        Vector<Point3D>::reverse_iterator rbeg = vec.rbegin();
        Vector<Point3D>::reverse_iterator rend = vec.rend();

        myVectorTestFile << "\nVector element at begin() pos: " << *beg << std::endl;
        myVectorTestFile << "Vector element at end() pos: " << *end << std::endl;

        myVectorTestFile << "Vector element at rbegin() pos: " << *rbeg << std::endl;
        myVectorTestFile << "Vector element at rend() pos: " << *rend << std::endl;

        myVectorTestFile << "\nPrinting Vector elements from begin() to end() ...\n" << std::endl;
        for (; beg != end; beg++) {
            myVectorTestFile << *beg << std::endl;
        }

        myVectorTestFile << "\nPrinting Vector elements from rbegin() to rend() ...\n" << std::endl;
        for (; rbeg != rend; rbeg++) {
            myVectorTestFile << *rbeg << std::endl;
        }

        Vector<Point3D>::const_iterator cbeg = vec.cbegin();
        Vector<Point3D>::const_iterator cend = vec.cend();
        Vector<Point3D>::const_reverse_iterator crbeg = vec.crbegin();
        Vector<Point3D>::const_reverse_iterator crend = vec.crend();

        myVectorTestFile << "\nVector element at cbegin() pos: " << *cbeg << std::endl;
        myVectorTestFile << "Vector element at cend() pos: " << *cend << std::endl;

        myVectorTestFile << "Vector element at crbegin() pos: " << *crbeg << std::endl;
        myVectorTestFile << "Vector element at crend() pos: " << *crend << std::endl;

        myVectorTestFile << "\nPrinting Vector elements from cbegin() to cend() ...\n" << std::endl;
        for (; cbeg != cend; cbeg++) {
            myVectorTestFile << *cbeg << std::endl;
        }

        myVectorTestFile << "\nPrinting Vector elements from crbegin() to crend() ...\n" << std::endl;
        for (; crbeg != crend; crbeg++) {
            myVectorTestFile << *crbeg << std::endl;
        }
    }

    // Vector capacity
    {
        myVectorTestFile << "\nVECTOR CAPACITY" << std::endl;

        Vector<Point3D> vec;

        myVectorTestFile << "\nPrint empty Vector" << std::endl;
        writeVector(vec, myVectorTestFile);

        vec.empty() ? myVectorTestFile << "\nThe Vector is empty" << std::endl : 
                      myVectorTestFile << "\nThe Vector is not empty" << std::endl;
        
        for (size_t i = 1; i < 6; i++) {
            vec.push_back(Point3D(i));
        }

        myVectorTestFile << "\nPrint Vector with Point3D(1) to Point3D(5) elements" << std::endl;
        writeVector(vec, myVectorTestFile);

        vec.empty() ? myVectorTestFile << "\nThe Vector is empty" << std::endl : 
                      myVectorTestFile << "\nThe Vector is not empty" << std::endl;
    }

    // Vector modifiers
    {   
        myVectorTestFile << "\nVECTOR MODIFIERS" << std::endl;

        Point3D p3d(10);
        
        Vector<Point3D> vec(10, p3d);

        myVectorTestFile << "\nConstruct Vector with 10 Point3D(10) elements" << std::endl;
        writeVector(vec, myVectorTestFile);

        Point3D p60(60);

        myVectorTestFile << "\nInsert 2 Point3D(60) elements at the beginning" << std::endl;
        vec.insert(vec.cbegin(), p60); // size 11
        vec.insert(vec.cbegin(), p60); // size 12
        writeVector(vec, myVectorTestFile);

        Point3D p70(70);

        myVectorTestFile << "\nInsert Point3D(70) element at the end" << std::endl;
        vec.insert(vec.cend(), p70); // size 13
        writeVector(vec, myVectorTestFile);

        Point3D p900(900);

        myVectorTestFile << "\nInsert Point3D(900) element at the beginning" << std::endl;
        vec.insert(vec.cbegin(), p900); // size 14
        writeVector(vec, myVectorTestFile);

        myVectorTestFile << "\nInsert Point3D(900) element at 2nd position" << std::endl;
        auto iter = vec.cbegin();
        std::advance(iter, 2);
        vec.insert(iter, p900); // size 15
        writeVector(vec, myVectorTestFile);

        myVectorTestFile << "\nInsert Point3D(900) element at 6th position" << std::endl;
        std::advance(iter, 4); 
        vec.insert(iter, p900); // size 16
        writeVector(vec, myVectorTestFile);
        
        Point3D p800(800);

        myVectorTestFile << "\nInsert 3 Poin3D(800) elements at the end" << std::endl;
        vec.insert(vec.cend(), p800); // size 17
        vec.insert(vec.cend(), p800); // size 18
        vec.insert(vec.cend(), p800); // size 19
        writeVector(vec, myVectorTestFile);

        Point3D p200(200);

        myVectorTestFile << "\nMove Point3D(200) element at the beginning" << std::endl;
        vec.insert(vec.cbegin(), std::move(p200)); // size() 20
        writeVector(vec, myVectorTestFile);

        myVectorTestFile << "\nInsert Point3D(200) rvalue element at the end" << std::endl;
        vec.insert(vec.cend(), Point3D(200, 200, 200)); // size() 21
        writeVector(vec, myVectorTestFile);

        Point3D p1000(1000);
        
        myVectorTestFile << "\nInsert 4 Point3D(1000) elements at the beginning" << std::endl;
        vec.insert(vec.cbegin(), 4, p1000);
        writeVector(vec, myVectorTestFile);

        myVectorTestFile << "\nInsert 2 Point3D(1000) elements at the end" << std::endl;
        vec.insert(vec.cend(), 2, p1000);
        writeVector(vec, myVectorTestFile);

        // second test Vector
        Vector<Point3D> vec2;

        myVectorTestFile << "\nConstruct 2nd empty Test Vector" << std::endl;
        writeVector(vec2, myVectorTestFile);

        myVectorTestFile << "\nInsert Point3D(1000) element at the beginning of test vec2" << std::endl;
        vec2.insert(vec2.cbegin(), p1000);
        writeVector(vec2, myVectorTestFile);

        Point3D p300(300);

        myVectorTestFile << "\nInsert 3 Point3D(300) elements at the end of test vec2" << std::endl;
        vec2.insert(vec2.cend(), 3, p300);
        writeVector(vec2, myVectorTestFile);

        myVectorTestFile << "\nEmplace Point3D(2000) element at end of test vec2" << std::endl;
        vec2.emplace(vec2.cend(), 2000);
        writeVector(vec2, myVectorTestFile);

        myVectorTestFile << "\nErase element at end() position from test vec2" << std::endl;
        auto eIter = vec2.end();
        std::advance(eIter, -1);
        vec2.erase(eIter);
        writeVector(vec2, myVectorTestFile);

        myVectorTestFile << "\nErase element at 2nd position before the end() from test vec2" << std::endl;
        std::advance(eIter, -2);
        vec2.erase(eIter);
        writeVector(vec2, myVectorTestFile);

        myVectorTestFile << "\nErase element at first position from test vec2" << std::endl;
        auto begIt = vec2.erase(vec2.cbegin());
        myVectorTestFile << *begIt << std::endl;
        writeVector(vec2, myVectorTestFile);

        myVectorTestFile << "\nTest vec1" << std::endl;
        writeVector(vec, myVectorTestFile);

        myVectorTestFile << "\nErase first 10 elements from test vec1" << std::endl;
        auto it10 = vec.cbegin();
        std::advance(it10, 10);
        vec.erase(vec.cbegin(), it10);
        writeVector(vec, myVectorTestFile);

        myVectorTestFile << "\nClear test vec1" << std::endl;
        vec.clear();
        writeVector(vec, myVectorTestFile);

        myVectorTestFile << "\nPush back from Point3D(0) to Point3D(9) to test vec1" << std::endl;
        for (int i = 0; i < 10; i++) {
            vec.push_back(Point3D(i));
        }
        writeVector(vec, myVectorTestFile);

        myVectorTestFile << "\nConstructs elements in-place from Point3D(10) to Point3D(29) at the end of test vec1" << std::endl;
        for (int i = 10; i < 30; i++) {
            vec.emplace_back(i);
        }
        writeVector(vec, myVectorTestFile);

        myVectorTestFile << "\nPop the last 13 elements from test vec1" << std::endl;
        for (int i = 0; i < 13; i++) {
            vec.pop_back();
        }
        writeVector(vec, myVectorTestFile);

        myVectorTestFile << "\nEmplace from Point3D(100) to Point3D(1000) rvalue elements at the beginning of test vec1" << std::endl;
        for (int i = 1; i < 11; i++) {
            vec.emplace(vec.cbegin(), Point3D(i * 100));
        }
        writeVector(vec, myVectorTestFile);

        myVectorTestFile << "\nResize test vec2 to 20 Point3D(20) elements" << std::endl;
        vec2.resize(20, Point3D(20));
        writeVector(vec2, myVectorTestFile);

        myVectorTestFile << "\nResize test vec1 to 10 Point3D elements" << std::endl;
        vec.resize(10, Point3D(10));
        writeVector(vec, myVectorTestFile);

        myVectorTestFile << "\nSwap test vec1 and test vec2" << std::endl;
        vec.swap(vec2);
        myVectorTestFile << "\nTest vec1" << std::endl;
        writeVector(vec, myVectorTestFile);
        myVectorTestFile << "\nTest vec2" << std::endl;
        writeVector(vec2, myVectorTestFile);
    }

    myVectorTestFile.close();

    return 0;
}
