#include <iostream>
#include <iomanip>
#include <fstream>
#include <deque>


#include "Deque.h"

struct Point3D {
    Point3D() : _x(0.0f), _y(0.0f), _z(0.0f) {
        _memoryBlock = new int[10]{};
    }

    Point3D(float scalar) : _x(scalar), _y(scalar), _z(scalar) {
        _memoryBlock = new int[10]{};
    }

    Point3D(float x, float y, float z) : _x(x), _y(y), _z(z) {
        _memoryBlock = new int[10]{};
    }

    Point3D(const Point3D& source) : _x(source._x), _y(source._y), _z(source._z) {
        _memoryBlock = new int[10]{};
    }

    Point3D& operator=(const Point3D& right) {
        if (this != &right) {
            _x = right._x;
            _y = right._y;
            _z = right._z;

            _memoryBlock = new int[10]{};
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
void writeDeque(const Deque<T>& deq, std::ofstream& tFile) {
    tFile << "\nDeque::size() = " << deq.size() << "\n" << std::endl;
        
    tFile << "--------start-printing-deque---------" << std::endl;    
    for (auto it = deq.cbegin(); it != deq.cend(); it++) {
        tFile << *it << std::endl;
    }
    tFile << "--------stop-printing-deque----------" << std::endl;
}

int main() {
    std::ofstream myDequeTestFile("DequeTests.txt", std::ofstream::out | std::ios::trunc);

    myDequeTestFile << "DEQUE CONSTRUCTION, CLEANUP, COPY, AND MOVE\n" << std::endl;

    // default Deque ctor
    {   
        myDequeTestFile << "* Default Deque ctor" << std::endl;

        Deque<Point3D> defDeq;

        writeDeque(defDeq, myDequeTestFile);
    }

    // Deque with n default elements
    {
        myDequeTestFile << "\n* Deque ctor with 18 Point3D default elements" << std::endl;

        size_t n = 18;
        Deque<Point3D> defDeq(n);

        writeDeque(defDeq, myDequeTestFile);
    }

    // Deque with n number of elements
    {
        myDequeTestFile << "\n* Deque ctor with 5 Point3D(10) elements" << std::endl;

        size_t n = 5;
        Point3D p3d(10);

        Deque<Point3D> deq(n, p3d);

        writeDeque(deq, myDequeTestFile);
    }

    // Deque copy ctor
    {
        myDequeTestFile << "\n* Deque copy ctor with 7 Point3D(15) elements" << std::endl;

        size_t n = 7;
        Point3D p3d(15);

        Deque<Point3D> source(n, p3d);

        Deque<Point3D> deq(source);

        myDequeTestFile << "\nPrint lhs Deque" << std::endl;
        writeDeque(deq, myDequeTestFile);

        myDequeTestFile << "\nPrint rhs Deque" << std::endl;
        writeDeque(source, myDequeTestFile);
    }

    // Deque move ctor
    {
        myDequeTestFile << "\n* Deque move ctor with 9 Point3D(20) elements" << std::endl;

        size_t n = 9;
        Point3D p3d(20);

        Deque<Point3D> source(n, p3d);

        Deque<Point3D> deq(std::move(source));

        myDequeTestFile << "\nPrint lhs Deque" << std::endl;
        writeDeque(deq, myDequeTestFile);

        myDequeTestFile << "\nPrint rhs Deque" << std::endl;
        writeDeque(source, myDequeTestFile);
    }

    // Deque initializer_list ctor
    {
        myDequeTestFile << "\n* Deque initializer_list ctor with Point3D(10), Point3D(3, 4, 5)," 
        " and default Point3D elements" << std::endl;

        Point3D p3d1(10), p3d2(3, 4, 5), p3d3;

        Deque<Point3D> deq{ p3d1, p3d2, p3d3, p3d1, p3d2, p3d3, p3d1, p3d2, p3d3, p3d1, p3d2, p3d3, 
                            p3d1, p3d2, p3d3, p3d1, p3d2, p3d3, p3d1, p3d2, p3d3, p3d1, p3d2, p3d3,
                            p3d1, p3d2, p3d3, p3d1, p3d2, p3d3, p3d1, p3d2, p3d3, p3d1, p3d2, p3d3,
                            p3d1, p3d2, p3d3, p3d1, p3d2, p3d3, p3d1, p3d2, p3d3, p3d1, p3d2, p3d3 };

        writeDeque(deq, myDequeTestFile);
    }

    // Deque copy assignment operator
    {
        myDequeTestFile << "\n* Deque copy assignment operator" << std::endl;

        size_t n = 11;
        Point3D p3d(24);

        Deque<Point3D> right(n, p3d);

        Deque<Point3D> left(3);

        myDequeTestFile << "\nPrint lhs Deque with 3 default Point3D elements [before assignemt]" << std::endl;
        writeDeque(left, myDequeTestFile);

        left = right;

        myDequeTestFile << "\nPrint lhs Deque after assignemt with 11 Point3D(24) elements" << std::endl;
        writeDeque(left, myDequeTestFile);
    }

    // Deque move assignment operator
    {
        myDequeTestFile << "\n* Deque move assignment operator" << std::endl;

        size_t n = 14;
        Point3D p3d(28);

        Deque<Point3D> right(n, p3d);

        Deque<Point3D> left(5);

        myDequeTestFile << "\nPrint lhs Deque with 5 default Point3D elements [before move assignment]" << std::endl;
        writeDeque(left, myDequeTestFile);

        left = std::move(right);

        myDequeTestFile << "\n* Print lhs Deque after move assignment with 14 Point3D(28) elements" << std::endl;
        writeDeque(left, myDequeTestFile);

        myDequeTestFile << "\n* Print rhs Deque after its move" << std::endl;
        writeDeque(right, myDequeTestFile);
    }

    // Deque element access
    {
        myDequeTestFile << "\nDEQUE ELEMENT ACCESS" << std::endl;

        Point3D p1(25, 35, 45), p2(500), p3, p4(9, 8, 7), p5(1, 1, 1);

        Deque<Point3D> deq;

        deq.push_back(p1);
        deq.push_back(p2);
        deq.push_back(p3);
        deq.push_back(p4);
        deq.push_back(p5);

        myDequeTestFile << "\nPrint Deque with Point3D(25, 35, 45), Point3D(500), default Point3D, Point3D(9, 8, 7),"
            " Point3D(1, 1, 1)" << std::endl;
        writeDeque(deq, myDequeTestFile);

        myDequeTestFile << "\nFront element: " << deq.front() << std::endl;
        myDequeTestFile << "Back element: " << deq.back() << std::endl;
        myDequeTestFile << "Second element: " << deq[2] << std::endl;
        myDequeTestFile << "Third element: " << deq.at(3) << std::endl;
    }

    // Deque iterators
    {
        myDequeTestFile << "\nDEQUE ITERATORS" << std::endl;

        Point3D p1(25, 35, 45), p2(500), p3, p4(9, 8, 7), p5(1, 1, 1);

        Deque<Point3D> deq;

        deq.push_back(p1);
        deq.push_back(p2);
        deq.push_back(p3);
        deq.push_back(p4);
        deq.push_back(p5);  

        myDequeTestFile << "\nPrint Deque with Point3D(25, 35, 45), Point3D(500), default Point3D, Point3D(9, 8, 7),"
            " Point3D(1, 1, 1)" << std::endl;
        writeDeque(deq, myDequeTestFile);

        Deque<Point3D>::iterator beg = deq.begin();
        Deque<Point3D>::iterator end = deq.end();
        Deque<Point3D>::reverse_iterator rbeg = deq.rbegin();
        Deque<Point3D>::reverse_iterator rend = deq.rend();

        myDequeTestFile << "\nDeque element at begin() pos: " << *beg << std::endl;
        myDequeTestFile << "Deque element at end() pos: " << *end << std::endl;

        myDequeTestFile << "Deque element at rbegin() pos: " << *rbeg << std::endl;
        // myDequeTestFile << "Deque element at rend() pos: " << *rend << std::endl; // possible bug 

        myDequeTestFile << "\nPrinting Deque elements from begin() to end() ...\n" << std::endl;
        for (; beg != end; beg++) {
            myDequeTestFile << *beg << std::endl;
        }

        myDequeTestFile << "\nPrinting Deque elements from rbegin() to rend() ...\n" << std::endl;
        for (; rbeg != rend; rbeg++) {
            myDequeTestFile << *rbeg << std::endl;
        }

        Deque<Point3D>::const_iterator cbeg = deq.cbegin();
        Deque<Point3D>::const_iterator cend = deq.cend();
        Deque<Point3D>::const_reverse_iterator crbeg = deq.crbegin();
        Deque<Point3D>::const_reverse_iterator crend = deq.crend();

        myDequeTestFile << "\nDeque element at cbegin() pos: " << *cbeg << std::endl;
        myDequeTestFile << "Deque element at cend() pos: " << *cend << std::endl;

        myDequeTestFile << "Deque element at crbegin() pos: " << *crbeg << std::endl;
        // myDequeTestFile << "Deque element at crend() pos: " << *crend << std::endl;

        myDequeTestFile << "\nPrinting Deque elements from cbegin() to cend() ...\n" << std::endl;
        for (; cbeg != cend; cbeg++) {
            myDequeTestFile << *cbeg << std::endl;
        }

        myDequeTestFile << "\nPrinting Deque elements from crbegin() to crend() ...\n" << std::endl;
        for (; crbeg != crend; crbeg++) {
            myDequeTestFile << *crbeg << std::endl;
        }
    } 

    // Deque capacity
    {
        myDequeTestFile << "\nDEQUE CAPACITY" << std::endl;

        Deque<Point3D> deq;

        myDequeTestFile << "\nPrint empty Deque" << std::endl;
        writeDeque(deq, myDequeTestFile);

        deq.empty() ? myDequeTestFile << "\nThe Deque is empty" << std::endl : 
                      myDequeTestFile << "\nThe Deque is not empty" << std::endl;
        
        for (size_t i = 1; i < 6; i++) {
            deq.push_back(Point3D(i));
        }

        myDequeTestFile << "\nPrint Deque with Point3D(1) to Point3D(5) elements" << std::endl;
        writeDeque(deq, myDequeTestFile);

        deq.empty() ? myDequeTestFile << "\nThe Deque is empty" << std::endl : 
                      myDequeTestFile << "\nThe Deque is not empty" << std::endl;
    }

    // Deque modifiers
    {   
        myDequeTestFile << "\nDEQUE MODIFIERS" << std::endl;

        Point3D p3d(10);
        
        Deque<Point3D> deq(10, p3d);

        myDequeTestFile << "\nConstruct Deque with 10 Point3D(10) elements" << std::endl;
        writeDeque(deq, myDequeTestFile);

        Point3D p60(60);

        myDequeTestFile << "\nInsert 2 Point3D(60) elements at the beginning" << std::endl;
        deq.insert(deq.cbegin(), p60); // size 11
        deq.insert(deq.cbegin(), p60); // size 12
        writeDeque(deq, myDequeTestFile);

        Point3D p70(70);

        myDequeTestFile << "\nInsert Point3D(70) element at the end" << std::endl;
        deq.insert(deq.cend(), p70); // size 13
        writeDeque(deq, myDequeTestFile);

        Point3D p900(900);

        myDequeTestFile << "\nInsert Point3D(900) element at the beginning" << std::endl;
        deq.insert(deq.cbegin(), p900); // size 14
        writeDeque(deq, myDequeTestFile);

        myDequeTestFile << "\nInsert Point3D(900) element at 2nd position" << std::endl;
        auto iter = deq.cbegin();
        std::advance(iter, 2);
        deq.insert(iter, p900); // size 15
        writeDeque(deq, myDequeTestFile);

        myDequeTestFile << "\nInsert Point3D(900) element at 6th position" << std::endl;
        iter = deq.cbegin();
        std::advance(iter, 6); 
        deq.insert(iter, p900); // size 16
        writeDeque(deq, myDequeTestFile);
        
        Point3D p800(800);

        myDequeTestFile << "\nInsert 3 Poin3D(800) elements at the end" << std::endl;
        deq.insert(deq.cend(), p800); // size 17
        deq.insert(deq.cend(), p800); // size 18
        deq.insert(deq.cend(), p800); // size 19
        writeDeque(deq, myDequeTestFile);

        Point3D p200(200);

        myDequeTestFile << "\nMove Point3D(200) element at the beginning" << std::endl;
        deq.insert(deq.cbegin(), std::move(p200)); // size() 20
        writeDeque(deq, myDequeTestFile);

        myDequeTestFile << "\nInsert Point3D(200) rvalue element at the end" << std::endl;
        deq.insert(deq.cend(), Point3D(200, 200, 200)); // size() 21
        writeDeque(deq, myDequeTestFile);

        Point3D p1000(1000);
        
        myDequeTestFile << "\nInsert 4 Point3D(1000) elements at the beginning" << std::endl;
        deq.insert(deq.cbegin(), 4, p1000);
        writeDeque(deq, myDequeTestFile);

        myDequeTestFile << "\nInsert 2 Point3D(1000) elements at the end" << std::endl;
        deq.insert(deq.cend(), 2, p1000);
        writeDeque(deq, myDequeTestFile);

        // second test Deque
        Deque<Point3D> deq2;

        myDequeTestFile << "\nConstruct 2nd empty Test Deque" << std::endl;
        writeDeque(deq2, myDequeTestFile);

        myDequeTestFile << "\nInsert Point3D(1000) element at the beginning of test deq2" << std::endl;
        deq2.insert(deq2.cbegin(), p1000);
        writeDeque(deq2, myDequeTestFile);

        Point3D p300(300);

        myDequeTestFile << "\nInsert 3 Point3D(300) elements at the end of test deq2" << std::endl;
        deq2.insert(deq2.cend(), 3, p300);
        writeDeque(deq2, myDequeTestFile);

        myDequeTestFile << "\nEmplace Point3D(2000) element at end of test deq2" << std::endl;
        deq2.emplace(deq2.cend(), 2000);
        writeDeque(deq2, myDequeTestFile);

        Point3D p7000(7000);

        myDequeTestFile << "\nInsert 5 Point3D(7000) elements at 3rd position of test deq2" << std::endl;
        auto iter2 = deq2.cbegin();
        std::advance(iter2, 3);
        deq2.insert(iter2, 5, p7000);
        writeDeque(deq2, myDequeTestFile);

        Point3D p40(40);

        myDequeTestFile << "\nInsert 2 Point3D(40) elements at 2st position of test deq2" << std::endl;
        iter2 = deq2.cbegin();
        std::advance(iter2, 2);
        deq2.insert(iter2, 2, p40);
        writeDeque(deq2, myDequeTestFile);

        myDequeTestFile << "\nErase element at end() position from test deq2" << std::endl;
        auto eIter = deq2.cend();
        std::advance(eIter, -1);
        deq2.erase(eIter);
        writeDeque(deq2, myDequeTestFile);

        myDequeTestFile << "\nErase element 7 positions before the end() from test deq2" << std::endl;
        eIter = deq2.cend();
        std::advance(eIter, -7);
        deq2.erase(eIter);
        writeDeque(deq2, myDequeTestFile);

        myDequeTestFile << "\nErase element at first position from test deq2" << std::endl;
        auto begIt = deq2.erase(deq2.cbegin());
        myDequeTestFile << *begIt << std::endl;
        writeDeque(deq2, myDequeTestFile);

        myDequeTestFile << "\nTest deq1" << std::endl;
        writeDeque(deq, myDequeTestFile);

        myDequeTestFile << "\nErase first 10 elements from test deq1" << std::endl;
        auto it10 = deq.cbegin();
        std::advance(it10, 10);
        deq.erase(deq.cbegin(), it10);
        writeDeque(deq, myDequeTestFile);

        myDequeTestFile << "\nErase 10th to 15th element from test deq1" << std::endl;
        auto itFirst = deq.cbegin();
        auto itSecond = deq.cbegin();
        std::advance(itFirst, 10);
        std::advance(itSecond, 15);
        deq.erase(itFirst, itSecond);
        writeDeque(deq, myDequeTestFile);

        myDequeTestFile << "\nClear test deq1" << std::endl;
        deq.clear();
        writeDeque(deq, myDequeTestFile);

        myDequeTestFile << "\nPush back from Point3D(0) to Point3D(9) to test deq1" << std::endl;
        for (int i = 0; i < 10; i++) {
            deq.push_back(Point3D(i));
        }
        writeDeque(deq, myDequeTestFile);

        myDequeTestFile << "\nConstructs elements in-place from Point3D(10) to Point3D(29) at the end of test deq1" << std::endl;
        for (int i = 10; i < 30; i++) {
            deq.emplace_back(i);
        }
        writeDeque(deq, myDequeTestFile);

        myDequeTestFile << "\nPop the last 13 elements from test deq1" << std::endl;
        for (int i = 0; i < 13; i++) {
            deq.pop_back();
        }
        writeDeque(deq, myDequeTestFile);

        myDequeTestFile << "\nEmplace from Point3D(100) to Point3D(1000) rvalue elements at the beginning of test deq1" << std::endl;
        for (int i = 1; i < 11; i++) {
            deq.emplace(deq.cbegin(), Point3D(i * 100));
        }
        writeDeque(deq, myDequeTestFile);

        myDequeTestFile << "\nResize test deq2 to 20 Point3D(20) elements" << std::endl;
        deq2.resize(20, Point3D(20));
        writeDeque(deq2, myDequeTestFile);

        myDequeTestFile << "\nResize test deq1 to 10 Point3D elements" << std::endl;
        deq.resize(10, Point3D(10));
        writeDeque(deq, myDequeTestFile);

        myDequeTestFile << "\nSwap test deq1 and test deq2" << std::endl;
        deq.swap(deq2);
        myDequeTestFile << "\nTest deq1" << std::endl;
        writeDeque(deq, myDequeTestFile);
        myDequeTestFile << "\nTest deq2" << std::endl;
        writeDeque(deq2, myDequeTestFile);
    }

    myDequeTestFile.close();

    return 0;
}
