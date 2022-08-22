#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <algorithm>

// interface of an open ends doubly-linked list data structure

template<typename _ListIterValType, bool _IsConst>
class ListIterator;

template<typename _Type>
struct _ListNode {
    using _nodePtr = _ListNode<_Type>*;

    _ListNode(const _Type& value) : _data(value) {}

    _Type _data;
    _nodePtr _next{ nullptr };
    _nodePtr _prev{ nullptr };
};

template<typename _Type>
class List {
private:
    using _LNodePtr = _ListNode<_Type>*;

public:
    using iterator = ListIterator<_Type, false>;
    using const_iterator = ListIterator<_Type, true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // ctors
    List();

    explicit List(size_t count);

    List(size_t count, const _Type& value);

    List(const List& source);

    List(List&& source);

    List(std::initializer_list<_Type> ilist);

    // dtor
    ~List();

    // operator=
    List& operator=(const List& right);

    List& operator=(List&& right);

    // element access
    _Type& front();
    const _Type& front() const;

    _Type& back();
    const _Type& back() const;

    // iterators
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator crbegin() const;

    reverse_iterator rend();
    const_reverse_iterator rend() const;
    const_reverse_iterator crend() const;

    // capacity
    bool empty() const;

    size_t size() const;

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
    void push_front(_Type&& value);

    template<typename... Args>
    _Type& emplace_front(Args&&... args);

    void pop_front();

    void resize(size_t count);
    void resize(size_t count, const _Type& value);

    void swap(List& other);

    // operations
    void merge(List& source);

    template<typename Compare>
    void merge(List& source, Compare comp);

    void splice(const_iterator where, List& source);
    void splice(const_iterator where, List& source, const_iterator iter);
    void splice(const_iterator where, List& source, const_iterator first, const_iterator last);

    void remove(const _Type& value);

    template<typename UnaryPredicate>
    void remove_if(UnaryPredicate pred);
    
    void reverse();

    void unique();

    template<typename BinaryPredicate>
    void unique(BinaryPredicate bpred);

    void sort();

    template<typename Compare>
    void sort(Compare comp);

private:
    void _constructSentinels(const _Type& value);
    
    template<typename... Args>
    iterator _constructInternalNode(const_iterator where, Args&&... args);

    void _fillNodes(size_t count, const _Type& value);

    void _transferNodes(const_iterator where, const_iterator first, const_iterator last);

    void _invalidateList(List& source);

    void _unhook(const_iterator iter);

private:
    size_t _size; // the size of the List
    _LNodePtr _head; // _ListNode representing one before the first valid element - Sentinel
    _LNodePtr _tail; // _ListNode representing one after the last valid element - Sentinel
};

// List definition

template<typename _Type>
void List<_Type>::_constructSentinels(const _Type& value) {
    _tail = new _ListNode(value);
    _head = new _ListNode(value);

    // hook up both sentinels
    _head->_next = _tail;
    _tail->_prev = _head;

    _size = 0;
}

template<typename _Type>
template<typename... Args>
typename List<_Type>::iterator List<_Type>::_constructInternalNode(const_iterator where, Args&&... args) {
    _LNodePtr newMidNode = new _ListNode(std::forward<Args>(args)...);

    // hook up to the new node 
    newMidNode->_prev = where._nodePtr->_prev;
    newMidNode->_next = where._nodePtr;

    // rehook up the old where->_prev and the where itself
    where._nodePtr->_prev->_next = newMidNode;
    where._nodePtr->_prev = newMidNode;

    ++_size;

    return newMidNode;
}

template<typename _Type>
void List<_Type>::_fillNodes(size_t count, const _Type& value) {
    for (size_t i = 0; i < count; i++) {
        _constructInternalNode(cend(), value);
    }
}

template<typename _Type>
void List<_Type>::_transferNodes(const_iterator where, const_iterator first, const_iterator last) {
    // splice [first, last) before where
    if (where != first && where != last && first != last) {
        // hook up first
        first._nodePtr->_prev = where._nodePtr->_prev;
        where._nodePtr->_prev->_next = first._nodePtr;

        // hook up the prev of last
        last._nodePtr->_prev->_next = where._nodePtr;
        where._nodePtr->_prev = last._nodePtr->_prev;
    }
}

template<typename _Type>
void List<_Type>::_invalidateList(List& source) {
    // rehook-up sentinels
    source._head->_next = source._tail;
    source._tail->_prev = source._head;

    source._size = 0;
}

template<typename _Type>
void List<_Type>::_unhook(const_iterator iter) {
    // unhook node at iter position
    iter._nodePtr->_prev->_next = iter._nodePtr->_next;
    iter._nodePtr->_next->_prev = iter._nodePtr->_prev;

    --_size;
}

template<typename _Type>
List<_Type>::List() : _size(0), _head(nullptr), _tail(nullptr) {
    _constructSentinels(_Type());
} 

template<typename _Type>
List<_Type>::List(size_t count) : _size(0), _head(nullptr), _tail(nullptr) {
    _constructSentinels(_Type());
    _fillNodes(count, _Type());
}

template<typename _Type>
List<_Type>::List(size_t count, const _Type& value) {
    _constructSentinels(value);
    _fillNodes(count, value);
}

template<typename _Type>
List<_Type>::List(const List& source) : _size(0), _head(nullptr), _tail(nullptr) {
    _constructSentinels(_Type());
    for (auto it = source.begin(); it != source.end(); it++) {
        _constructInternalNode(cend(), *it);
    }
}

template<typename _Type>
List<_Type>::List(List&& source) : _size(0), _head(nullptr), _tail(nullptr) {
    _constructSentinels(_Type());
    splice(cend(), source);
}

template<typename _Type>
List<_Type>::List(std::initializer_list<_Type> ilist) :_size(0), _head(nullptr), _tail(nullptr) {
    _constructSentinels(_Type());
    for (auto it = ilist.begin(); it != ilist.end(); it++) {
        _constructInternalNode(cend(), *it);
    }
}

template<typename _Type>
List<_Type>::~List() {
    clear();
    delete _tail;
    delete _head;
}

template<typename _Type>
List<_Type>& List<_Type>::operator=(const List& right) {
    if (this != std::addressof(right)) {
        clear();
        for (auto it = right.begin(); it != right.end(); it++) {
            _constructInternalNode(cend(), *it);
        }
    }

    return *this;
}

template<typename _Type> 
List<_Type>& List<_Type>::operator=(List&& right) {
    if (this != std::addressof(right)) {
        clear();
        splice(cend(), right);
    }

    return *this;    
}

template<typename _Type>
_Type& List<_Type>::front() {
    if (empty()) {
        std::cerr << "front() called on empty List" << std::endl;
    }

    return _head->_next->_data;
}

template<typename _Type>
const _Type& List<_Type>::front() const {
    if (empty()) {
        std::cerr << "front() called on empty List" << std::endl;
    }
    
    return _head->_next->_data;
}

template<typename _Type>
_Type& List<_Type>::back() {
    if (empty()) {
        std::cerr << "back() called on empty List" << std::endl;
    }
    
    return _tail->_prev->_data;
}

template<typename _Type>
const _Type& List<_Type>::back() const {
    if (empty()) {
        std::cerr << "back() called on empty List" << std::endl;
    }
    
    return _tail->_prev->_data;
}

template<typename _Type>
typename List<_Type>::iterator List<_Type>::begin() {
    return iterator(_head->_next);
}

template<typename _Type>
typename List<_Type>::const_iterator List<_Type>::begin() const {
    return const_iterator(_head->_next);
}

template<typename _Type>
typename List<_Type>::const_iterator List<_Type>::cbegin() const {
    return const_iterator(_head->_next);
}

template<typename _Type>
typename List<_Type>::iterator List<_Type>::end() {
    return iterator(_tail);
}

template<typename _Type>
typename List<_Type>::const_iterator List<_Type>::end() const {
    return const_iterator(_tail);
}

template<typename _Type>
typename List<_Type>::const_iterator List<_Type>::cend() const {
    return const_iterator(_tail);
}

template<typename _Type>
typename List<_Type>::reverse_iterator List<_Type>::rbegin() {
    return reverse_iterator(end());
}

template<typename _Type>
typename List<_Type>::const_reverse_iterator List<_Type>::rbegin() const {
    return const_reverse_iterator(end());
}

template<typename _Type>
typename List<_Type>::const_reverse_iterator List<_Type>::crbegin() const {
    return const_reverse_iterator(end());
}

template<typename _Type>
typename List<_Type>::reverse_iterator List<_Type>::rend() {
    return reverse_iterator(begin());
}

template<typename _Type>
typename List<_Type>::const_reverse_iterator List<_Type>::rend() const {
    return const_reverse_iterator(begin());
}

template<typename _Type>
typename List<_Type>::const_reverse_iterator List<_Type>::crend() const {
    return const_reverse_iterator(begin());
}

template<typename _Type>
bool List<_Type>::empty() const {
    return size() == 0;
}

template<typename _Type>
size_t List<_Type>::size() const {
    return _size;
}

template<typename _Type>
void List<_Type>::clear() {
    while (!empty()) {
        pop_front();
    }
}

template<typename _Type>
typename List<_Type>::iterator List<_Type>::insert(const_iterator where, const _Type& value) {
    if (!where._nodePtr) { // nullptr
        std::cerr << "insert() called with invalid position" << std::endl;
        return end();
    }

    if (empty()) { // always insert
        where = cend();
    }

    return _constructInternalNode(where, value);
}

template<typename _Type>
typename List<_Type>::iterator List<_Type>::insert(const_iterator where, _Type&& value) {
    if (!where._nodePtr) { // nullptr or empty List
        std::cerr << "insert() called with invalid position" << std::endl;
        return end();
    }

    if (empty()) { // always insert
        where = cend();
    }

    return _constructInternalNode(where, std::move(value));
}

template<typename _Type>
typename List<_Type>::iterator List<_Type>::insert(const_iterator where, size_t count, const _Type& value) {
    for (size_t i = 0; i < count; i++) {
        insert(where, value);
    }

    return where._nodePtr->_prev;
}

template<typename _Type>
template<typename... Args>
typename List<_Type>::iterator List<_Type>::emplace(const_iterator where, Args&&... args) {
    if (!where._nodePtr) { // nullptr
        std::cerr << "emplace() called with invalid position" << std::endl;
        return end();
    }
    
    return _constructInternalNode(where, _Type(std::forward<Args>(args)...));
}

template<typename _Type>
typename List<_Type>::iterator List<_Type>::erase(const_iterator where) {
    if (empty()) {
        std::cerr << "erase() called on empty List" << std::endl;
        return end();
    }

    if (where == cend()) {
        return iterator(where._nodePtr);
    }

    _LNodePtr retiredNode = where._nodePtr;

    where._nodePtr = where._nodePtr->_next;

    // unhook target node for delete
    _unhook(retiredNode);

    delete retiredNode;

    return iterator(where._nodePtr);
}

template<typename _Type>
typename List<_Type>::iterator List<_Type>::erase(const_iterator first, const_iterator last) {
    for (; first != last; ) {
        first = static_cast<const_iterator>(erase(first));
    }
    
    return iterator(last._nodePtr);
}

template<typename _Type>
void List<_Type>::push_back(const _Type& value) {
    _constructInternalNode(cend(), value);
}

template<typename _Type>
void List<_Type>::push_back(_Type&& value)  {
    _constructInternalNode(cend(), std::move(value));
}

template<typename _Type>
template<typename... Args>
_Type& List<_Type>::emplace_back(Args&&... args) {
    _constructInternalNode(cend(), _Type(std::forward<Args>(args)...));
    return back();
}

template<typename _Type>
void List<_Type>::pop_back() {
    _LNodePtr backNode = _tail->_prev;
    _unhook(backNode);
    delete backNode;
}

template<typename _Type>
void List<_Type>::push_front(const _Type& value) {
    _constructInternalNode(cbegin(), value);
}

template<typename _Type>
void List<_Type>::push_front(_Type&& value) {
    _constructInternalNode(cbegin(), std::move(value));
}

template<typename _Type>
template<typename... Args>
_Type& List<_Type>::emplace_front(Args&&... args) {
    _constructInternalNode(cbegin(), _Type(std::forward<Args>(args)...));
    return front();
}

template<typename _Type>
void List<_Type>::pop_front() {
    _LNodePtr frontNode = _head->_next;
    _unhook(frontNode);
    delete frontNode;
}

template<typename _Type>
void List<_Type>::resize(size_t count) {
    resize(count, _Type());
}

template<typename _Type>
void List<_Type>::resize(size_t count, const _Type& value) {
    if (count < size()) {
        while (count != size()) {
            pop_back();
        }
    } else {
        while (count != size()) {
            _constructInternalNode(cend(), value);
        }
    }
}

template<typename _Type>
void List<_Type>::swap(List& other) {
    if (this != std::addressof(other)) {
        std::swap(_head, other._head);
        std::swap(_tail, other._tail);
        std::swap(_size, other._size);
    }
}

template<typename _Type>
void List<_Type>::merge(List& source) {
    if (!source.empty() && this != std::addressof(source)) {
        iterator first = begin();
        iterator last = end();
        iterator sourceFirst = source.begin();
        iterator sourceLast = source.end();

        while (first != last && sourceFirst != sourceLast) {
            if (*sourceFirst < *first) {
                iterator nextSourceFirst = sourceFirst;
                _transferNodes(first, sourceFirst, ++nextSourceFirst);
                sourceFirst = nextSourceFirst;
            } else {
                ++first;
            }
        }

        if (sourceFirst != sourceLast) {
            _transferNodes(last, sourceFirst, sourceLast);
        }

        _size += source.size();
        _invalidateList(source);
    }
}

template<typename _Type>
template<typename Compare>
void List<_Type>::merge(List& source, Compare comp) {
    if (!source.empty() && this != std::addressof(source)) {
        iterator first = begin();
        iterator last = end();
        iterator sourceFirst = source.begin();
        iterator sourceLast = source.end();

        while (first != last && sourceFirst != sourceLast) {
            if (comp(*sourceFirst, *first)) {
                iterator nextSourceFirst = sourceFirst;
                _transferNodes(first, sourceFirst, ++nextSourceFirst);
                sourceFirst = nextSourceFirst;
            } else {
                ++first;
            }
        }

        if (sourceFirst != sourceLast) {
            _transferNodes(last, sourceFirst, sourceLast);
        }

        _size += source.size();
        _invalidateList(source);
    }
}

template<typename _Type>
void List<_Type>::splice(const_iterator where, List& source) {
    if (!source.empty() && this != std::addressof(source)) {
        _transferNodes(where, source.cbegin(), source.cend());
        _size += source.size();
        _invalidateList(source);
    }
}   

template<typename _Type>
void List<_Type>::splice(const_iterator where, List& source, const_iterator iter) {
    if (!source.empty() && this != std::addressof(source)) {
        source._unhook(iter);

        // hook up target node
        iter._nodePtr->_prev = where._nodePtr->_prev;
        iter._nodePtr->_next = where._nodePtr;
        
        // rehook up the old where->_prev and the where itself
        where._nodePtr->_prev->_next = iter._nodePtr;
        where._nodePtr->_prev = iter._nodePtr;

        _size++;
    }
}

template<typename _Type>
void List<_Type>::splice(const_iterator where, List& source, const_iterator first, const_iterator last) {
    if (!source.empty() && this != std::addressof(source)) {
        while (first != last) {
            splice(where, source, first++);
        }
    }
}

template<typename _Type>
void List<_Type>::remove(const _Type& value) {
    iterator first = begin();
    iterator last = end();

    while (first != last) {
        iterator next = first;
        ++next;
        if (*first == value) {
            erase(first);
        }
        first = next;
    }
}

template<typename _Type>
template<typename UnaryPredicate>
void List<_Type>::remove_if(UnaryPredicate pred) {
    iterator first = begin();
    iterator last = end();

    while (first != last) {
        iterator next = first;
        ++next;
        if (pred(*first)) {
            erase(first);
        }
        first = next;
    }    
}

template<typename _Type>
void List<_Type>::reverse() {
    iterator first = begin();
    iterator last = end();

    if (first == last) {
        return;
    }

    // std::iter_swap swaps the values pointed by first and last
    while (first != last && first != --last) {
        std::iter_swap(first++, last);
    }
}

template<typename _Type>
void List<_Type>::unique() {
    iterator first = begin();
    iterator last = end();

    if (first == last) {
        return;
    }

    iterator next = first;
    while (++next != last) {
        if (*first == *next) {
            erase(next);
        } else {
            first = next;
        }
        next = first;
    }
}

template<typename _Type>
template<typename BinaryPredicate>
void List<_Type>::unique(BinaryPredicate bpred) {
    iterator first = begin();
    iterator last = end();

    if (first == last) {
        return;
    }

    iterator next = first;
    while (++next != last) {
        if (bpred(*first, *next)) {
            erase(next);
        } else {
            first = next;
        }
        next = first;
    }
}

template<typename _Type>
void List<_Type>::sort() {
    // stolen from glibc
    List carry;
    List buckets[64];
    List* fill = buckets;
    List* counter;

    do {
        carry.splice(carry.begin(), *this, begin());

        for (counter = buckets;  counter != fill && !counter->empty(); ++counter) {
            counter->merge(carry);
            carry.swap(*counter);
        }

        carry.swap(*counter);

        if (counter == fill) {
            ++fill;
        }

    } while (!empty());

    for (counter = buckets + 1; counter != fill; ++counter) {
        counter->merge(*(counter - 1));
    }

    swap(*(fill - 1));
}

// definition of List bidirectional iterator

template<typename _ListIterValType, bool _IsConst>
class ListIterator {
private:
    friend class List<_ListIterValType>;

public:
    using _Iterator = ListIterator<_ListIterValType, _IsConst>;
    using _NodePtr = _ListNode<_ListIterValType>*;

    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = _ListIterValType;
    using difference_type = std::ptrdiff_t;
    using pointer = typename std::conditional_t<_IsConst, const value_type*, value_type>;
    using reference = typename std::conditional_t<_IsConst, const value_type&, value_type&>;

    // ctors
    ListIterator(_NodePtr& nodePtr) : _nodePtr(nodePtr) {}

    ListIterator(const _NodePtr& nodePtr) : _nodePtr(nodePtr) {}

    // indirection operator
    template<bool _Conts = _IsConst>
    std::enable_if_t<_Conts, reference> operator*() const {
        return _nodePtr->_data;
    }

    template<bool _Conts = _IsConst>
    std::enable_if_t<!_Conts, reference> operator*() const {
        return _nodePtr->_data;
    }

    // member of pointer operator
    template<bool _Conts = _IsConst>
    std::enable_if_t<_Conts, pointer> operator->() const {
        return std::addressof(_nodePtr->_data);
    }

    template<bool _Conts = _IsConst>
    std::enable_if_t<!_Conts, pointer> operator->() const {
        return std::addressof(_nodePtr->_data);
    }

    // iterator increment
    _Iterator& operator++() { // prefix
        _nodePtr = _nodePtr->_next;
        return *this;
    }

    _Iterator operator++(int) { // postfix
        _Iterator tmp = *this;
        ++*this;
        return tmp;
    }

    // iterator decrement
    _Iterator& operator--() { // prefix
        _nodePtr = _nodePtr->_prev;
        return *this;
    }

    _Iterator operator--(int) { // postfix
        _Iterator tmp = *this;
        --*this;
        return tmp;
    }

    // equality operator
    bool operator==(const _Iterator& right) {
        return _nodePtr == right._nodePtr;
    }

    // inequality operator
    bool operator!=(const _Iterator& right) {
        return !(*this == right);
    }
    
    // overloading typecast operator -> cast iterator to const_interator
    operator ListIterator<_ListIterValType, true>() const { 
        return _nodePtr; 
    }

protected:
    _NodePtr _nodePtr {}; // pointer to the current _ListNode
};

#endif // !LIST_H
