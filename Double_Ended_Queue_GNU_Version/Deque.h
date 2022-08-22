#ifndef DEQUE_H
#define DEQUE_H

#include <iostream>
#include <memory>
#include <algorithm>
#include <exception>

constexpr size_t CHUNK_SIZE = 8;
constexpr size_t DEFAULT_INIT_MAP_SIZE = 8;
constexpr size_t SIZE_MPTR_IN_BYTES = 8;

template<typename _Type, typename _Ref, typename _Ptr>
class _DequeIterator;

// Interface of double-ended queue implemented as array of pointers to fixed size arrays 
// (map of nodes according to the gcc STL terminology). 
// The map grows outwards in both directions and shinks inwards from both directions.

template<typename _Type>
class Deque {
public:
    using value_type = _Type;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = _Type&;
    using const_reference = const _Type&;
    using pointer = _Type*;
    using const_pointer = const _Type*;

    using iterator = _DequeIterator<_Type, _Type&, _Type*>;
    using const_iterator = _DequeIterator<_Type, const _Type&, const _Type*>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using _Elptr = typename iterator::_Elptr;
    using _Mapptr = typename iterator::_Mapptr;;

public:
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

    void resize(size_t new_size);
    void resize(size_t new_size, const _Type& value);

    void swap(Deque& other);

private:
    void _allocate_nodes(_Mapptr n_start, _Mapptr n_finish);

    _Mapptr _initialize_map(size_t num_els);

    void _fill_map(iterator first, iterator last, const _Type& value);

    template<typename InputIt, typename ForwardIt>
    void _copy_map(InputIt first, InputIt last, ForwardIt dest_first);

    void _destroy_elements(iterator first, iterator last);

    void _deallocate_nodes(_Mapptr n_start, _Mapptr n_finish);

    void _deallocate_map();
    
    template<typename InputIt, typename OutputIt>
    void _move(InputIt first, InputIt last, OutputIt dest_first);

    template<typename BidirectIt1, typename BidirectIt2>
    void _move_backwards(BidirectIt1 first, BidirectIt1 last, BidirectIt2 dest_last);

    void _reallocate_map(size_t nodes_count, bool add_at_front);
    
    void _reserve_map_at_front(size_t nodes_count = 1);

    void _reserve_map_at_back(size_t nodes_count = 1);

    iterator _reserve_elements_at_front(size_t count);

    iterator _reserve_elements_at_back(size_t count);

    template<typename InputIt, typename ForwardIt>
    ForwardIt _uninit_move(InputIt first, InputIt last, ForwardIt dest_first);

    void _uninit_fill(iterator first, iterator last, const _Type& value);

    template<typename... Args>
    iterator _insert_helper(iterator where, Args&&... args);

    void _insert_range_helper(iterator where, size_t count, const _Type& value);

    void _fill_insert(iterator where, size_t count, const _Type& value);

private:
    _Mapptr _map; // array of pointers to arrays (map) of elements of type _Type 
    size_t _map_size; // the size of the map
    iterator _start; // the first node of the map (the first chunk/array of pointers) 
    iterator _finish; // the last node of the map (the last chunk/array of pointers)
};

// Deque definition

template<typename _Type>
void Deque<_Type>::_allocate_nodes(_Mapptr n_start, _Mapptr n_finish) {
    _Mapptr current_node = n_start;
    for (; current_node != n_finish; ++current_node) {
        *current_node = static_cast<_Elptr>(::operator new(CHUNK_SIZE * sizeof(_Type)));
    }
}

template<typename _Type>
typename Deque<_Type>::_Mapptr Deque<_Type>::_initialize_map(size_t num_els) {
    const size_t num_nodes = (num_els / CHUNK_SIZE) + 1;
    _map_size = std::max(DEFAULT_INIT_MAP_SIZE, num_nodes + 2);

    // allocate memory for the map
    _map = static_cast<_Mapptr>(::operator new(_map_size * SIZE_MPTR_IN_BYTES)); 

    _Mapptr node_start = _map + (_map_size - num_nodes) / 2;
    _Mapptr node_finish = node_start + num_nodes;

    // allocate memory for each chunk (node) of the map
    _allocate_nodes(node_start, node_finish);
    
    _start._set_node(node_start);
    _start._curr = _start._first;
    _finish._set_node(node_finish - 1);
    _finish._curr = _finish._first + (num_els % CHUNK_SIZE); // finds the offset

    return _map;
}

template<typename _Type>
void Deque<_Type>::_fill_map(iterator first, iterator last, const _Type& value) {
    for (; first != last; ++first) {
        ::new(static_cast<void*>(&*first)) _Type(value);
    }
}

template<typename _Type>
template<typename InputIt, typename ForwardIt>
void Deque<_Type>::_copy_map(InputIt first, InputIt last, ForwardIt dest_first) {
    for (; first != last; ++first, ++dest_first) {
        ::new(static_cast<void*>(&*dest_first)) _Type(*first);
    }
}

template<typename _Type>
void Deque<_Type>::_destroy_elements(iterator first, iterator last) {
    for (; first != last; ++first) {
        (*first).~_Type();
    }
}

template<typename _Type>
void Deque<_Type>::_deallocate_nodes(_Mapptr n_start, _Mapptr n_finish) {
    _Mapptr current_node = n_start;
    for (; current_node != n_finish; ++current_node) {
        ::operator delete(*current_node, CHUNK_SIZE * sizeof(_Type));
    }
}

template<typename _Type>
void Deque<_Type>::_deallocate_map() {
    // deallocates memory for [_start, _finish node)
    _deallocate_nodes(_start._node, _finish._node);

    // deallocates memory for the last node
    if (_finish._node) 
        ::operator delete(*_finish._node, CHUNK_SIZE * sizeof(_Type));
    
    // deallocates the map
    ::operator delete(_map, DEFAULT_INIT_MAP_SIZE);
}

template<typename _Type>
template<typename InputIt, typename OutputIt>
void Deque<_Type>::_move(InputIt first, InputIt last, OutputIt dest_first) {
    while (first != last) {
        *dest_first++ = std::move(*first++);
    }
}

template<typename _Type>
template<typename BidirectIt1, typename BidirectIt2>
void Deque<_Type>::_move_backwards(BidirectIt1 first, BidirectIt1 last, BidirectIt2 dest_last) {
    while (first != last) {
        *(--dest_last) = std::move(*(--last));
    }
}

template<typename _Type>
void Deque<_Type>::_reallocate_map(size_t nodes_count, bool add_at_front) {
    const size_t old_num_nodes = _finish._node - _start._node + 1;
    const size_t new_num_nodes = old_num_nodes + nodes_count;

    _Mapptr new_start_node;
    if (_map_size > 2 * new_num_nodes) {
        new_start_node = _map + (_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_count : 0);
        if (new_start_node < _start._node) {
            using std::copy;
            copy(_start._node, _finish._node + 1, new_start_node);
        } else {
            using std::copy_backward;
            copy_backward(_start._node, _finish._node + 1, new_start_node + old_num_nodes);
        }
    } else {
        size_t new_map_size = _map_size + std::max(_map_size, nodes_count) + 2;
        _Mapptr new_map = static_cast<_Mapptr>(::operator new(new_map_size * SIZE_MPTR_IN_BYTES));
        
        new_start_node = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_count : 0);
        
        using std::copy;
        copy(_start._node, _finish._node + 1, new_start_node);

        // deallocate only the _map!
        // the _start to _finish nodes are copied (the pointers) in the new_start_node
        ::operator delete(_map, DEFAULT_INIT_MAP_SIZE);

        _map = new_map;
        _map_size = new_map_size;
    }

    _start._set_node(new_start_node);
    _finish._set_node(new_start_node + old_num_nodes - 1);
}

template<typename _Type>
void Deque<_Type>::_reserve_map_at_front(size_t nodes_count) {
    if (nodes_count > _start._node - _map) {
        _reallocate_map(nodes_count, true);
    }
}

template<typename _Type>
void Deque<_Type>::_reserve_map_at_back(size_t nodes_count) {
    if (nodes_count + 1 > _map_size - (_finish._node - _map)) {
        _reallocate_map(nodes_count, false);
    }
}

template <typename _Type>
typename Deque<_Type>::iterator Deque<_Type>::_reserve_elements_at_front(size_t count) {
    const size_t vacancies = _start._curr - _start._first;
    
    if (count > vacancies) {
        const size_t new_elements = count - vacancies;
        const size_t new_nodes_count = (new_elements + CHUNK_SIZE - 1) / CHUNK_SIZE;
        _reserve_map_at_front(new_nodes_count);
        for (size_t i = 1; i <= new_nodes_count; ++i)
            *(_start._node - i) = static_cast<_Elptr>(::operator new(CHUNK_SIZE * sizeof(_Type)));
    }

    return _start - ptrdiff_t(count);
}

template <typename _Type>
typename Deque<_Type>::iterator Deque<_Type>::_reserve_elements_at_back(size_t count) {
    const size_t vacancies = _finish._last - _finish._curr;
    
    if (count > vacancies) {
        const size_t new_elements = count - vacancies;
        const size_t new_nodes_count = (new_elements + CHUNK_SIZE - 1) / CHUNK_SIZE;
        _reserve_map_at_back(new_nodes_count);
        for (size_t i = 1; i <= new_nodes_count; ++i)
            *(_finish._node + i) = static_cast<_Elptr>(::operator new(CHUNK_SIZE * sizeof(_Type)));
    }

    return _finish + ptrdiff_t(count);
}

template<typename _Type>
template<typename InputIt, typename ForwardIt>
ForwardIt Deque<_Type>::_uninit_move(InputIt first, InputIt last, ForwardIt dest_first) {
    ForwardIt curr_dest = dest_first;
    for (; first != last; ++first, ++curr_dest) {
        ::new(static_cast<void*>(&*curr_dest)) _Type(std::move(*first));
    }
    return curr_dest;
}

template<typename _Type>
void Deque<_Type>::_uninit_fill(iterator first, iterator last, const _Type& value) {
    for (; first != last; ++first) {
        ::new(static_cast<void*>(&*first)) _Type(value);
    }
}

template<typename _Type>
template<typename... Args>
typename Deque<_Type>::iterator Deque<_Type>::_insert_helper(iterator where, Args&&... args) {
    ptrdiff_t idx = where - _start;

    _Type val_cp(std::forward<Args>(args)...);

    // insert in the first half of the map
    if (static_cast<size_t>(idx) < size() / 2) {
        push_front(front());
        iterator front1 = _start;
        ++front1;
        iterator front2 = front1;
        ++front2;
        where = _start + idx;
        iterator where1 = where;
        ++where1;
        // moves the elements in the range [first, last) to another range beginning at dest_first
        _move(front2, where1, front1);

    // insert in the second half
    } else {
        push_back(back());
        iterator back1 = _finish;
        --back1;
        iterator back2 = back1;
        --back2;
        where = _start + idx;
        // moves in the range [first, last) to the range ending in dest_last; 
        // the elements are moved in reverse order - the last element is moved first
        _move_backwards(where, back2, back1);
    }

    *where = std::move(val_cp);
    return where;
}

template<typename _Type>
void Deque<_Type>::_insert_range_helper(iterator where, size_t count, const _Type& value) {
    const ptrdiff_t els_before = where - _start;
    const size_t size = this->size();
    _Type value_cp = value;
    
    // insert closer to the start 
    if (els_before < ptrdiff_t(size / 2)) {
        iterator new_start = _reserve_elements_at_front(count);
        iterator old_start = _start;
        where = _start + els_before;

        if (els_before >= ptrdiff_t(count)) {
            iterator start_n = _start + ptrdiff_t(count);
            _uninit_move(_start, start_n, new_start);
            _start = new_start;
            _move(start_n, where, old_start);
            using std::fill;
            fill(where - ptrdiff_t(count), where, value_cp);
        } else {
            iterator mid = _uninit_move(_start, where, new_start);
            _uninit_fill(mid, _start, value_cp);
            _start = new_start;
            using std::fill;
            fill(old_start, where, value_cp);
        }
    // insert closer to the finish
    } else {
        iterator new_finish = _reserve_elements_at_back(count);
        iterator old_finish = _finish;
        const ptrdiff_t els_after = ptrdiff_t(size) - els_before;
        where = _finish - els_after;

        if (els_after > ptrdiff_t(count)) {
            iterator finish_n = _finish - ptrdiff_t(count);
            _uninit_move(finish_n, _finish, _finish);
            _finish = new_finish;
            _move(where, finish_n, old_finish);
            using std::fill;
            fill(where, where + ptrdiff_t(count), value_cp);
        } else {
            _uninit_fill(_finish, where + ptrdiff_t(count), value_cp);
            _uninit_move(where, _finish, where + ptrdiff_t(count));
            _finish = new_finish;
            using std::fill;
            fill(where, old_finish, value_cp);
        }
    }
}   

template<typename _Type>
void Deque<_Type>::_fill_insert(iterator where, size_t count, const _Type& value) {
    if (where._curr == _start._curr) {
        iterator new_start = _reserve_elements_at_front(count);
        _fill_map(new_start, _start, value);
        _start = new_start;
    } else if (where._curr == _finish._curr) {
        iterator new_finish = _reserve_elements_at_back(count);
        _fill_map(_finish, new_finish, value);
        _finish = new_finish;
    } else {
        _insert_range_helper(where, count, value);
    }
}

template<typename _Type>
Deque<_Type>::Deque() : _map(), _map_size(0), _start(), _finish() {
    _initialize_map(0);
}

template<typename _Type>
Deque<_Type>::Deque(size_t count) : _map(), _map_size(0), _start(), _finish() {
    _initialize_map(count);
    _fill_map(_start, _finish, _Type());
}

template<typename _Type>
Deque<_Type>::Deque(size_t count, const _Type& value) : _map(), _map_size(0), _start(), _finish() {
    _initialize_map(count);
    _fill_map(_start, _finish, value);
}

template<typename _Type>
Deque<_Type>::Deque(const Deque& source) : _map(), _map_size(0), _start(), _finish() {
    _initialize_map(source.size());
    _copy_map(source.cbegin(), source.cend(), begin());
}

template<typename _Type>
Deque<_Type>::Deque(Deque&& source) : _map(), _map_size(0), _start(), _finish() {
    swap(source);    
}

template<typename _Type>
Deque<_Type>::Deque(std::initializer_list<_Type> ilist) : _map(), _map_size(0), _start(), _finish() {
    _initialize_map(0);
    for (auto it = ilist.begin(); it != ilist.end(); ++it) {
        emplace_back(*it);
    } 
}

template<typename _Type>
Deque<_Type>::~Deque() {
    clear();
}

template<typename _Type>
Deque<_Type>& Deque<_Type>::operator=(const Deque& right) {
    if (this != &right) {
        clear();
        _initialize_map(right.size());
        _copy_map(right.cbegin(), right.cend(), begin());
    }

    return *this;
}

template<typename _Type>
Deque<_Type>& Deque<_Type>::operator=(Deque&& right) {
    if (this != &right) {
        _destroy_elements(begin(), end());
        swap(right);
        // places right in a valid but unspecified state as required from the standard
        right._start._curr = right._finish._curr;
    }

    return *this;
}

template<typename _Type>
_Type& Deque<_Type>::at(size_t pos) {
    if (pos > size()) 
        throw std::out_of_range("Deque Error: Index out of bounds!");
    
    return (*this)[pos];
}

template<typename _Type>
const _Type& Deque<_Type>::at(size_t pos) const {
    if (pos > size()) 
        throw std::out_of_range("Deque Error: Index out of bounds!");
    
    return (*this)[pos];
}

template<typename _Type>
_Type& Deque<_Type>::operator[](size_t pos) {
    return _start[ptrdiff_t(pos)];
}

template<typename _Type>
const _Type& Deque<_Type>::operator[](size_t pos) const {
    return _start[ptrdiff_t(pos)];
}

template<typename _Type>
_Type& Deque<_Type>::front() {
    return *begin();
}

template<typename _Type>
const _Type& Deque<_Type>::front() const {
    return *begin();
}

template<typename _Type>
_Type& Deque<_Type>::back() {
    iterator tmp = end();
    --tmp;
    return *tmp;
}

template<typename _Type>
const _Type& Deque<_Type>::back() const {
    const_iterator tmp_back = end();
    --tmp_back;
    return *tmp_back;
}

template<typename _Type>
typename Deque<_Type>::iterator Deque<_Type>::begin() {
    return _start;
}

template<typename _Type>
typename Deque<_Type>::const_iterator Deque<_Type>::cbegin() const {
    return _start;
}

template<typename _Type>
typename Deque<_Type>::iterator Deque<_Type>::end() {
    return _finish;
}

template<typename _Type>
typename Deque<_Type>::const_iterator Deque<_Type>::cend() const {
    return _finish;
}

template<typename _Type>
typename Deque<_Type>::reverse_iterator Deque<_Type>::rbegin() {
    return reverse_iterator(_finish);
}

template<typename _Type>
typename Deque<_Type>::const_reverse_iterator Deque<_Type>::crbegin() const {
    return const_reverse_iterator(_finish);
}

template<typename _Type>
typename Deque<_Type>::reverse_iterator Deque<_Type>::rend() {
    return reverse_iterator(_start);
}

template<typename _Type>
typename Deque<_Type>::const_reverse_iterator Deque<_Type>::crend() const {
    return const_reverse_iterator(_start);
}

template<typename _Type>
bool Deque<_Type>::empty() const {
    return _finish == _start;
}

template<typename _Type>
size_t Deque<_Type>::size() const {
    return _finish == _start ? 0 : _finish - _start;
}

template<typename _Type>
void Deque<_Type>::clear() {
    _destroy_elements(_start, _finish);
    _deallocate_nodes(_start._node + 1, _finish._node + 1);
    _finish = _start;
}

template<typename _Type>
typename Deque<_Type>::iterator Deque<_Type>::insert(const_iterator where, const _Type& value) {
    if (where._curr == _start._curr) {
        push_front(value);
        return _start;
    } else if (where._curr == _finish._curr) {
        push_back(value);
        iterator tmp_back = _finish;
        --tmp_back;
        return tmp_back;
    }

    return _insert_helper(where._const_cast(), value);
}

template<typename _Type>
typename Deque<_Type>::iterator Deque<_Type>::insert(const_iterator where, _Type&& value) {
    return emplace(where, std::move(value));
}

template<typename _Type>
typename Deque<_Type>::iterator Deque<_Type>::insert(const_iterator where, size_t count, const _Type& value) {
    ptrdiff_t offset = where - cbegin();
    _fill_insert(where._const_cast(), count, value);
    return begin() + offset;
}

template<typename _Type>
template<typename... Args>
typename Deque<_Type>::iterator Deque<_Type>::emplace(const_iterator where, Args&&... args) {
    if (where._curr == _start._curr) {
        emplace_front(std::forward<Args>(args)...);
        return _start;
    } else if (where._curr == _finish._curr) {
        emplace_back(std::forward<Args>(args)...);
        iterator tmp_back = _finish;
        --tmp_back;
        return tmp_back;
    }

    return _insert_helper(where._const_cast(), std::forward<Args>(args)...);
}

template<typename _Type>
typename Deque<_Type>::iterator Deque<_Type>::erase(const_iterator where) {
    iterator next = where._const_cast();
    ++next;

    const ptrdiff_t idx = where - begin();
    if (static_cast<size_t>(idx) < (size() >> 1)) {
        if (where != begin())
            _move_backwards(begin(), where._const_cast(), next);
        pop_front();
    } else {
        if (next != end())
            _move(next, end(), where._const_cast());
        pop_back();
    }

    return begin() + idx;
}

template<typename _Type>
typename Deque<_Type>::iterator Deque<_Type>::erase(const_iterator first, const_iterator last) {
    if (first == last) {
        return first._const_cast();
    } else if (first == begin() && first == end()) {
        clear();
        return end();
    } else {
        const ptrdiff_t distance = last - first;
        const ptrdiff_t els_before = first - begin(); 

        // erase at first half
        if (static_cast<size_t>(els_before) <= (size() - distance) / 2) {
            if (first != begin())
                _move_backwards(begin(), first._const_cast(), last._const_cast());

            _destroy_elements(begin(), begin() + distance);
            iterator pos = begin() + distance; 
            _deallocate_nodes(_start._node, pos._node);
            _start = pos;
        
        // erase at second half
        } else {
            if (last != end())
                _move(last._const_cast(), end(), first._const_cast());
            
            _destroy_elements(end() - distance, end());
            iterator pos = end() - distance;
            _deallocate_nodes(pos._node + 1, _finish._node + 1);
            _finish = pos;
        }

        return begin() + els_before;
    }
}

template<typename _Type>
void Deque<_Type>::push_back(const _Type& value) {
    if (_finish._curr != _finish._last - 1) {
        ::new(static_cast<void*>(&*_finish._curr)) _Type(value);
        ++_finish._curr;
    } else {
        _reserve_map_at_back();
        *(_finish._node + 1) = static_cast<_Elptr>(::operator new(CHUNK_SIZE * sizeof(_Type)));
        ::new(static_cast<void*>(&*_finish._curr)) _Type(value);
        _finish._set_node(_finish._node + 1);
        _finish._curr = _finish._first;
    }
}

template<typename _Type>
void Deque<_Type>::push_back(_Type&& value) {
    emplace_back(std::move(value));
}

template<typename _Type>
template<typename... Args>
_Type& Deque<_Type>::emplace_back(Args&&... args) {
    if (_finish._curr != _finish._last - 1) {
        ::new(static_cast<void*>(&*_finish._curr)) _Type(std::forward<Args>(args)...);
        ++_finish._curr;
    } else {
        _reserve_map_at_back();
        *(_finish._node + 1) = static_cast<_Elptr>(::operator new(CHUNK_SIZE * sizeof(_Type)));
        ::new(static_cast<void*>(&*_finish._curr)) _Type(std::forward<Args>(args)...);
        _finish._set_node(_finish._node + 1);
        _finish._curr = _finish._first;
    }

    return back();
}

template<typename _Type>
void Deque<_Type>::pop_back() {
    if (_finish._curr != _finish._first) {
        --_finish._curr;
        (*_finish._curr).~_Type();
    } else {
        ::operator delete(&*_finish._first, CHUNK_SIZE * sizeof(_Type));
        _finish._set_node(_finish._node - 1);
        _finish._curr = _finish._last - 1;
        (*_finish._curr).~_Type();
    }
}

template<typename _Type>
void Deque<_Type>::push_front(const _Type& value) {
    if (_start._curr != _start._first) {
        ::new(static_cast<void*>(&*(_start._curr - 1))) _Type(value);
        --_start._curr;
    } else {
        _reserve_map_at_front();
        *(_start._node - 1) = static_cast<_Elptr>(::operator new(CHUNK_SIZE * sizeof(_Type)));
        _start._set_node(_start._node - 1);
        _start._curr = _start._last - 1;
        ::new(static_cast<void*>(&*_start._curr)) _Type(value);
    }
}

template<typename _Type>
void Deque<_Type>::push_front(_Type&& value) {
    emplace_front(std::move(value));
}

template<typename _Type>
template<typename... Args>
_Type& Deque<_Type>::emplace_front(Args&&... args) {
    if (_start._curr != _start._first) {
        ::new(static_cast<void*>(&*(_start._curr - 1))) _Type(std::forward<Args>(args)...);
        --_start._curr;
    } else {
        _reserve_map_at_front();
        *(_start._node - 1) = static_cast<_Elptr>(::operator new(CHUNK_SIZE * sizeof(_Type)));
        _start._set_node(_start._node - 1);
        _start._curr = _start._last - 1;
        ::new(static_cast<void*>(&*_start._curr)) _Type(std::forward<Args>(args)...);
    }

    return front();
}

template<typename _Type>
void Deque<_Type>::pop_front() {
    if (_start._curr != _start._last - 1) {
        (*_start._curr).~_Type();
        ++_start._curr;
    } else {
        (*_start._curr).~_Type();
        ::operator delete(&*_start._first, CHUNK_SIZE * sizeof(_Type));
        _start._set_node(_start._node + 1);
        _start._curr = _start._first;
    }
}

template<typename _Type>
void Deque<_Type>::resize(size_t new_size) {
    const size_t _size = size();
    if (new_size > _size) {
        for (size_t i = _size; i < new_size; ++i) 
            push_back(_Type());
    } else {
        for (size_t i = new_size; i < _size; ++i)
            pop_back();
    }
}

template<typename _Type>
void Deque<_Type>::resize(size_t new_size, const _Type& value) {
    const size_t _size = size();
    if (new_size > _size) {
        for (size_t i = _size; i < new_size; ++i) 
            push_back(value);
    } else {
        for (size_t i = new_size; i < _size; ++i)
            pop_back();
    }
}

template<typename _Type>
void Deque<_Type>::swap(Deque& other) {
    if (this != &other) {
        std::swap(_start, other._start);
        std::swap(_finish, other._finish);
        std::swap(_map, other._map);
        std::swap(_map_size, other._map_size);
    }
}

// definition of random access deque iterator class

template<typename _Type, typename _Ref, typename _Ptr>
class _DequeIterator {
public:
    friend class Deque<_Type>;

public:
    using iterator = _DequeIterator<_Type, _Type&, _Type*>;
    using const_iterator = _DequeIterator<_Type, const _Type&, const _Type*>;

    using iterator_category = std::random_access_iterator_tag;
    using value_type = _Type;
    using pointer = _Ptr;
    using reference = _Ref;
    using difference_type = ptrdiff_t;

    using _Elptr = _Type*;
    using _Mapptr = _Type**;
    using _Self = _DequeIterator;

public:
    _DequeIterator() : _curr(), _first(), _last(), _node() {}

    _DequeIterator(_Elptr el_ptr, _Mapptr map_ptr) : 
        _curr(el_ptr), 
        _first(*map_ptr), 
        _last(*map_ptr + CHUNK_SIZE), 
        _node(map_ptr) {}

    _DequeIterator(const iterator& deq_it) :
        _curr(deq_it._curr),
        _first(deq_it._first),
        _last(deq_it._last),
        _node(deq_it._node) {}
    
    reference operator*() const {
        return *_curr;
    }

    pointer operator->() const {
        return _curr;
    }

    _Self& operator++() {
        ++_curr;
        if (_curr == _last) {
            _set_node(_node + 1);
            _curr = _first;
        }
        return *this;
    }

    _Self operator++(int) {
        _Self tmp = *this;
        ++*this;
        return tmp;
    }

    _Self& operator--() {
        if (_curr == _first) {
            _set_node(_node - 1);
            _curr = _last;
        }
        --_curr;
        return *this;
    }

    _Self operator--(int) {
        _Self tmp = *this;
        --*this;
        return tmp;
    }

    _Self& operator+=(difference_type n) {
        const difference_type offset = n + (_curr - _first);
        if (offset >= 0 && offset < difference_type(CHUNK_SIZE))
            _curr += n;
        else {
            const difference_type node_offset = 
                            offset > 0 ? offset / difference_type(CHUNK_SIZE) 
                                       : -difference_type((-offset - 1) / CHUNK_SIZE) - 1;
            _set_node(_node + node_offset);
            _curr = _first + (offset - node_offset * difference_type(CHUNK_SIZE));
        }
        return *this;
    }   

    _Self operator+(difference_type n) const {
        _Self tmp = *this;
        return tmp += n;
    }

    _Self& operator-=(difference_type n) {
        return *this += -n;
    }

    _Self operator-(difference_type n) const {
        _Self tmp = *this;
        return tmp -= n;
    }

    reference operator[](difference_type n) const {
        return *(*this + n);
    }

    inline bool operator==(const iterator& rhs) const {
        return _curr == rhs._curr;
    }

    inline bool operator==(const const_iterator& rhs) const {
        return _curr == rhs._curr;
    }
    
    inline bool operator!=(const iterator& rhs) const {
        return !(_curr == rhs._curr);
    }

    inline bool operator!=(const const_iterator& rhs) const {
        return !(_curr == rhs._curr);
    }

    inline difference_type operator-(const iterator& rhs) const {
        return difference_type(CHUNK_SIZE) * (_node - rhs._node - 1) + (_curr - _first) + (rhs._last - rhs._curr);
    }

    inline difference_type operator-(const const_iterator& rhs) const {
        return difference_type(CHUNK_SIZE) * (_node - rhs._node - 1) + (_curr - _first) + (rhs._last - rhs._curr);
    }

private:
    iterator _const_cast() const {
        return iterator(_curr, _node);
    }

    void _set_node(_Mapptr new_node) {
        _node = new_node;
        _first = *new_node;
        _last = *new_node + difference_type(CHUNK_SIZE);
    }

public:
    _Elptr _curr; // current element in the chunk (_Type*)
    _Elptr _first; // first element of the chunk (_Type*)
    _Elptr _last; // last element of the chunk (_Type*)
    _Mapptr _node; // pointer in the range of the map (_Type**)
};

#endif // !DEQUE_H
