#include <iostream>
#include <iomanip>
#include <fstream>
#include <stack>
#include <queue>

#include "Stack.h"

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
void writeStack(Stack<T>& stack, std::ofstream& tFile) {
    tFile << "\nStack::size() = " << stack.size() << "\n" << std::endl;
        
    tFile << "--------start-popping-stack---------" << std::endl;
    while (!stack.empty()) {
        tFile << stack.top() << std::endl;
        stack.pop();
    }    
    tFile << "--------stop-popping-stack----------" << std::endl;
}

int main() {
    std::ofstream myStackTestFile("StackTests.txt", std::ofstream::out | std::ios::trunc);

    myStackTestFile << "STACK CONSTRUCTION, CLEANUP, COPY, AND MOVE\n" << std::endl;

    // default Stack ctor
    {   
        myStackTestFile << "* Default Stack ctor" << std::endl;

        Stack<Point3D> defStack;

        writeStack(defStack, myStackTestFile);
    }

    // Stack with n default elements
    {
        myStackTestFile << "\n* Stack created from Deque with 18 Point3D default elements" << std::endl;

        size_t n = 18;
        Deque<Point3D> defDeq(n);

        Stack<Point3D> defStack(defDeq);

        writeStack(defStack, myStackTestFile);
    }

    // Stack with n number of elements
    {
        myStackTestFile << "\n* Stack created from moved Deque with 5 Point3D(10) elements" << std::endl;

        size_t n = 5;
        Point3D p3d(10);

        Deque<Point3D> deq(n, p3d);

        Stack<Point3D> stack(std::move(deq));

        writeStack(stack, myStackTestFile);
    }

    // Stack copy ctor
    {
        myStackTestFile << "\n* Stack copy ctor with 7 Point3D(15) elements" << std::endl;

        size_t n = 7;
        Point3D p3d(15);

        Stack<Point3D> source;

        for (unsigned i = 0; i < n; ++i)
            source.push(p3d);

        Stack<Point3D> stack(source);

        myStackTestFile << "\nPrint lhs Stack" << std::endl;
        writeStack(stack, myStackTestFile);

        myStackTestFile << "\nPrint rhs Stack" << std::endl;
        writeStack(source, myStackTestFile);
    }

    // Stack move ctor
    {
        myStackTestFile << "\n* Stack move ctor with 9 Point3D(20) elements" << std::endl;

        size_t n = 9;
        Point3D p3d(20);

        Stack<Point3D> source;

        for (unsigned i = 0; i < n; ++i)
            source.push(p3d);

        Stack<Point3D> stack(std::move(source));

        myStackTestFile << "\nPrint lhs Stack" << std::endl;
        writeStack(stack, myStackTestFile);

        myStackTestFile << "\nPrint rhs Stack" << std::endl;
        writeStack(source, myStackTestFile);
    }

    // Stack copy assignment operator
    {
        myStackTestFile << "\n* Stack copy assignment operator" << std::endl;

        size_t n = 11;
        Point3D p3d(24);

        Stack<Point3D> right;

        for (unsigned i = 0; i < n; ++i)
            right.push(p3d);

        Stack<Point3D> left;

        for (unsigned i = 0; i < 3; ++i)
            left.push(Point3D());

        myStackTestFile << "\nPrint lhs Stack with 3 default Point3D elements [before assignemt]" << std::endl;
        writeStack(left, myStackTestFile);

        left = right;

        myStackTestFile << "\nPrint lhs Stack after assignemt with 11 Point3D(24) elements" << std::endl;
        writeStack(left, myStackTestFile);
    }

    // Stack move assignment operator
    {
        myStackTestFile << "\n* Stack move assignment operator" << std::endl;

        size_t n = 14;
        Point3D p3d(28);

        Stack<Point3D> right;

        for (unsigned i = 0; i < n; ++i)
            right.push(p3d);

        Stack<Point3D> left;

        for (unsigned i = 0; i < 5; ++i)
            left.push(Point3D());
            
        myStackTestFile << "\nPrint lhs Stack with 5 default Point3D elements [before assignemt]" << std::endl;
        writeStack(left, myStackTestFile);

        left = std::move(right);

        myStackTestFile << "\nPrint lhs Stack after move assignment with 14 Point3D(24) elements" << std::endl;
        writeStack(left, myStackTestFile);

        myStackTestFile << "\n* Print rhs Stack after its move" << std::endl;
        writeStack(right, myStackTestFile);
    }

    // Stack element access
    {
        myStackTestFile << "\nSTACK ELEMENT ACCESS" << std::endl;

        Point3D p1(25, 35, 45), p2(500), p3, p4(9, 8, 7), p5(1, 1, 1);

        Stack<Point3D> stack;

        stack.push(p1);
        stack.push(p2);
        stack.push(p3);
        stack.push(p4);
        stack.push(p5);

        myStackTestFile << "\nPopulate Stack with Point3D(25, 35, 45), Point3D(500), default Point3D, Point3D(9, 8, 7),"
                        " Point3D(1, 1, 1)" << std::endl;
        writeStack(stack, myStackTestFile);
    }

    // Stack capacity
    {
        myStackTestFile << "\nSTACK CAPACITY" << std::endl;

        Stack<Point3D> stack;

        myStackTestFile << "\nPrint empty Stack" << std::endl;
        writeStack(stack, myStackTestFile);

        stack.empty() ? myStackTestFile << "\nThe Stack is empty" << std::endl : 
                      myStackTestFile << "\nThe Stack is not empty" << std::endl;
        
        for (unsigned i = 1; i < 6; i++)
            stack.push(Point3D(i));

        myStackTestFile << "\nPop Stack with Point3D(1) to Point3D(5) elements" << std::endl;
        writeStack(stack, myStackTestFile);

        stack.empty() ? myStackTestFile << "\nThe Stack is empty" << std::endl : 
                      myStackTestFile << "\nThe Stack is not empty" << std::endl;
    }

    // Stack modifiers
    {   
        myStackTestFile << "\nDEQUE MODIFIERS" << std::endl;

        Stack<Point3D> stack;

        myStackTestFile << "\nPopulate Stack with Point3D(1) to Point3D(10) elements" << std::endl;
        for (unsigned i = 0; i < 10; ++i) 
            stack.emplace(i);

        writeStack(stack, myStackTestFile);

        Point3D p10(10), p20(20);

        Deque<Point3D> deq1(10, p10);
        Deque<Point3D> deq2(20, p20);

        myStackTestFile << "\nCreate Stack st1 from Deque with 10 Point3D(10) elements" << std::endl;
        Stack<Point3D> st1(deq1);

        myStackTestFile << "\nCreate Stack st2 from Deque with 20 Point3D(20) elements" << std::endl;
        Stack<Point3D> st2(deq2);

        myStackTestFile << "\nSwap st1 with st2" << std::endl;
        st1.swap(st2);

        myStackTestFile << "\nPop st1" << std::endl;
        writeStack(st1, myStackTestFile);
        myStackTestFile << "\nPop st2" << std::endl;
        writeStack(st2, myStackTestFile);
    }

    myStackTestFile.close();

    return 0;
}
