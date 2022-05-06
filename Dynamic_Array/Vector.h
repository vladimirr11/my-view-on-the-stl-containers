#ifndef VECTOR_H
#define VECTOR_H

#include <memory>
#include <exception>

// interface of custum dynamically-sized heap-allocated Vector (std::vector)

namespace constants {
    constexpr size_t INIT_CAPACITY = 2;
    constexpr float MEM_GROWTH = 1.5f;
}

template<typename _Type>
class Vector {
public:
    using iterator = _Type*;
    using const_iterator = const _Type*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // ctors
    Vector();

    explicit Vector(size_t size);
    Vector(size_t size, const _Type& initValue);

    Vector(const Vector& source);
    Vector(Vector&& source);

    Vector(std::initializer_list<_Type> initList);

    // destructor
    ~Vector();

    // operator=
    Vector& operator=(const Vector& right);

    Vector& operator=(Vector&& right);

    // element access
    constexpr _Type& at(size_t idx);
    constexpr const _Type& at(size_t idx) const;

    constexpr _Type& operator[](size_t idx);
    constexpr const _Type& operator[](size_t idx) const;

    constexpr _Type& front();
    constexpr const _Type& front() const;

    constexpr _Type& back();
    constexpr const _Type& back() const;

    constexpr _Type* data();
    constexpr const _Type* data() const;

    // iterators
    constexpr iterator begin() { 
        return iterator(&_data[0]); 
    }

    constexpr const_iterator cbegin() const { 
        return const_iterator(&_data[0]); 
    };

    constexpr iterator end() { 
        return iterator(&_data[_size]); 
    }

    constexpr const_iterator cend() const { 
        return const_iterator(&_data[_size]);
    }

    constexpr reverse_iterator rbegin() { 
        return reverse_iterator(end());
    }

    constexpr const_reverse_iterator crbegin() const { 
        return const_reverse_iterator(cend()); 
    }

    constexpr reverse_iterator rend() {
        return reverse_iterator(begin());
    }
    constexpr const_reverse_iterator crend() const {
        return const_reverse_iterator(cbegin());
    }

    // capacity
    constexpr bool empty() const;

    constexpr size_t size() const;

    constexpr void reserve(size_t newCapacity);

    constexpr size_t capacity() const;

    constexpr void shrink_to_fit();

    // modifiers
    constexpr void clear();

    constexpr iterator insert(const_iterator pos, const _Type& value);
    constexpr iterator insert(const_iterator pos, _Type&& value);

    constexpr void insert(const_iterator pos, size_t count, const _Type& value);

    template<typename... Args>
    constexpr iterator emplace(const _Type* pos, Args&&... args);

    constexpr iterator erase(const_iterator pos);
    constexpr iterator erase(const_iterator firstIt, const_iterator lastIt);

    constexpr void push_back(const _Type& value);
    constexpr void push_back(_Type&& value);

    template <typename... Args>
    constexpr _Type& emplace_back(Args&&... args);

    constexpr void pop_back();

    constexpr void resize(size_t newSize, const _Type& value);

    constexpr void swap(Vector& other);

private:
    void _reAllocMem(size_t newCapacity);
    void _moveBackward(size_t first, size_t last);

private:
    size_t _size;
    size_t _capacity;
    _Type* _data;
};

// Vector definition

template<typename _Type>
void Vector<_Type>::_reAllocMem(size_t capacity) {

    size_t newCapacity = std::max(constants::INIT_CAPACITY, capacity);

    _Type* newData = static_cast<_Type*>(::operator new(newCapacity * sizeof(_Type)));

    for (size_t i = 0; i < _size; i++) {
        newData[i] = std::move(_data[i]);
    }

    for (size_t i = 0; i < _size; i++) {
        _data[_size - 1 - i].~_Type();
    }
    ::operator delete(_data, _capacity * sizeof(_Type));

    _data = newData;
    _capacity = newCapacity;
}

template<typename _Type>
void Vector<_Type>::_moveBackward(size_t first, size_t last) {
    for (size_t i = last; i > first; i--) {
        _data[i] = _data[i - 1];
    }
}

template<typename _Type>
Vector<_Type>::Vector() : _size(0), _capacity(0), _data(nullptr) {}

template<typename _Type>
Vector<_Type>::Vector(size_t size) : 
    _size(size), _capacity(size), _data(static_cast<_Type*>(::operator new(size * sizeof(_Type)))) {
    
    for (size_t i = 0; i < _size; i++) {
        _data[i] = _Type();
    }
}

template<typename _Type>
Vector<_Type>::Vector(size_t size, const _Type& initValue) : 
    _size(size), _capacity(size), _data(static_cast<_Type*>(::operator new(size * sizeof(_Type)))) {

    for (size_t i = 0; i < _size; i++) {
        _data[i] = initValue;
    }
}

template<typename _Type>
Vector<_Type>::Vector(const Vector<_Type>& source) : 
    _size(source._size), _capacity(source._capacity), _data(static_cast<_Type*>(::operator new(source._capacity * sizeof(_Type)))) {

    std::copy(source._data, source._data + source._capacity, _data);   
}

template<typename _Type>
Vector<_Type>::Vector(Vector<_Type>&& source) : _size(0), _capacity(0), _data(nullptr) {
    *this = std::move(source);   
}

template<typename _Type>
Vector<_Type>::Vector(std::initializer_list<_Type> initList) : 
    _size(initList.size()), _capacity(initList.size()), _data(static_cast<_Type*>(::operator new(_capacity * sizeof(_Type)))) {

    std::copy(initList.begin(), initList.end(), _data);   
}

template<typename _Type>
Vector<_Type>::~Vector() {
    clear();
    ::operator delete(_data, _capacity * sizeof(_Type));
}

template<typename _Type>
Vector<_Type>& Vector<_Type>::operator=(const Vector& right) {
    if (this != &right) {
        if (_data) {
            clear();
        }

        _size = right._size;
        _capacity = right._capacity;
        _data = static_cast<_Type*>(::operator new(right._capacity * sizeof(_Type)));

        std::copy(right._data, right._data + right._capacity, _data);
    }

    return *this;
}

template<typename _Type>
Vector<_Type>& Vector<_Type>::operator=(Vector&& right) {
    if (this != &right) {
        if (_data) {
            clear();
        }

        _size = right._size;
        _capacity = right._capacity;
        _data = right._data;

        right._data = nullptr;

        right._capacity = 0;
        right._size = 0;
    }

    return *this;
}

template<typename _Type>
constexpr _Type& Vector<_Type>::at(size_t idx) {
    if (idx >= size()) {
        throw std::out_of_range("Vector Error: Index out of bounds!");
    }

    return _data[idx];
}

template<typename _Type>
constexpr const _Type& Vector<_Type>::at(size_t idx) const {
    if (idx >= size()) {
        throw std::out_of_range("Vector Error: Index out of bounds!");
    }

    return _data[idx];
}

template<typename _Type>
constexpr _Type& Vector<_Type>::operator[](size_t idx) {
    return _data[idx];
}

template<typename _Type>
constexpr const _Type& Vector<_Type>::operator[](size_t idx) const {
    return _data[idx];
}

template<typename _Type>
constexpr _Type& Vector<_Type>::front() {
    return _data[0];
}

template<typename _Type>
constexpr const _Type& Vector<_Type>::front() const {
    return _data[0];
}

template<typename _Type>
constexpr _Type& Vector<_Type>::back() {
    return _data[_size - 1];
}

template<typename _Type>
constexpr const _Type& Vector<_Type>::back() const {
    return _data[_size - 1];
}

template<typename _Type>
constexpr _Type* Vector<_Type>::data() {
    return _data;
}

template<typename _Type>
constexpr const _Type* Vector<_Type>::data() const {
    return _data;
}

template<typename _Type>
constexpr bool Vector<_Type>::empty() const {
    return size() == 0;
}

template<typename _Type>
constexpr size_t Vector<_Type>::size() const {
    return _size;
}

template<typename _Type>
constexpr void Vector<_Type>::reserve(size_t newCapacity) {
    // in case of shrinking
    if (newCapacity < _size) {
        _size = newCapacity;
        _reAllocMem(newCapacity);
        return;
    }

    _reAllocMem(newCapacity);
}

template<typename _Type>
constexpr size_t Vector<_Type>::capacity() const {
    return _capacity;
}

template<typename _Type>
constexpr void Vector<_Type>::shrink_to_fit() {
    _reAllocMem(_size);
}

template<typename _Type>
constexpr void Vector<_Type>::clear() {
    // calls the destructor of each element in reverse order
    for (size_t i = 0; i < _size; i++) {
        _data[_size - 1 - i].~_Type();
    }

    _size = 0;
}  

template<typename _Type>
constexpr typename Vector<_Type>::iterator Vector<_Type>::insert(const_iterator where, const _Type& value) {
    return emplace(where, value);
}

template<typename _Type>
constexpr typename Vector<_Type>::iterator Vector<_Type>::insert(const_iterator where, _Type&& value) {
    return emplace(where, std::forward<_Type>(value));
}

template<typename _Type>
constexpr void Vector<_Type>::insert(const_iterator where, size_t count, const _Type& value) {
    const auto distance = where - cbegin();

    if (_size + count < _capacity) {
        for (size_t i = _size + count - 1; i > distance + count - 1; i--) {
            _data[i] = _data[i - count];
        }
    } else {
        if (_size + count > _capacity * constants::MEM_GROWTH) {
            _reAllocMem(_size + count * constants::MEM_GROWTH);
        } else {
            _reAllocMem(_capacity * constants::MEM_GROWTH);
        }
        for (size_t i = _size + count - 1; i > distance + count - 1; i--) {
            _data[i] = _data[i - count];
        }
    }

    for (size_t i = 0; i < count; i++) {
        _data[distance + i] = value;
        _size++;
    }
}

template<typename _Type>
constexpr typename Vector<_Type>::iterator Vector<_Type>::erase(const_iterator where) {
    const size_t distance = where - cbegin();
    
    // check if where is before the last valid element in the Vector
    if (distance + 1 < _size) {
        for (size_t i = distance; i < _size; i++) {
            _data[i] = _data[i + 1];
        }
        _data[--_size].~_Type();
    } else {
        _data[distance].~_Type();
        _size--;
    }

    return &_data[distance];
}

template<typename _Type>
constexpr typename Vector<_Type>::iterator Vector<_Type>::erase(const_iterator first, const_iterator last) {
    const size_t distance = last - first;
    const size_t start = first - cbegin();

    for (size_t i = start; i < _size - distance; i++) {
        _data[i] = _data[i + distance];
    }

    for (size_t i = _size - 1; i > _size - distance; i--) {
        _data[i].~_Type();
    }

    _size -= distance;

    return &_data[start];
}

template<typename _Type>
template<typename... Args>
constexpr typename Vector<_Type>::iterator Vector<_Type>::emplace(const_iterator where, Args&&... args) {
    const auto distance = where - cbegin();

    if (_size != _capacity) {
        if (where == cend()) {
            emplace_back(_Type(std::forward<Args>(args)...));
            return &_data[distance];
        } else {
            _moveBackward(distance, _size);
        }
    } else {   
        _reAllocMem(_capacity * constants::MEM_GROWTH);
        _moveBackward(distance, _size);
    }

    auto it = &_data[distance];
    *it = _Type(std::forward<Args>(args)...);
    _size++;    

    return it;
}

template<typename _Type>
constexpr void Vector<_Type>::push_back(const _Type& value) {
    if (_size >= _capacity) {
        _reAllocMem(_capacity * constants::MEM_GROWTH);
    }

    _data[_size++] = value;
}

template<typename _Type>
constexpr void Vector<_Type>::push_back(_Type&& value) {
    if (_size >= _capacity) {
        _reAllocMem(_capacity * constants::MEM_GROWTH);
    }

    _data[_size++] = std::move(value);
}

template<typename _Type>
template <typename... Args>
constexpr _Type& Vector<_Type>::emplace_back(Args&&... args) {
    if (_size >= _capacity) {
        _reAllocMem(_capacity * constants::MEM_GROWTH);
    }

    new(&_data[_size]) _Type(std::forward<Args>(args)...);

    return _data[_size++];
}

template<typename _Type>
constexpr void Vector<_Type>::pop_back() {
    if (_size > 0) {
        _size--;
        _data[_size].~_Type();
    }
} 

template<typename _Type>
constexpr void Vector<_Type>::resize(size_t newSize, const _Type& value) {
    if (newSize > _size) {
        if (newSize < _capacity) {
            // construct elements inplace until reaches requested newSize
            for (size_t i = _size; i < newSize; i++) {
                new(&_data[i]) _Type(value);
            }
            _size = newSize;
        } else {
            // realloc memory then construct elements inplace until reaches requested newSize
            _reAllocMem(newSize * constants::MEM_GROWTH);
            for (size_t i = _size; i < newSize; i++) {
                new(&_data[i]) _Type(value);
            }
            _size = newSize;
        }
    } else {
        // destroy elements in reverse order until reaches requested newSize
        for (size_t i = _size - 1; i > newSize; i--) {
            _data[i].~_Type();
        }

        _size = newSize;
    }
}

template<typename _Type>
constexpr void Vector<_Type>::swap(Vector& other) {
    if (this != &other) {
        std::swap(_data, other._data);
        std::swap(_size, other._size);
        std::swap(_capacity, other._capacity);
    }
}

#endif // !VECTOR_H
