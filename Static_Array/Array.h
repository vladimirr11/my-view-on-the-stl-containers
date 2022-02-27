#ifndef STATICARRAY_H
#define STATICARRAY_H

#include <exception>
#include <algorithm>

// interface of fixed-size stack-allocated Array (std::array)

template<typename Type, std::size_t Size>
class Array {
public:
    using iterator = Type*;
    using const_iterator = const Type*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // element access
    constexpr Type& at(size_t idx);
    constexpr const Type& at(size_t idx) const;

    constexpr Type& operator[](size_t idx);
    constexpr const Type& operator[](size_t idx) const;

    constexpr Type& front();
    constexpr const Type& front() const;

    constexpr Type& back();
    constexpr const Type& back() const;

    constexpr Type* data();
    constexpr const Type* data() const;

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
    constexpr void fill(const Type& value);

    constexpr void swap(Array& other);

private:
    Type _data[Size];
};

// Array definition

template<typename Type, std::size_t Size>
constexpr Type& Array<Type, Size>::at(size_t idx) {
    if (idx >= size()) {
        throw std::out_of_range("Array Error: Index out of bounds!");
    }

    return _data[idx]; 
}

template<typename Type, std::size_t Size>
constexpr const Type& Array<Type, Size>::at(size_t idx) const { 
    if (idx >= size()) {
        throw std::out_of_range("Array Error: Index out of bounds!");
    }

    return _data[idx]; 
}

template<typename Type, std::size_t Size>
constexpr Type& Array<Type, Size>::operator[](size_t idx) {
    return _data[idx]; 
}

template<typename Type, std::size_t Size>
constexpr const Type& Array<Type, Size>::operator[](size_t idx) const { 
    return _data[idx]; 
}

template<typename Type, std::size_t Size>
constexpr Type& Array<Type, Size>::front() {
    return _data[0]; 
}

template<typename Type, std::size_t Size>
constexpr const Type& Array<Type, Size>::front() const { 
    return _data[0]; 
}

template<typename Type, std::size_t Size>
constexpr Type& Array<Type, Size>::back() {
    return _data[Size - 1]; 
}

template<typename Type, std::size_t Size>
constexpr const Type& Array<Type, Size>::back() const { 
    return _data[Size - 1]; 
}

template<typename Type, std::size_t Size>
constexpr Type* Array<Type, Size>::data() { 
    return _data;
}

template<typename Type, std::size_t Size>
constexpr const Type* Array<Type, Size>::data() const { 
    return _data; 
}

template<typename Type, std::size_t Size>
constexpr typename Array<Type, Size>::iterator Array<Type, Size>::begin() { 
    return iterator(&_data[0]);
}

template<typename Type, std::size_t Size>
constexpr typename Array<Type, Size>::const_iterator Array<Type, Size>::cbegin() const { 
    return const_iterator(&_data[0]); 
}

template<typename Type, std::size_t Size>
constexpr typename Array<Type, Size>::iterator Array<Type, Size>::end() { 
    return iterator(&_data[Size]); 
}

template<typename Type, std::size_t Size>
constexpr typename Array<Type, Size>::const_iterator Array<Type, Size>::cend() const { 
    return const_iterator(&_data[Size]); 
}


template<typename Type, std::size_t Size>
constexpr typename Array<Type, Size>::reverse_iterator Array<Type, Size>::rbegin() { 
    return reverse_iterator(&_data[Size]);
}

template<typename Type, std::size_t Size>
constexpr typename Array<Type, Size>::const_reverse_iterator Array<Type, Size>::crbegin() const { 
    return const_reverse_iterator(&_data[Size]); 
}

template<typename Type, std::size_t Size>
constexpr typename Array<Type, Size>::reverse_iterator Array<Type, Size>::rend() { 
    return reverse_iterator(&_data[0]); 
}

template<typename Type, std::size_t Size>
constexpr typename Array<Type, Size>::const_reverse_iterator Array<Type, Size>::crend() const { 
    return const_reverse_iterator(&_data[0]); 

}

template<typename Type, std::size_t Size>
constexpr bool Array<Type, Size>::empty() const { 
    return size() == 0; 
}

template<typename Type, std::size_t Size>
constexpr size_t Array<Type, Size>::size() const { 
    return Size; 
}

template<typename Type, std::size_t Size>
constexpr void Array<Type, Size>::fill(const Type& value) {
    std::fill_n(begin(), size(), value);
}

template<typename Type, std::size_t Size>
constexpr void Array<Type, Size>::swap(Array& other) {
    std::swap_ranges(begin(), end(), other.begin());
}

#endif // !STATICARRAY_H
