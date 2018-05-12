#ifndef VECTOR_H
#define VECTOR_H

#include <type_traits>
#include <cstring>
#include <algorithm>
#include <cmath>

using std::size_t;

namespace __vector_impl {
    template<typename T, typename InIter>
    typename std::enable_if<std::is_trivially_destructible<T>::value, void>::type
    destruct(InIter begin_iter, InIter end_iter) {}

    template<typename T, typename InIter>
    typename std::enable_if<!std::is_trivially_destructible<T>::value, void>::type
    destruct(InIter begin_iter, InIter end_iter) {
        std::for_each(begin_iter, end_iter, [] (T elem) {
            elem.~T();
        });
    }

    template<typename T>
    typename std::enable_if<std::is_trivially_constructible<T>::value, void>::type
    construct(T* begin_iter, T* end_iter) {
        memset(begin_iter, 0, (end_iter - begin_iter) * sizeof(T));
    }

    template<typename T>
    typename std::enable_if<!std::is_trivially_constructible<T>::value, void>::type
    construct(T* begin_iter, T* end_iter) {
        for (; begin_iter != end_iter; ++begin_iter) {
            new (begin_iter) T();
        }
    }

    template<typename T>
    void finalize(T* ptr, size_t size) {
        destruct<T>(ptr + 0, ptr + size);
        operator delete(ptr);
    }
}

template<typename T>
class vector {
    T* m_ptr;
    size_t sz;
    size_t cap;

    void moveTo(T * t_ptr) {
        std::copy(m_ptr, m_ptr + sz, t_ptr);

        __vector_impl::finalize(m_ptr, size());

        m_ptr = t_ptr;
    }

public:
    const double resize_multiplier = 1.75;

    typedef T * iterator;
    typedef T const* const_iterator;

    vector() noexcept : m_ptr(nullptr), sz(0), cap(0) {}

    explicit vector(size_t t_size) : vector() {
        resize(t_size);
    }

    vector(size_t t_size, T value) : m_ptr(operator new(sizeof(T) * t_size)), sz(t_size), cap(t_size) {
        for (size_t i = 0; i < size(); ++i) {
            new (m_ptr + i) T(value);
        }
    }

    vector(vector<T> const& rhs) : m_ptr((T*) operator new(sizeof(T) * rhs.sz)), sz(rhs.sz), cap(rhs.sz) {
        for (size_t i = 0; i < sz; ++i) {
            new (m_ptr + i) T(rhs[i]);
        }
    }

    template<typename Type, typename II>
    vector(II begin_iter, II end_iter) {
        vector<Type> tmp;
        while (begin_iter != end_iter) {
            tmp.push_back(*begin_iter);
            ++begin_iter;
        }

        swap(*this, tmp);
    }

    template<typename Type >
    vector(Type const* begin_iter, Type const* end_iter) : m_ptr(nullptr), sz(0), cap(0) {
        if (begin_iter == end_iter) { return; }

        vector<Type> tmp;
        tmp.reserve(end_iter - begin_iter);
        tmp.sz = end_iter - begin_iter;
        memcpy(tmp.m_ptr, begin_iter, (end_iter - begin_iter) * sizeof(Type));

        swap(*this, tmp);
    }

    vector<T>& operator=(vector<int> const& rhs) {
        vector<T> tmp(rhs);
        swap(*this, rhs);
    }

    ~vector() {
        __vector_impl::finalize(m_ptr, size());
    }

    iterator begin() {
        return m_ptr;
    }

    const_iterator begin() const {
        return m_ptr;
    }

    iterator end() {
        return m_ptr + sz;
    }

    const_iterator end() const {
        return m_ptr + sz;
    }

    size_t size() const {
        return sz;
    }

    size_t capacity() const {
        return cap;
    }

    void shrink_to_fit() {
        moveTo((T*) operator new(sizeof(T) * size()));
        cap = size();
    }

    void try_fit() {
        if (sz < (cap >> 2)) {
                size_t new_cap = cap;
                while (sz < (static_cast<size_t>(ceil(cap / resize_multiplier)) >> 2)) {
                    cap = static_cast<size_t>(ceil(cap / resize_multiplier));
                }

                moveTo((T*) operator new(sizeof(T) * new_cap));
                cap = new_cap;
            }
    }

    void resize(size_t t_size) {
        if (t_size < size()) {
            __vector_impl::destruct<T>(m_ptr + t_size, m_ptr + size());

            sz = t_size;

            try_fit();
        } else {
            if (t_size > capacity()) {
                reserve(t_size);
            }

            __vector_impl::construct(m_ptr + size(), m_ptr + t_size);
            sz = t_size;
        }
    }

    void resize(size_t t_size, T value) {
        if (t_size < size()) {
            __vector_impl::destruct<T>(m_ptr + t_size, m_ptr + sz);
            sz = t_size;

            try_fit();
        } else {
            if (t_size > capacity()) {
                reserve(t_size);
            }

            for (size_t i = size(); i < t_size; ++i) {
                new (m_ptr + i) T(value);
            }
            sz = t_size;
        }
    }

    void push_back(T const& value) {
        T tmp_copy(value);
        resize(sz + 1, tmp_copy);
    }

    void pop_back() {
        sz = std::max(sz - 1, static_cast<size_t >(0));
    }

    T& back() {
        return m_ptr + (sz - 1);
    }

    T const & back() const {
        return m_ptr + (sz - 1);
    }

    bool empty() const {
        return (sz == 0);
    }

    T& operator[](size_t pos) {
        return m_ptr[pos];
    }

    T* data() {
        return m_ptr;
    }

    T const* data() const {
        return m_ptr;
    }

    void assign(size_t n) {
        vector<T> tmp(*this);
        __vector_impl::destruct<T>(tmp.m_ptr, tmp.m_ptr + sz);
        tmp.sz = 0;
        tmp.resize(n);
        swap(*this, tmp);
    }

    void assign(size_t n, T value) {
        vector<T> tmp(*this);
        __vector_impl::destruct<T>(tmp.m_ptr, tmp.m_ptr + sz);
        tmp.sz = 0;
        tmp.resize(n, value);
        swap(*this, tmp);
    }

    template<typename It>
    void assign(It first, It last) {
        vector<T> tmp(first, last);
        swap(tmp, *this);
    }

    void clear() {
        __vector_impl::destruct<T>(m_ptr, m_ptr + sz);
        sz = std::min(sz, static_cast<size_t>(64));
        try_fit();
        sz = 0;
    }

    void insert(const_iterator t_pos, T const& val) {
        vector<T> tmp(*this);
        size_t pos = t_pos - tmp.m_ptr;
        T tmp_val(val);

        tmp.reserve(tmp.sz + 1);
        for (size_t i = tmp.sz + 1; i > pos; --i) {
            tmp.m_ptr[i] = tmp.m_ptr[i - 1];
        }

        tmp.m_ptr[pos] = tmp_val;
        swap(*this, tmp);
    }

    void reserve(size_t n) {
        if (cap < n) {
            size_t new_cap = std::max(cap, static_cast<size_t>(1));
            while (new_cap < n) {
                new_cap = static_cast<size_t>(ceil(new_cap * resize_multiplier));
            }

            moveTo((T*) operator new(sizeof(T) * new_cap));
            cap = new_cap;
        }
    }

    T const& operator[](size_t pos) const {
        return m_ptr[pos];
    }

    template<typename Type>
    friend void swap(vector<Type> & a, vector<Type> & b);

    template<typename Type>
    friend bool operator==(vector<Type> const& lhs, vector<Type> const& rhs);
};

template<typename T>
bool operator==(vector<T> const& lhs, vector<T> const& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }

    for (size_t i = 0; i < lhs.size(); i++) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }

    return true;
}

template<typename Type>
void swap(vector<Type> & a, vector<Type> & b) {
    std::swap(a.m_ptr, b.m_ptr);

    a.sz ^= b.sz;
    b.sz ^= a.sz;
    a.sz ^= b.sz;

    a.cap ^= b.cap;
    b.cap ^= a.cap;
    a.cap ^= b.cap;
}

#endif