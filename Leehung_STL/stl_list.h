#pragma once
#include "stl_config.h"
#include "stl_alloc.h"
#include "stl_iterator.h"
#include "stl_uninitialized.h"

__STL_BEGIN_NAMESPACE
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template<class _Tp>
struct _List_node : public _List_node_base {
    _Tp _M_data;
};

struct _List_iterator_base {
    typedef size_t                      size_type;
    typedef ptrdiff_t                   difference_type;
    typedef bidirectional_iterator_tag  iterator_category;

    _List_node_base* _M_node;

    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    _List_iterator_base() {}

    void _M_incr() {
        _M_node = _M_node->_M_next;
    }

    void _M_decr() {
        _M_node = _M_node->_M_prev;
    }

    bool operator==(const _List_iterator_base& __x) const {
        return _M_node == __x._M_node;
    }

    bool operator!=(const _List_iterator_base& __x) const {
        return _M_node != __x._M_node;
    }
};

template<class _Tp, class _Ref, class _Ptr>
struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Tp&, _Tp*>             iterator;
    typedef _List_iterator<_Tp, const _Tp&, const _Tp*> const_iterator;
    typedef _List_iterator<_Tp, _Ref, _Ptr>             _Self;

    typedef _Tp     value_type;
    typedef _Ptr    pointer;
    typedef _Ref    reference;
    typedef _List_node<_Tp> _Node;

    // 构造函数
    _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
    _List_iterator() {}
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}

    reference operator*() const {return ((_Node*)_M_node)->_M_data;}

    pointer operator->() const {return &(operator*()); }

    _Self& operator++() {
        this->_M_incr();
        return *this;
    }

    _Self operator++(int) {
        _Self __tmp = *this;
        this->_M_incr;
        return __tmp;
    }

    _Self& operator--() {
        this->_M_decr;
        return *this;
    }

    _Self operator--(int) {
        _Self __tmp = *this;
        this->_M_decr;
        return __tmp;
    }
};

inline bidirectional_iterator_tag
iterator_category(const _List_iterator_base&) {
    return bidirectional_iterator_tag();
}

template<class _Tp, class _Ref, class _Ptr>
inline _Tp* value_type(const _List_iterator<_Tp, _Ref, _Ptr>&) { return 0; }

inline ptrdiff_t*
distance_type(const _List_iterator_base&)
{
    return 0;
}

template<class _Tp, class _Alloc>
class _List_base {
public:
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }

    _List_base(const allocator_type&) {
        _M_node = _M_get_node();
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }

    ~_List_base() {
        clear();
        _M_put_node(_M_node);
    }

    void clear();

protected:
    typedef simple_alloc<_List_node<_Tp>, _Alloc> _Alloc_type;
    _List_node<_Tp>* _M_get_node() { return _Alloc_type::allcate(1); }
    void _M_put_node(_List_node<_Tp>* __p) { _Alloc_type::deallocate(__p, 1); }

protected:
    _List_node<_Tp*> _M_node;
};

template<class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear() {
    _List_node<_Tp*> __cur = (_List_node<_Tp>*)_M_node->_M_next;
    while (__cur != _M_node) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*) __cur->_M_next;
        _Destroy(&tmp->_M_data);
        _M_put_node(__tmp);
    }
    _M_node->_M_next = _M_node;
    _M_node->_M_prev = _M_node;
}

template<class _Tp, class _Alloc = alloc>
class list : protected _List_base<_Tp, _Alloc> {
    typedef _List_base<_Tp, _Alloc> _Base;

protected:
    typedef void* _Void_pointer;

public:
    typedef _Tp value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef _List_node<_Tp> _Node;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef typename _Base::allocator_type allocator_type;
    allocator_type get_allocator() cosnt { return _Base::get_allocator(); }

public:
    typedef _List_iterator<_Tp, _Tp&, _Tp*> iterator;
    typedef _List_iterator<_Tp, cosnt _Tp&, const _Tp*> const_iterator;
    typedef reverse_bidirectional_iterator<const_iterator, value_type, const_reference, 
        difference_type> cosnt_reverse_iterator;
    typedef reverse_bidirectional_iterator<iterator, value_type, reference,
        difference_type> reverse_iterator;
    
protected:
    using _Base::_M_node;
    using _Base::_M_put_node;
    using _Base::_M_get_node;

protected:
    _Node* _M_create_node(const _Tp& __x) {
        _Node* __p = _M_get_node();
        __STL_TRY {
            _Construct(&__p->_M_data, __x);
        }
        __STL_UNWIND(_M_put_node(__p));
        return __p;
    }

    _Node* _M_create_node()
    {
        _Node* __p = _M_get_node();
        __STL_TRY{
          _Construct(&__p->_M_data);
        }
        __STL_UNWIND(_M_put_node(__p));
        return __p;
    }

public:
    explicit list(const allocator_type& __a = allocator_type()) : _Base(__a) {}

    iterator begin() { return (_Node*)(_M_node->_M_next); }
    const_iterator begin() const { return (_Node*)(_M_node->_M_next); }

    iterator end() { return _M_node; }
    const_iterator end() const { return _M_node; }

    reverse_iterator rbegin()
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const
    {
        return const_reverse_iterator(end());
    }

     reverse_iterator rend()
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const
    {
        return const_reverse_iterator(begin());
    }

    bool empty() const { return _M_node->_M_next == _M_node; }

    size_type size() const {
        size_type __result = 0;
        distance(begin(), end(), __result);
        return __result;
    }

    size_type max_size() const { return size_type(-1); }

    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }
    reference back() { return *(--end()); }
    const_reference back() const { return *(--end()); }

    void swap(list<_Tp, _Alloc>& __x) { __STL_NAME::swap(_M_node, __x._M_node); }

    iterator insert(iterator __position, const _Tp& __x) {
        _Node* __tmp = _M_create_node(__x);
        __tmp->_M_next = __position._M_node;
        __tmp->_M_prev = __position._M_node->_M_prev;
        __position._M_node->_M_prev->_M_next = __tmp;
        __position._M_node->_M_prev = __tmp;
        return __tmp;
    }

    iterator insert(iterator __position) { return insert(__position, _Tp()); }

    void insert(iterator __position, const _Tp* __first, const _Tp* __last);

    void insert(iterator __position,
        const_iterator __first, const_iterator __last);

    void insert(iterator __pos, size_type __n, const _Tp& __x)
    {
        _M_fill_insert(__pos, __n, __x);
    }

    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);

    void push_front(const _Tp& __x) { insert(begin(), __x); }
    void push_front() { insert(begin()); }
    void push_back(const _Tp& __x) { insert(end(), __x); }
    void push_back() { insert(end()); }

    iterator erase(iterator __position) {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        _M_put_node(__n);
        return iterator((_Node*)__next_node);
    }

    iterator erase(iterator __first, iterator __last);
    void clear() { _Base::clear(); }

    void resize(size_type __new_size, const _Tp& __x);
    void resize(size_type __new_size) { this->resize(__new_size, _Tp()); }

    void pop_front() { erase(begin()); }

    void pop_back() {
        iterator __tmp = end();
        erase(--__tmp);
    }

    // 构造函数
    list(size_type __n, const _Tp& __value,
        const allocator_type& __a = allocator_type())
        : _Base(__a)
    {
        insert(begin(), __n, __value);
    }

    explicit list(size_type __n)
        : _Base(allocator_type())
    {
        insert(begin(), __n, _Tp());
    }

    list(const _Tp* __first, const _Tp* __last,
        const allocator_type& __a = allocator_type())
        : _Base(__a)
    {
        this->insert(begin(), __first, __last);
    }
    list(const_iterator __first, const_iterator __last,
        const allocator_type& __a = allocator_type())
        : _Base(__a)
    {
        this->insert(begin(), __first, __last);
    }

    list(const list<_Tp, _Alloc>& __x) : _Base(__x.get_allocator())
    {
        insert(begin(), __x.begin(), __x.end());
    }

    ~list() { }

    list<_Tp, _Alloc>& operator=(const list<_Tp, _Alloc>& __x);
public:

    void assign(size_type __n, const _Tp& __val) { _M_fill_assign(__n, __val); }

    void _M_fill_assign(size_type __n, const _Tp& __val);

protected:
    void transfer(iterator __position, iterator __first, iterator __last) {
        if (__position != __last) {
            // Remove [first, last) from its old position.
            __last._M_node->_M_prev->_M_next = __position._M_node;
            __first._M_node->_M_prev->_M_next = __last._M_node;
            __position._M_node->_M_prev->_M_next = __first._M_node;

            // Splice [first, last) into its new position.
            _List_node_base* __tmp = __position._M_node->_M_prev;
            __position._M_node->_M_prev = __last._M_node->_M_prev;
            __last._M_node->_M_prev = __first._M_node->_M_prev;
            __first._M_node->_M_prev = __tmp;
        }
    }

public:
    void splice(iterator __position, list& __x) {
        if (!__x.empty()) this->transfer(__position, __x.begin(), __x.end());
    }

    void splice(iterator __position, list&, iterator __i) {
        iterator __j = __i;
        ++__j;
        if (__position == __i || __position == __j) return;
        this->transfer(__position, __i, __j);
    }

    void splice(iterator __position, list&, iterator __first, iterator __last) {
        if (__first != __last)
            this->transfer(__position, __first, __last);
    }

    void remove(const _Tp& __value);
    void unique();
    void merge(list& __x);
    void reverse();
    void sort();
};






__STL_END_NAMESPACE