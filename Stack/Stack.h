#ifndef STACK_H
#define STACK_H

#include "../Double_Ended_Queue_GNU_Version/Deque.h"

// interface of container adaptor that gives FILO behaviour

template<typename _Type, typename _Cont = Deque<_Type>>
class Stack {
public:
    using value_type = typename _Cont::value_type;
    using size_type = typename _Cont::size_type;
    using refernce = typename _Cont::reference;
    using const_reference = typename _Cont::const_reference;
    using container_type = _Cont; 

public:
    // ctors
    Stack();

    explicit Stack(const _Cont& cont);

    explicit Stack(_Cont&& cont);

    Stack(const Stack& source);

    Stack(Stack&& source);

    // dtor
    ~Stack();

    // operator=
    Stack& operator=(const Stack& right);

    Stack& operator=(Stack&& right);

    // element access
    _Type& top();
    const _Type& top() const;

    // capacity
    bool empty() const;

    size_t size() const;

    // modifiers
    void push(const _Type& value);
    void push(_Type&& value);

    template<typename... Args>
    decltype(auto) emplace(Args&&... args);

    void pop();

    void swap(Stack& other);

protected:
    _Cont _cont; // underlying container
};

// Stack definition

template<typename _Type, typename _Cont>
Stack<_Type, _Cont>::Stack() : _cont() {}

template<typename _Type, typename _Cont>
Stack<_Type, _Cont>::Stack(const _Cont& cont) : _cont(cont) {}

template<typename _Type, typename _Cont>
Stack<_Type, _Cont>::Stack(_Cont&& cont) : _cont(std::move(cont)) {}

template<typename _Type, typename _Cont>
Stack<_Type, _Cont>::Stack(const Stack& source) : _cont(source._cont) {}

template<typename _Type, typename _Cont>
Stack<_Type, _Cont>::Stack(Stack&& source) : _cont(std::move(source._cont)) {}

template<typename _Type, typename _Cont>
Stack<_Type, _Cont>::~Stack() {}

template<typename _Type, typename _Cont>
Stack<_Type, _Cont>& Stack<_Type, _Cont>::operator=(const Stack& right) {
    if (this != &right)
        _cont = right._cont;
    return *this;
}

template<typename _Type, typename _Cont>
Stack<_Type, _Cont>& Stack<_Type, _Cont>::operator=(Stack&& right) {
    if (this != &right)
        _cont = std::move(right._cont);
    return *this;
}

template<typename _Type, typename _Cont>
_Type& Stack<_Type, _Cont>::top() {
    return _cont.back();
}

template<typename _Type, typename _Cont>
const _Type& Stack<_Type, _Cont>::top() const {
    return _cont.back();
}

template<typename _Type, typename _Cont>
bool Stack<_Type, _Cont>::empty() const {
    return _cont.empty();
}

template<typename _Type, typename _Cont>
size_t Stack<_Type, _Cont>::size() const {
    return _cont.size();
}

template<typename _Type, typename _Cont>
void Stack<_Type, _Cont>::push(const _Type& value) {
    return _cont.push_back(value);
}

template<typename _Type, typename _Cont>
void Stack<_Type, _Cont>::push(_Type&& value) {
    return _cont.push_back(std::move(value));
}

template<typename _Type, typename _Cont>
template<typename... Args>
decltype(auto) Stack<_Type, _Cont>::emplace(Args&&... args) {
    return _cont.emplace_back(std::forward<Args>(args)...);
}

template<typename _Type, typename _Cont>
void Stack<_Type, _Cont>::pop() {
    return _cont.pop_back();
}

template<typename _Type, typename _Cont>
void Stack<_Type, _Cont>::swap(Stack& other) {
    std::swap(_cont, other._cont);
}

#endif // !STACK_H
