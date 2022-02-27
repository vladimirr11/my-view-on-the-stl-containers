#ifndef STATICARRAY_H
#define STATICARRAY_H

#include <exception>
#include <algorithm>

// interface of fixed-size stack-allocated Array (std::array)

template<typename _Type, std::size_t _Size>
class Array {
public:
    using iterator = _Type*;
    using const_iterator = const _Type*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

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
    constexpr iterator begin();
    constexpr const_iterator cbegin() const;

    constexpr iterator end();
    constexpr const_iterator cend() const;

    constexpr reverse_iterator rbegin();
    constexpr const_reverse_iterator crbegin() const;

    constexpr reverse_iterator rend();
    constexpr const_reverse_iterator crend() const;

    // capacity
    constexpr bool empty() const;

    constexpr size_t size() const;

    // operations
    constexpr void fill(const _Type& value);

    constexpr void swap(Array& other);

private:
    _Type _data[_Size];
};

// Array definition

template<typename _Type, std::size_t _Size>
constexpr _Type& Array<_Type, _Size>::at(size_t idx) {
    if (idx >= size()) {
        throw std::out_of_range("Array Error: Index out of bounds!");
    }

    return _data[idx]; 
}

template<typename _Type, std::size_t _Size>
constexpr const _Type& Array<_Type, _Size>::at(size_t idx) const { 
    if (idx >= size()) {
        throw std::out_of_range("Array Error: Index out of bounds!");
    }

    return _data[idx]; 
}

template<typename _Type, std::size_t _Size>
constexpr _Type& Array<_Type, _Size>::operator[](size_t idx) {
    return _data[idx]; 
}

template<typename _Type, std::size_t _Size>
constexpr const _Type& Array<_Type, _Size>::operator[](size_t idx) const { 
    return _data[idx]; 
}

template<typename _Type, std::size_t _Size>
constexpr _Type& Array<_Type, _Size>::front() {
    return _data[0]; 
}

template<typename _Type, std::size_t _Size>
constexpr const _Type& Array<_Type, _Size>::front() const { 
    return _data[0]; 
}

template<typename _Type, std::size_t _Size>
constexpr _Type& Array<_Type, _Size>::back() {
    return _data[_Size - 1]; 
}

template<typename _Type, std::size_t _Size>
constexpr const _Type& Array<_Type, _Size>::back() const { 
    return _data[_Size - 1]; 
}

template<typename _Type, std::size_t _Size>
constexpr _Type* Array<_Type, _Size>::data() { 
    return _data;
}

template<typename _Type, std::size_t _Size>
constexpr const _Type* Array<_Type, _Size>::data() const { 
    return _data; 
}

template<typename _Type, std::size_t _Size>
constexpr typename Array<_Type, _Size>::iterator Array<_Type, _Size>::begin() { 
    return iterator(&_data[0]);
}

template<typename _Type, std::size_t _Size>
constexpr typename Array<_Type, _Size>::const_iterator Array<_Type, _Size>::cbegin() const { 
    return const_iterator(&_data[0]); 
}

template<typename _Type, std::size_t _Size>
constexpr typename Array<_Type, _Size>::iterator Array<_Type, _Size>::end() { 
    return iterator(&_data[_Size]); 
}

template<typename _Type, std::size_t _Size>
constexpr typename Array<_Type, _Size>::const_iterator Array<_Type, _Size>::cend() const { 
    return const_iterator(&_data[_Size]); 
}


template<typename _Type, std::size_t _Size>
constexpr typename Array<_Type, _Size>::reverse_iterator Array<_Type, _Size>::rbegin() { 
    return reverse_iterator(&_data[_Size]);
}

template<typename _Type, std::size_t _Size>
constexpr typename Array<_Type, _Size>::const_reverse_iterator Array<_Type, _Size>::crbegin() const { 
    return const_reverse_iterator(&_data[_Size]); 
}

template<typename _Type, std::size_t _Size>
constexpr typename Array<_Type, _Size>::reverse_iterator Array<_Type, _Size>::rend() { 
    return reverse_iterator(&_data[0]); 
}

template<typename _Type, std::size_t _Size>
constexpr typename Array<_Type, _Size>::const_reverse_iterator Array<_Type, _Size>::crend() const { 
    return const_reverse_iterator(&_data[0]); 

}

template<typename _Type, std::size_t _Size>
constexpr bool Array<_Type, _Size>::empty() const { 
    return size() == 0; 
}

template<typename _Type, std::size_t _Size>
constexpr size_t Array<_Type, _Size>::size() const { 
    return _Size; 
}

template<typename _Type, std::size_t _Size>
constexpr void Array<_Type, _Size>::fill(const _Type& value) {
    std::fill_n(begin(), size(), value);
}

template<typename _Type, std::size_t _Size>
constexpr void Array<_Type, _Size>::swap(Array& other) {
    std::swap_ranges(begin(), end(), other.begin());
}

#endif // !STATICARRAY_H
