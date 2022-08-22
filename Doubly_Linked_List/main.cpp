#include <iostream>
#include <iomanip>
#include <fstream>
#include <list>

#include "List.h"

struct Point3D {
    Point3D() : _x(0.0f), _y(0.0f), _z(0.0f) {
        _memoryBlock = new int[10];
    }

    Point3D(float scaler) : _x(scaler), _y(scaler), _z(scaler) {
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
    friend std::ostream& operator<<(std::ostream&, const Point3D&);

    friend bool operator==(const Point3D&, const Point3D&);

    friend bool operator<(const Point3D&, const Point3D&);

    float _x, _y, _z;
    int* _memoryBlock = nullptr;
};

std::ostream& operator<<(std::ostream& out, const Point3D& point3d) {
    return out << "x=" << point3d._x << ", "  << "y=" << point3d._y << ", " << "z=" << point3d._z;
}

bool operator==(const Point3D& p3d1, const Point3D& p3d2) {
    if (p3d1._x == p3d2._x && p3d1._y == p3d2._y && p3d1._z == p3d2._z) {
        return true;
    }

    return false;
}

bool operator<(const Point3D& p3d1, const Point3D& p3d2) {
    return (p3d1._x < p3d2._x) ? true : false;
}

template<typename T>
void writeList(const List<T>& list, std::ofstream& tFile) {
    tFile << "\nList::size() = " << list.size() << "\n" << std::endl;
        
    tFile << "--------start-printing-list---------" << std::endl;    
    for (auto it = list.begin(); it != list.end(); it++) {
        tFile << *it << std::endl;
    }
    tFile << "--------stop-printing-list----------" << std::endl;
}


int main() {
    std::ofstream myListTestFile("ListTests.txt", std::ofstream::out | std::ios::trunc);

    myListTestFile << "LIST CONSTRUCTION, CLEANUP, COPY, AND MOVE\n" << std::endl;

    // default List ctor
    {   
        myListTestFile << "* Default List ctor" << std::endl;

        List<Point3D> defList;

        writeList(defList, myListTestFile);
    }

    // List with n default elements
    {
        myListTestFile << "\n* List ctor with 3 Point3D default elements" << std::endl;

        size_t n = 3;
        List<Point3D> defList(n);

        writeList(defList, myListTestFile);
    }

    // List with n number of elements
    {
        myListTestFile << "\n* List ctor with 5 Point3D(10) elements" << std::endl;

        size_t n = 5;
        Point3D p3d(10);

        List<Point3D> lst(n, p3d);

        writeList(lst, myListTestFile);
    }

    // List copy ctor
    {
        myListTestFile << "\n* List copy ctor with 7 Point3D(15) elements" << std::endl;

        size_t n = 7;
        Point3D p3d(15);

        List<Point3D> source(n, p3d);

        List<Point3D> lst(source);

        myListTestFile << "\nPrint lhs List" << std::endl;
        writeList(lst, myListTestFile);

        myListTestFile << "\nPrint rhs List" << std::endl;
        writeList(source, myListTestFile);
    }

    // List move ctor
    {
        myListTestFile << "\n* List move ctor with 9 Point3D(20) elements" << std::endl;

        size_t n = 9;
        Point3D p3d(20);

        List<Point3D> source(n, p3d);

        List<Point3D> lst(std::move(source));

        myListTestFile << "\nPrint lhs List" << std::endl;
        writeList(lst, myListTestFile);

        myListTestFile << "\nPrint rhs List" << std::endl;
        writeList(source, myListTestFile);
    }

    // List initializer_list ctor
    {
        myListTestFile << "\n* List initializer_list ctor with Point3D(10), Point3D(3, 4, 5)," 
        " and default Point3D elements" << std::endl;

        Point3D p3d1(10), p3d2(3, 4, 5), p3d3;

        List<Point3D> lst{ p3d1, p3d2, p3d3 };

        writeList(lst, myListTestFile);
    }

    // List copy assignment operator
    {
        myListTestFile << "\n* List copy assignment operator" << std::endl;

        size_t n = 11;
        Point3D p3d(24);

        List<Point3D> right(n, p3d);

        List<Point3D> left(3);

        myListTestFile << "\nPrint lhs List with 3 default Point3D elements [before assignemt]" << std::endl;
        writeList(left, myListTestFile);

        left = right;

        myListTestFile << "\nPrint lhs List after assignemt with 11 Point3D(24) elements" << std::endl;
        writeList(left, myListTestFile);
    }

    // List move assignment operator
    {
        myListTestFile << "\n* List move assignment operator" << std::endl;

        size_t n = 14;
        Point3D p3d(28);

        List<Point3D> right(n, p3d);

        List<Point3D> left(5);

        myListTestFile << "\nPrint lhs List with 5 default Point3D elements [before move assignment]" << std::endl;
        writeList(left, myListTestFile);

        left = std::move(right);

        myListTestFile << "\n* Print lhs List after move assignment with 14 Point3D(28) elements" << std::endl;
        writeList(left, myListTestFile);

        myListTestFile << "\n* Print rhs List after its move" << std::endl;
        writeList(right, myListTestFile);
    }

    // List element access
    {
        myListTestFile << "\nLIST ELEMENT ACCESS" << std::endl;

        Point3D p1(25, 35, 45), p2(500), p3, p4(9, 8, 7), p5(1, 1, 1);

        List<Point3D> lst;

        lst.push_back(p1);
        lst.push_back(p2);
        lst.push_back(p3);
        lst.push_back(p4);
        lst.push_back(p5);

        myListTestFile << "\nPrint List with Point3D(25, 35, 45), Point3D(500), default Point3D, Point3D(9, 8, 7),"
            " Point3D(1, 1, 1)" << std::endl;
        writeList(lst, myListTestFile);

        myListTestFile << "\nFront element: " << lst.front() << std::endl;
        myListTestFile << "Back element: " << lst.back() << std::endl;
    }

    // List iterators
    {
        myListTestFile << "\nLIST ITERATORS" << std::endl;

        Point3D p1(25, 35, 45), p2(500), p3, p4(9, 8, 7), p5(1, 1, 1);

        List<Point3D> lst;

        lst.push_back(p1);
        lst.push_back(p2);
        lst.push_back(p3);
        lst.push_back(p4);
        lst.push_back(p5);

        myListTestFile << "\nPrint List with Point3D(24, 35, 45), Point3D(500), default Point3D, Point3D(9, 8, 7),"
            " Point3D(1, 1, 1)" << std::endl;
        writeList(lst, myListTestFile);

        List<Point3D>::iterator beg = lst.begin();
        List<Point3D>::iterator end = lst.end();
        List<Point3D>::reverse_iterator rbeg = lst.rbegin();
        List<Point3D>::reverse_iterator rend = lst.rend();

        myListTestFile << "\nList element at begin() pos: " << *beg << std::endl;
        myListTestFile << "List element at end() pos: " << *end << std::endl;

        myListTestFile << "List element at rbegin() pos: " << *rbeg << std::endl;
        myListTestFile << "List element at rend() pos: " << *rend << std::endl;

        myListTestFile << "\nPrinting List elements from begin() to end() ...\n" << std::endl;
        for (; beg != end; beg++) {
            myListTestFile << *beg << std::endl;
        }

        myListTestFile << "\nPrinting List elements from rbegin() to rend() ...\n" << std::endl;
        for (; rbeg != rend; rbeg++) {
            myListTestFile << *rbeg << std::endl;
        }

        List<Point3D>::const_iterator cbeg = lst.cbegin();
        List<Point3D>::const_iterator cend = lst.cend();
        List<Point3D>::const_reverse_iterator crbeg = lst.crbegin();
        List<Point3D>::const_reverse_iterator crend = lst.crend();

        myListTestFile << "\nList element at cbegin() pos: " << *cbeg << std::endl;
        myListTestFile << "List element at cend() pos: " << *cend << std::endl;

        myListTestFile << "List element at crbegin() pos: " << *crbeg << std::endl;
        myListTestFile << "List element at crend() pos: " << *crend << std::endl;

        myListTestFile << "\nPrinting List elements from cbegin() to cend() ...\n" << std::endl;
        for (; cbeg != cend; cbeg++) {
            myListTestFile << *cbeg << std::endl;
        }

        myListTestFile << "\nPrinting List elements from crbegin() to crend() ...\n" << std::endl;
        for (; crbeg != crend; crbeg++) {
            myListTestFile << *crbeg << std::endl;
        }
    } 

    // List capacity
    {
        myListTestFile << "\nLIST CAPACITY" << std::endl;

        List<Point3D> lst;

        myListTestFile << "\nPrint empty List" << std::endl;
        writeList(lst, myListTestFile);

        lst.empty() ? myListTestFile << "\nThe List is empty" << std::endl : 
                      myListTestFile << "\nThe List is not empty" << std::endl;
        
        for (size_t i = 1; i < 6; i++) {
            lst.push_back(Point3D(i));
        }

        myListTestFile << "\nPrint List with Point3D(1) to Point3D(5) elements" << std::endl;
        writeList(lst, myListTestFile);

        lst.empty() ? myListTestFile << "\nThe List is empty" << std::endl : 
                      myListTestFile << "\nThe List is not empty" << std::endl;
    }

    // List modifiers
    {   
        myListTestFile << "\nLIST MODIFIERS" << std::endl;

        Point3D p3d(10);
        
        List<Point3D> lst(10, p3d);
        
        myListTestFile << "\nConstruct List with 10 Point3D(10) elements" << std::endl;
        writeList(lst, myListTestFile);

        Point3D p60(60);

        myListTestFile << "\nInsert 2 Point3D(60) elements at the beginning" << std::endl;
        lst.insert(lst.cbegin(), p60); // size 11
        lst.insert(lst.cbegin(), p60); // size 12
        writeList(lst, myListTestFile);

        Point3D p70(70);

        myListTestFile << "\nInsert Point3D(70) element at the end" << std::endl;
        lst.insert(lst.cend(), p70); // size 13
        writeList(lst, myListTestFile);

        Point3D p900(900);

        myListTestFile << "\nInsert Point3D(900) element at the beginning" << std::endl;
        lst.insert(lst.cbegin(), p900); // size 14
        writeList(lst, myListTestFile);

        myListTestFile << "\nInsert Point3D(900) element at 2nd position" << std::endl;
        auto iter = lst.cbegin();
        std::advance(iter, 2); 
        lst.insert(iter, p900); // size 15
        writeList(lst, myListTestFile);

        myListTestFile << "\nInsert Point3D(900) element at 6th position" << std::endl;
        std::advance(iter, 4); 
        lst.insert(iter, p900); // size 16
        writeList(lst, myListTestFile);
        
        Point3D p800(800);

        myListTestFile << "\nInsert 3 Poin3D(800) elements at the end" << std::endl;
        lst.insert(lst.cend(), p800); // size 17
        lst.insert(lst.cend(), p800); // size 18
        lst.insert(lst.cend(), p800); // size 19
        writeList(lst, myListTestFile);

        Point3D p200(200);

        myListTestFile << "\nMove Point3D(200) element at the beginning" << std::endl;
        lst.insert(lst.cbegin(), std::move(p200)); // size() 20
        writeList(lst, myListTestFile);

        myListTestFile << "\nInsert Point3D(200) rvalue element at the end" << std::endl;
        lst.insert(lst.cend(), Point3D(200, 200, 200)); // size() 21
        writeList(lst, myListTestFile);

        Point3D p1000(1000);
        
        myListTestFile << "\nInsert 4 Point3D(1000) elements at the beginning" << std::endl;
        lst.insert(lst.cbegin(), 4, p1000);
        writeList(lst, myListTestFile);

        myListTestFile << "\nInsert 2 Point3D(1000) elements at the end" << std::endl;
        lst.insert(lst.cend(), 2, p1000);
        writeList(lst, myListTestFile);

        // second test List
        List<Point3D> lst2;

        myListTestFile << "\nConstruct 2nd empty Test List" << std::endl;
        writeList(lst2, myListTestFile);

        myListTestFile << "\nInsert Point3D(1000) element at the beginning of test lst2" << std::endl;
        lst2.insert(lst2.cbegin(), p1000);
        writeList(lst2, myListTestFile);

        Point3D p300(300);

        myListTestFile << "\nInsert 3 Point3D(300) elements at the end of test lst2" << std::endl;
        lst2.insert(lst2.cend(), 3, p300);
        writeList(lst2, myListTestFile);

        myListTestFile << "\nEmplace Point3D(2000) element at end of test lst2" << std::endl;
        lst2.emplace(lst2.cend(), 2000);
        writeList(lst2, myListTestFile);

        myListTestFile << "\nErase element at end() position from test lst2" << std::endl;
        auto eIter = lst2.end();
        std::advance(eIter, -1);
        lst2.erase(eIter);
        writeList(lst2, myListTestFile);

        // myListTestFile << "\nErase element at 2nd position before the end() from test lst2" << std::endl;
        // std::advance(eIter, -2);
        // writeList(lst2, myListTestFile);

        myListTestFile << "\nErase element at first position from test lst2" << std::endl;
        auto begIt = lst2.erase(lst2.cbegin());
        myListTestFile << *begIt << std::endl;
        writeList(lst2, myListTestFile);

        myListTestFile << "\nTest lst1" << std::endl;
        writeList(lst, myListTestFile);

        myListTestFile << "\nErase first 10 elements from test lst1" << std::endl;
        auto it10 = lst.cbegin();
        std::advance(it10, 10);
        lst.erase(lst.cbegin(), it10);
        writeList(lst, myListTestFile);
        
        myListTestFile << "\nErase all elements from test lst1" << std::endl;
        lst.erase(lst.cbegin(), lst.cend());
        writeList(lst, myListTestFile);

        myListTestFile << "\nPush back from Point3D(0) to Point3D(9) to test lst1" << std::endl;
        for (int i = 0; i < 10; i++) {
            lst.push_back(Point3D(i));
        }
        writeList(lst, myListTestFile);

        myListTestFile << "\nConstructs elements in-place from Point3D(10) to Point3D(29) at the end of test lst1" << std::endl;
        for (int i = 10; i < 30; i++) {
            lst.emplace_back(i);
        }
        writeList(lst, myListTestFile);

        myListTestFile << "\nPop the last 13 elements from test lst1" << std::endl;
        for (int i = 0; i < 13; i++) {
            lst.pop_back();
        }
        writeList(lst, myListTestFile);

        myListTestFile << "\nPush from Point3D(100) to Point3D(1000) rvalue elements at the beginning of test lst1" << std::endl;
        for (int i = 1; i < 11; i++) {
            lst.push_front(Point3D(i * 100));
        }
        writeList(lst, myListTestFile);

        myListTestFile << "\nConstructs 10 Point3D(2000) elements in-place at the beginning of test lst2" << std::endl;
        for (int i = 1; i < 11; i++) {
            lst2.emplace_front(2000);
        }
        writeList(lst2, myListTestFile);

        myListTestFile << "\nPop first 10 elements from the beginning of test lst1" << std::endl;
        for (int i = 1; i < 11; i++) {
            lst.pop_front();
        }
        writeList(lst, myListTestFile);

        myListTestFile << "\nResize test lst2 to 20 elements" << std::endl;
        lst2.resize(20);
        writeList(lst2, myListTestFile);

        myListTestFile << "\nResize test lst2 to 10 elements" << std::endl;
        lst2.resize(10);
        writeList(lst2, myListTestFile);

        myListTestFile << "\nSwap test lst1 and test lst2" << std::endl;
        lst.swap(lst2);
        writeList(lst, myListTestFile);
        writeList(lst2, myListTestFile);
    }

    // List operations
    {
        myListTestFile << "\nLIST OPERATIONS" << std::endl;

        List<int> intLst = { 1, 3, 3, 5, 9 };
        myListTestFile << "\nTest list1" << std::endl;
        for (auto it = intLst.begin(); it != intLst.end(); it++) {
            myListTestFile << *it << " ";
        }
        myListTestFile << std::endl;

        List<int> intLst2 = { 2, 3, 4, 4, 7, 8 };
        myListTestFile << "\nTest list2" << std::endl;
        for (auto it = intLst2.begin(); it != intLst2.end(); it++) {
            myListTestFile << *it << " ";
        }
        myListTestFile << std::endl;

        myListTestFile << "\nMerge list2 into list1 using comparison function" << std::endl;
        intLst.merge(intLst2, [](const int i1, const int i2) {
            return i1 < i2;
        });
        for (auto it = intLst.begin(); it != intLst.end(); it++) {
            myListTestFile << *it << " ";
        }
        myListTestFile << std::endl;

        List<Point3D> lst;

        myListTestFile << "\nConstructs test lst1 with elements in-place from Point3D(0) to Point3D(9)" << std::endl;
        for (int i = 0; i < 10; i++) {
            lst.emplace_back(i);
        }
        writeList(lst, myListTestFile);

        List<Point3D> lst2;

        myListTestFile << "\nConstructs test lst2 with Point3D elements from Point3D(10) to Point3D(100)" << std::endl;
        for (int i = 1; i < 11; i++) {
            lst2.push_back(i * 10);
        }
        writeList(lst2, myListTestFile);

        myListTestFile << "\nSplice test lst2 into the beginning of test lst1" << std::endl;
        lst.splice(lst.cbegin(), lst2);
        writeList(lst, myListTestFile);
        writeList(lst2, myListTestFile);

        myListTestFile << "\nSplice 3rd element from test lst1 into the beginning of test lst2" << std::endl;
        auto iter = lst.cbegin();
        std::advance(iter, 3);
        lst2.splice(lst2.cbegin(), lst, iter);
        writeList(lst2, myListTestFile);
        writeList(lst, myListTestFile);

        myListTestFile << "\nSplice first 14 elements from test lst1 into test lst2" << std::endl;
        auto it = lst.cbegin();
        std::advance(it, 14);
        lst2.splice(lst2.cbegin(), lst, lst.cbegin(), it);
        myListTestFile << "\nTest lst2" << std::endl;
        writeList(lst2, myListTestFile);
        myListTestFile << "\nTest lst1" << std::endl;
        writeList(lst, myListTestFile);

        myListTestFile << "\nRemove Point3D(100) and Point3D(40) from test lst2" << std::endl;
        lst2.remove(Point3D(100));
        lst2.remove(Point3D(40));
        writeList(lst2, myListTestFile);

        myListTestFile << "\nRemove all Point3D < Point3D(10)" << std::endl;
        lst2.remove_if([](const Point3D& p1) {
            return p1 < Point3D(10);
        });
        writeList(lst2, myListTestFile);

        myListTestFile << "\nExclude non-unique Point3D elements" << std::endl;
        lst2.insert(lst2.cbegin(), Point3D(10));
        lst2.insert(lst2.cend(), Point3D(90));
        myListTestFile << "\nBefore calling unique()" << std::endl;
        writeList(lst2, myListTestFile);
        myListTestFile << "\nAfter calling unique()" << std::endl;
        lst2.unique();
        writeList(lst2, myListTestFile);

        myListTestFile << "\nAfter calling reverse()" << std::endl;
        lst2.reverse();
        writeList(lst2, myListTestFile);

        myListTestFile << "\nTesting sort()" << std::endl;
        List<int> lst3 { 476, 4950, 484, 35, 565, -494, 59, 44, 4, 67 };
        lst3.sort();
        writeList(lst3, myListTestFile);
    }

    return 0;
}
