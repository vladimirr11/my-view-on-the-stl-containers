#ifndef DEQUE_H
#define DEQUE_H

#include <iostream>
#include <memory>
#include <algorithm>
#include <exception>

namespace constants {
    constexpr size_t _defaultCapacity = 20;
    constexpr size_t _capacityGrowthFactor = 2;
}

template<typename _DequeIterValType, bool _IsConst>
class _DequeIterator;

// interface of custom double-ended queue implemented as circular array

template<typename _Type>
class Deque {
private: 
    friend class _DequeIterator<_Type, false>;
    friend class _DequeIterator<_Type, true>;

public:
    using iterator = _DequeIterator<_Type, false>;
    using const_iterator = _DequeIterator<_Type, true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // ctors
    Deque();
    
    explicit Deque(size_t count);

    Deque(size_t count, const _Type& value);

    Deque(const Deque& source);

    Deque(Deque&& source);

    Deque(std::initializer_list<_Type> ilist);

    // dtor
    ~Deque();

    // operator=
    Deque& operator=(const Deque& right);

    Deque& operator=(Deque&& right);

    // element access
    _Type& at(size_t pos);
    const _Type& at(size_t pos) const;

    _Type& operator[](size_t pos);
    const _Type& operator[](size_t pos) const;

    _Type& front();
    const _Type& front() const;

    _Type& back();
    const _Type& back() const;

    // iterators
    iterator begin();
    const_iterator cbegin() const;

    iterator end();
    const_iterator cend() const;

    reverse_iterator rbegin();
    const_reverse_iterator crbegin() const;

    reverse_iterator rend();
    const_reverse_iterator crend() const;

    // capacity
    bool empty() const;

    size_t size() const;

    void shrink_to_fit();

    // modifiers
    void clear();

    iterator insert(const_iterator where, const _Type& value);
    iterator insert(const_iterator where, _Type&& value);
    iterator insert(const_iterator where, size_t count, const _Type& value);

    template<typename... Args>
    iterator emplace(const_iterator where, Args&&... args);

    iterator erase(const_iterator where);
    iterator erase(const_iterator first, const_iterator last);

    void push_back(const _Type& value);
    void push_back(_Type&& value);

    template<typename... Args>
    _Type& emplace_back(Args&&... args);

    void pop_back();

    void push_front(const _Type& value);
    void push_front(_Type&& velue);

    template<typename... Args>
    _Type& emplace_front(Args&&... args);

    void pop_front();

    void resize(size_t newSize);
    void resize(size_t newSize, const _Type& value);

    void swap(Deque& other);

private:
    void _adjustBackWhenMoveRight();

    void _adjustBackWhenMoveLeft();

    void _adjustBacWhenAddNCount(size_t count);

    void _adjustBacWhenSubtrNCount(size_t count);

    void _moveRight(const_iterator first, const_iterator last, iterator destFirst);

    void _moveLeft(const_iterator first, const_iterator last, iterator destFirst);

    void _fillValue(iterator first, iterator last, const _Type& value);

    void _reAllocMem(size_t newCapacity);

    void _checkForReAlloc();

    iterator _makeIterator(const_iterator where);

private:
    size_t _capacity; // the size of memory allocated to the _data 
    size_t _front; // indicate the front element of the deque object
    size_t _back; // indicate one-past-the-end element of the deque object
    _Type* _data; // the Deque content 
};

// Deque definition

template<typename _Type>
void Deque<_Type>::_adjustBackWhenMoveRight() {
    _back == _capacity - 1 ? _back = 0 : _back++;
}

template<typename _Type>
void Deque<_Type>::_adjustBackWhenMoveLeft() {
    _back == 0 ? _back = _capacity - 1 : _back--;
}

template<typename _Type>
void Deque<_Type>::_adjustBacWhenAddNCount(size_t count) {
    _back + count >= _capacity ? _back = (_back - _capacity) + count : _back += count; 
}

template<typename _Type>
void Deque<_Type>::_adjustBacWhenSubtrNCount(size_t count) {
    count > _back ? _back = _capacity - (count - _back) : _back -= count;
}

template<typename _Type>
void Deque<_Type>::_moveRight(const_iterator first, const_iterator last, iterator destFirst) {
    for (; first != last; --last, --destFirst) {
        *destFirst = std::move(*last);
    }
}

template<typename _Type>
void Deque<_Type>::_moveLeft(const_iterator first, const_iterator last, iterator destFirst) {
    for (; first != last; ++first, ++destFirst) {
        *destFirst = std::move(*first);
    }
}

template<typename _Type>
void Deque<_Type>::_fillValue(iterator first, iterator last, const _Type& value) {
    for (; first != last; ++first) {
        *first = value;
    }
}

template<typename _Type>
void Deque<_Type>::_reAllocMem(size_t newCapacity) {
    _Type* newData = new _Type[newCapacity];

    if (_front <= _back) {
        for (size_t i = _front; i < _back; i++) {
            newData[i - _front] = std::move(_data[i]);
        }

    } else {
        for (size_t i = _front; i < _capacity; i++) {
            newData[i - _front] = std::move(_data[i]);
        }

        for (size_t i = 0; i < _back; i++) {
            newData[_capacity - _front + i] = std::move(_data[i]);
        }
    }

    delete[] _data;
    
    // Note: first reset _back then _front!
    _back = size();

    _front = 0;
    _data = newData;
    _capacity = newCapacity;
}

template<typename _Type>
void Deque<_Type>::_checkForReAlloc() {
    if (size() + 1 >= _capacity) {
        _reAllocMem(_capacity * constants::_capacityGrowthFactor);
    } else if (_capacity == 0) {
        _reAllocMem(constants::_defaultCapacity);
    }
}

template<typename _Type>
typename Deque<_Type>::iterator Deque<_Type>::_makeIterator(const_iterator where) {
    iterator deqIt;

    deqIt._dequePtr = this;
    deqIt._offset = where._offset;

    return deqIt;
}

template<typename _Type>
Deque<_Type>::Deque() : 
    _capacity(constants::_defaultCapacity), _front(0), _back(0), _data(new _Type[constants::_defaultCapacity]) {}

template<typename _Type>
Deque<_Type>::Deque(size_t count) : 
    _capacity(constants::_defaultCapacity), _front(0), _back(count), _data(new _Type[constants::_defaultCapacity]) {

    for (size_t i = 0; i < count; i++) {
        _data[i] = _Type();
    }
}

template<typename _Type>
Deque<_Type>::Deque(size_t count, const _Type& value) : 
    _capacity(constants::_defaultCapacity), _front(0), _back(count), _data(new _Type[constants::_defaultCapacity]) {

    for (size_t i = 0; i < count; i++) {
        _data[i] = value;
    }
}

template<typename _Type>
Deque<_Type>::Deque(const Deque& source) : 
    _capacity(source._capacity), _front(source._front), _back(source._back), _data(new _Type[source._capacity]) {

    for (size_t i = _front; i < _back; i++) {
        _data[i] = source._data[i];
    }
}

template<typename _Type>
Deque<_Type>::Deque(Deque&& source) : _capacity(0), _front(0), _back(0), _data(nullptr) {
    *this = std::move(source);
}

template<typename _Type>
Deque<_Type>::Deque(std::initializer_list<_Type> ilist) :
    _capacity(ilist.size() + constants::_defaultCapacity), _front(0), _back(ilist.size()), _data(new _Type[_capacity]) {
    
    std::copy(ilist.begin(), ilist.end(), _data);
}

template<typename _Type>
Deque<_Type>::~Deque() {
    clear();
    delete[] _data;
    _data = nullptr;
}

template<typename _Type>
Deque<_Type>& Deque<_Type>::operator=(const Deque& right) {
    if (this != std::addressof(right)) {
        if (_data) {
            delete[] _data;
            _data = nullptr;
        }
        
        _capacity = right._capacity;
        _front = right._front;
        _back = right._back;
        _data = new _Type[right._capacity];

        std::copy(right._data, right._data + right._capacity, _data);
    }

    return *this;
}

template<typename _Type>
Deque<_Type>& Deque<_Type>::operator=(Deque&& right) {
    if (this != std::addressof(right)) {
        if (_data) {
            delete[] _data;
            _data = nullptr;
        }

        _capacity = right._capacity;
        _front = right._front;
        _back = right._back;
        _data = right._data;

        right._data = nullptr;
        right._back = 0;
        right._front = 0;
        right._capacity = 0;
    }

    return *this;
}

template<typename _Type>
_Type& Deque<_Type>::at(size_t pos) {
    if (pos >= size()) {
        throw std::out_of_range("Deque Error: Index out of bounds!");
    }

    pos += _front;
    if (pos >= _capacity) {
        pos -= _capacity;
    }

    return _data[pos];
}

template<typename _Type>
const _Type& Deque<_Type>::at(size_t pos) const {
    if (pos >= size()) {
        throw std::out_of_range("Deque Error: Index out of bounds!");
    }

    pos += _front;
    if (pos >= _capacity) {
        pos -= _capacity;
    }

    return _data[pos];
}

template<typename _Type> 
_Type& Deque<_Type>::operator[](size_t pos) {
    pos += _front;
    if (pos >= _capacity) {
        pos -= _capacity;
    }

    return _data[pos];
}

template<typename _Type> 
const _Type& Deque<_Type>::operator[](size_t pos) const {
    pos += _front;
    if (pos >= _capacity) {
        pos -= _capacity;
    }

    return _data[pos];
}

template<typename _Type>
_Type& Deque<_Type>::front() {
    if (empty()) {
        std::cerr << "front() called on empty deque" << std::endl;
    }

    return _data[_front];
}

template<typename _Type>
const _Type& Deque<_Type>::front() const {
    if (empty()) {
        std::cerr << "front() called on empty deque" << std::endl;
    }
    
    return _data[_front];
}

template<typename _Type> 
_Type& Deque<_Type>::back() {
    if (empty()) {
        std::cerr << "back() called on empty deque" << std::endl;
    }

    return _data[_back - 1];
}

template<typename _Type> 
const _Type& Deque<_Type>::back() const {
    if (empty()) {
        std::cerr << "back() called on empty deque" << std::endl;
    }

    return _data[_back - 1];
}

template<typename _Type>
typename Deque<_Type>::iterator Deque<_Type>::begin() {
    iterator deqIt;

    deqIt._dequePtr = this;
    deqIt._offset = _front;

    return deqIt;
}

template<typename _Type>
typename Deque<_Type>::const_iterator Deque<_Type>::cbegin() const {
    const_iterator cdeqIt;

    cdeqIt._dequePtr = this;
    cdeqIt._offset = _front;

    return cdeqIt;
}

template<typename _Type> 
typename Deque<_Type>::iterator Deque<_Type>::end() {
    iterator deqIt;

    deqIt._dequePtr = this;
    deqIt._offset = _back;

    return deqIt;
}

template<typename _Type> 
typename Deque<_Type>::const_iterator Deque<_Type>::cend() const {
    const_iterator cdeqIt;

    cdeqIt._dequePtr = this;
    cdeqIt._offset = _back;

    return cdeqIt;
}

template<typename _Type>
typename Deque<_Type>::reverse_iterator Deque<_Type>::rbegin() {
    return reverse_iterator(end());
}

template<typename _Type>
typename Deque<_Type>::const_reverse_iterator Deque<_Type>::crbegin() const {
    return const_reverse_iterator(cend());
}

template<typename _Type>
typename Deque<_Type>::reverse_iterator Deque<_Type>::rend() {
    return reverse_iterator(begin());
}

template<typename _Type>
typename Deque<_Type>::const_reverse_iterator Deque<_Type>::crend() const {
    return const_reverse_iterator(cbegin());
}

template<typename _Type>
bool Deque<_Type>::empty() const {
    return size() == 0;
}

template<typename _Type>
size_t Deque<_Type>::size() const {
    return _back >= _front ? _back - _front : (_capacity - _front) + _back;
}

template<typename _Type>
void Deque<_Type>::shrink_to_fit() {
    _Type* newData = new _Type[size() + 1];
    std::move(begin(), end(), newData);

    delete[] _data;

    _capacity = size() + 1;
    _front = 0;
    _back = size();
    _data = newData;
}

template<typename _Type>
void Deque<_Type>::clear() {
    _front = 0;
    _back = 0;
}

template<typename _Type>
typename Deque<_Type>::iterator Deque<_Type>::insert(const_iterator where, const _Type& value) {
    return emplace(where, value);
}

template<typename _Type>
typename Deque<_Type>::iterator Deque<_Type>::insert(const_iterator where, _Type&& value) {
    return emplace(where, std::move(value));
}

template<typename _Type>
typename Deque<_Type>::iterator Deque<_Type>::insert(const_iterator where, size_t count, const _Type& value) {
    iterator deqIt = _makeIterator(where);

    if (count == 0) {
        return deqIt;
    }

    if (size() + count >= _capacity) {
        where._offset = _front - where._offset;
        _reAllocMem(count + (_capacity * constants::_capacityGrowthFactor));
    }

    _moveRight(--where, cend(), end() + count);
    _adjustBacWhenAddNCount(count);
    _fillValue(deqIt, deqIt + count, value);
    
    return deqIt;
}

template<typename _Type>
template<typename... Args>
typename Deque<_Type>::iterator Deque<_Type>::emplace(const_iterator where, Args&&...args) {
    if (size() + 1 >= _capacity) {
        // Note: in case of memory reallocation DequeIterator position must adjust according to the new _front
        where._offset = _front - where._offset;
        _reAllocMem(_capacity * constants::_capacityGrowthFactor);
    }

    iterator deqIt = _makeIterator(where);

    if (where == cbegin()) {
        emplace_front(std::forward<Args>(args)...);
        return deqIt;
    } else if (where == cend()) {
        emplace_back(std::forward<Args>(args)...);
        return deqIt;
    }

    _moveRight(--where, cend(), end() + 1);
    _adjustBackWhenMoveRight();

    *deqIt = _Type(std::forward<Args>(args)...);

    return deqIt;
}

template<typename _Type>
typename Deque<_Type>::iterator Deque<_Type>::erase(const_iterator where) {
    iterator deqIt = _makeIterator(where);

    // Note: cend() must be one-past-the-end offset to move properly all elements
    _moveLeft(++where, ++cend(), deqIt);
    _adjustBackWhenMoveLeft();

    return deqIt;
} 

template<typename _Type>
typename Deque<_Type>::iterator Deque<_Type>::erase(const_iterator first, const_iterator last) {
    iterator deqIt = _makeIterator(first);

    _moveLeft(last, cend(), deqIt);
    _adjustBacWhenSubtrNCount(static_cast<size_t>(last - first));

    return deqIt;
}

template<typename _Type>
void Deque<_Type>::push_back(const _Type& value) {
    _checkForReAlloc();

    if (_back == _capacity - 1) {
        _back = 0;
    }
     
    _data[_back++] = value;
}

template<typename _Type>
void Deque<_Type>::push_back(_Type&& value) {
    _checkForReAlloc();

    if (_back == _capacity - 1) {
        _back = 0;
    }
     
    _data[_back++] = std::move(value);
}

template<typename _Type> 
template<typename... Args>
_Type& Deque<_Type>::emplace_back(Args&&... args) {
    _checkForReAlloc();

    if (_back < _capacity) {
        _data[_back++] = _Type(std::forward<Args>(args)...);
        return back();
    }

    _back = 0;
    _data[_back] = _Type(std::forward<Args>(args)...);
    return back();
}

template<typename _Type>
void Deque<_Type>::pop_back() {
    if (empty()) {
        std::cerr << "pop_back() called on empty deque" << std::endl;
    }

    erase(cend() - 1);
}

template<typename _Type>
void Deque<_Type>::push_front(const _Type& value) {
    _checkForReAlloc();

    if (_front == 0) {
        _front = _capacity;
    }

    _data[--_front] = value;
}

template<typename _Type>
void Deque<_Type>::push_front(_Type&& value) {
    _checkForReAlloc();

    if (_front == 0) {
        _front = _capacity;
    }

    _data[--_front] = std::move(value);
}

template<typename _Type>
template<typename... Args>
_Type& Deque<_Type>::emplace_front(Args&&... args) {
    _checkForReAlloc();

    if (_front > 0) {
        _data[--_front] = _Type(std::forward<Args>(args)...);
        return front();
    }

    _front = _capacity - 1;
    _data[_front] = _Type(std::forward<Args>(args)...);
    return front();
}

template<typename _Type>
void Deque<_Type>::pop_front() {
    if (empty()) {
        std::cerr << "pop_front() called on empty deque" << std::endl;
    }

    erase(cbegin());
}

template<typename _Type>
void Deque<_Type>::resize(size_t newSize) {
    while (size() < newSize) {
        emplace_back();
    }

    while (newSize < size()) {
        pop_back();
    }
}

template<typename _Type>
void Deque<_Type>::resize(size_t newSize, const _Type& value) {
    while (size() < newSize) {
        emplace_back(value);
    }

    while (newSize < size()) {
        pop_back();
    }
}

template<typename _Type>
void Deque<_Type>::swap(Deque& other) {
    if (this != std::addressof(other)) {
        std::swap(_data, other._data);
        std::swap(_capacity, other._capacity);
        std::swap(_front, other._front);
        std::swap(_back, other._back);
    }
}

// definition of random access deque iterator class

template<typename _DequeIterValType, bool _IsConst>
class _DequeIterator {
private:
    friend class Deque<_DequeIterValType>;

public:
    using _Self = _DequeIterator<_DequeIterValType, _IsConst>;

    using iterator_category = std::random_access_iterator_tag;
    using value_type = _DequeIterValType;
    using difference_type = std::ptrdiff_t;
    using pointer = typename std::conditional_t<_IsConst, const value_type*, value_type*>;
    using reference = typename std::conditional_t<_IsConst, const value_type&, value_type&>;

    // ctors
    _DequeIterator() : _dequePtr(nullptr), _offset(0) {}

    _DequeIterator(const Deque<value_type>& deque) : _dequePtr(&deque), _offset(0) {}

    _DequeIterator(const _Self& other) : _dequePtr(other._dequePtr), _offset(other._offset) {}

    // copy assignment operator
    _Self& operator=(const _Self& right) {
        if (this != std::addressof(right)) {
            _dequePtr = right._dequePtr;
            _offset = right._offset;
        }

        return *this;
    }

    // operator overloads
    bool operator==(const _Self& right) const {
        return _offset == right._offset;
    }

    bool operator!=(const _Self& right) const {
        return !(*this == right);
    }
    
    // SFINAE enables the const indirection operator*() the const member of pointer operator->() 
    // and the subscript operator[]() or their non const variants depending on the bool _IsConst parameter 
    template<bool _Const = _IsConst>
    std::enable_if_t<_Const, reference> operator*() {
        if (_dequePtr == nullptr || _dequePtr->size() == 0) {
            throw std::runtime_error("DequeIterator Error: Invalid dereference");
        }

        return _dequePtr->_data[_offset];
    }

    template<bool _Const = _IsConst>
    std::enable_if_t<!_Const, reference> operator*() {
        if (_dequePtr == nullptr || _dequePtr->size() == 0) {
            throw std::runtime_error("DequeIterator Error: Invalid dereference");
        }

        return _dequePtr->_data[_offset];
    }

    template<bool _Const = _IsConst>
    std::enable_if_t<_Const, pointer> operator->() const {
        if (!_dequePtr) {
            throw std::runtime_error("DequeIterator Error: Invalid value retrieval");
        }

        return std::addressof(_dequePtr->_data[_offset]);
    }

    template<bool _Const = _IsConst>
    std::enable_if_t<!_Const, pointer> operator->() const {
        if (!_dequePtr) {
            throw std::runtime_error("DequeIterator Error: Invalid value retrieval");
        }

        return std::addressof(_dequePtr->_data[_offset]);
    }

    template<bool _Const = _IsConst>
    std::enable_if_t<_Const, reference> operator[](size_t index) const {
        if (index >= _dequePtr->size()) {
            throw std::out_of_range("DequeIterator Error: Index out of bounds");
        }

        return _dequePtr->operator[](index);
    }

    template<bool _Const = _IsConst>
    std::enable_if_t<!_Const, reference> operator[](size_t index) const {
        if (index >= _dequePtr->size()) {
            throw std::out_of_range("DequeIterator Error: Index out of bounds");
        }

        return _dequePtr->operator[](index);
    }

    // iterator increment and decrement
    _Self& operator++() { // prefix
        if (_offset + 1 >= _dequePtr->_capacity) {
            _offset = 0;
        } else {
            ++_offset;
        }

        return *this;
    }

    _Self operator++(int) { // postfix
        _Self tmp = *this;

        if (_offset + 1 >= _dequePtr->_capacity) {
            _offset = 0;
        } else {
            ++_offset;
        }

        return tmp;
    }

    _Self& operator--() { // prefix
        if (_offset == 0) {
            _offset = _dequePtr->_capacity - 1;
        } else {
            --_offset;
        }

        return *this;
    }

    _Self operator--(int) { // postfix
        _Self tmp = *this;

        if (_offset == 0) {
            _offset = _dequePtr->_capacity - 1;
        } else {
            --_offset;
        }

        return tmp;
    }

    // pointer arithmetic
    _Self& operator+=(const difference_type off) {
        _findOffsetWhenAdd(*this, off);
        return *this;
    }

    _Self operator+(const difference_type off) {
        _Self tmp = *this;
        _findOffsetWhenAdd(tmp, off);
        return tmp;
    }

    difference_type operator+(const _Self& right) const {
        return static_cast<difference_type>(_offset + right._offset);
    }

    _Self& operator-=(const difference_type off) {
        _findOffsetWhenSubtr(*this, off);
        return *this;
    }

    _Self operator-(const difference_type off) {
        _Self tmp = *this;
        _findOffsetWhenSubtr(tmp, off);
        return tmp;
    }

    difference_type operator-(const _Self& right) const {
        size_t diff{};
        
        if (right._offset < _offset) {
            diff = _offset - right._offset;
        } else {
            diff = (_dequePtr->_capacity - right._offset) + _offset;
        }

        return static_cast<difference_type>(diff);
    }

private:
    void _findOffsetWhenSubtr(_Self& it, const difference_type off) {
        if (it._dequePtr->_front < it._dequePtr->_back) {
            if (it._offset - off < it._dequePtr->_front) {
                it._offset = it._dequePtr->_back - ((it._offset - it._dequePtr->_front) - off);
            } else {
                it._offset -= off;
            }
        } else {
            if (it._offset - off >= 0) {
                it._offset -= off;
            } else {
                it._offset = it._dequePtr->_capacity - (off - it._offset);
            }
        }
    }

    void _findOffsetWhenAdd(_Self& it, const difference_type off) {
        if (it._dequePtr->_front < it._dequePtr->_back) {
            if (it._offset + off >= it._dequePtr->_capacity) {
                it._offset = (it._dequePtr->_back - it._dequePtr->_capacity) + off;
            } else {
                it._offset += off;
            }
        } else {
            if (it._offset + off < it._dequePtr->_capacity) {
                it._offset += off;
            } else {
                it._offset = (it._dequePtr->_front - it._dequePtr->_capacity) + off;
            }
        }
    }

protected:
    std::conditional_t<_IsConst, const Deque<value_type>*, Deque<value_type>*> _dequePtr{}; // this is the deque the iterator is bound to
    size_t _offset{}; // posistion of the iterator
};

#endif // !DEQUE_H
