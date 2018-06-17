//
// Created by starorpheus on 17/06/18.
//

#ifndef CPP_EXAM_SET_H
#define CPP_EXAM_SET_H
#include <memory>
#include <utility>

using std::unique_ptr;

//namespace exam {
//    template<typename T>
//    class set {
//        struct tree_node {
//            const T val;
//            unique_ptr<tree_node > left;
//            unique_ptr<tree_node > right;
//            tree_node * upper;
//            bool is_left;
//            int prior;
//
//            tree_node(T const& val) = delete;
//            tree_node(T const& val, tree_node * const upper, bool is_left);
//            tree_node(T const& val, tree_node * const upper, bool is_left, int pr);
//            tree_node(T&& val, tree_node * const upper, bool is_left);
//        };
//
//        typedef tree_node * node_ptr;
//
//        struct c_iterator : std::iterator<std::bidirectional_iterator_tag, const T> {
//            set<T> const * current_set = nullptr;
//
//            c_iterator operator++();
//            c_iterator operator--();
//            c_iterator operator++(int);
//            c_iterator operator--(int);
//
//            T const & operator*() const;
//
//            c_iterator(c_iterator const& rhs);
//            c_iterator & operator=(c_iterator const& rhs);
//
//        private:
//            node_ptr ptr;
//            c_iterator(node_ptr const ptr, set<T> const * cur_st);
//
//            friend class set<T>;
//
//            friend bool operator==(c_iterator const& a, c_iterator const& b);
//            friend bool operator!=(c_iterator const& a, c_iterator const& b);
//        };
//
//    public:
//        typedef c_iterator const_iterator;
//        typedef const_iterator iterator;
//        typedef std::reverse_iterator<iterator> reverse_iterator;
//        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
//
//    private:
//        node_ptr deep_copy(node_ptr ptr, node_ptr const up_ptr, bool is_left);
//
//        node_ptr merge(node_ptr lhs, node_ptr rhs);
//        std::pair<node_ptr, node_ptr> split(node_ptr root, T const& key);
//
//        unique_ptr<tree_node> root;
//        size_t sz;
//        std::less<T> t_less;
//
//        bool t_greater(T const& lhs, T const& rhs) const;
//        bool t_equal(T const& lhs, T const& rhs) const;
//        bool t_leq(T const& lhs, T const& rhs) const;
//
//        node_ptr next(node_ptr ptr) const;
//        node_ptr prev(node_ptr ptr) const;
//
//        const_iterator minimum(node_ptr ptr) const;
//        iterator minimum(node_ptr ptr);
//
//        const_iterator maximum(node_ptr ptr) const;
//        iterator maximum(node_ptr ptr);
//
//        bool leaf(node_ptr const ptr) const;
//
//        void validate_tree(node_ptr ptr, node_ptr upper, bool is_left);
//        void validate_tree();
//
//    public:
//
//        const_iterator minimum() const;
//        const_iterator maximum() const;
//
//        iterator begin();
//        const_iterator begin() const;
//
//        iterator end();
//        const_iterator end() const;
//
//        const_iterator cbegin() const;
//        const_iterator cbegin();
//
//        const_iterator cend() const;
//        const_iterator cend();
//
//        reverse_iterator rbegin();
//        const_reverse_iterator rbegin() const;
//
//        reverse_iterator rend();
//        const_reverse_iterator rend() const;
//
//        const_reverse_iterator crbegin() const;
//        const_reverse_iterator crend() const;
//
//        set() noexcept;
//        set(set<T> const& rhs);
//        set(set<T>&& rhs) noexcept;
//
//        set<T>& operator=(set<T> const& rhs);
//
//        set<T>& operator=(set<T>&& rhs) noexcept;
//
//        iterator erase(const_iterator x);
//        const_iterator find(T const& val) const;
//
//        const_iterator lower_bound(T const& val) const;
//        const_iterator upper_bound(T const& val) const;
//
//        bool empty() const;
//
//        void clear();
//
//        ~set() noexcept = default;
//
//        friend void swap(set<T> & x, set<T> & y);
//    };
//}

#include "../impl/set_impl.h"

#endif //CPP_EXAM_SET_H
