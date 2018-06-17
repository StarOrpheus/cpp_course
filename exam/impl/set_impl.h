#ifndef CPP_EXAM_SET_IMPL_H
#define CPP_EXAM_SET_IMPL_H

#include <utility>
#include <memory>
#include <cassert>
#include <limits>

using std::unique_ptr;

namespace exam {
    template<typename T>
    class set {
        struct tree_node {
            const T val;
            unique_ptr<tree_node > left;
            unique_ptr<tree_node > right;
            tree_node * upper;
            bool is_left;
            int prior;

            tree_node(T const& val) = delete;
            tree_node(T const& val, tree_node * const upper, bool is_left)
                    : val(val), upper(upper), is_left(is_left), prior(rand()) {}
            tree_node(T const& val, tree_node * const upper, bool is_left, int pr)
                    : val(val), upper(upper), is_left(is_left), prior(pr) {}
            tree_node(T&& val, tree_node * const upper, bool is_left)
                    : val(val), upper(upper), is_left(is_left), prior(rand()) {}
        };

        typedef tree_node * node_ptr;

        struct c_iterator : std::iterator<std::bidirectional_iterator_tag, const T> {
            set<T> const * current_set = nullptr;

            c_iterator operator++() {
                ptr = current_set->next(ptr);
                return *this;
            }

            c_iterator operator--() {
                if (ptr == nullptr) {
                    ptr = current_set->maximum().ptr;
                } else {
                    ptr = current_set->prev(ptr);
                }

                return *this;
            }

            c_iterator operator++(int) {
                c_iterator result(*this);
                operator++();
                return result;
            }

            c_iterator operator--(int) {
                c_iterator result(*this);
                operator--();
                return result;
            }

            T const & operator*() const {
                return ptr->val;
            }

            T const * operator->() const {
                return ptr->val;
            }

            c_iterator(c_iterator const& rhs) {
                current_set = rhs.current_set;
                ptr = rhs.ptr;
            }

            c_iterator & operator=(c_iterator const& rhs) {
                current_set = rhs.current_set;
                ptr = rhs.ptr;
                return *this;
            }

        private:
            node_ptr ptr;
            c_iterator(node_ptr const ptr, set<T> const * cur_st) : current_set(cur_st), ptr(ptr) {}

            friend class set<T>;

            friend bool operator==(c_iterator const& a, c_iterator const& b) {
                return a.current_set == b.current_set && a.ptr == b.ptr;
            }

            friend bool operator!=(c_iterator const& a, c_iterator const& b) {
                return a.current_set != b.current_set || a.ptr != b.ptr;
            }
        };

    public:
        typedef c_iterator const_iterator;
        typedef const_iterator iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    private:
        node_ptr deep_copy(node_ptr ptr, node_ptr const up_ptr, bool is_left) {
            if (ptr == nullptr) { return nullptr; }

            node_ptr result_root = new tree_node(ptr->val, up_ptr, is_left, ptr->prior);
            result_root->left.reset(deep_copy(ptr->left.get(), result_root, true));
            result_root->right.reset(deep_copy(ptr->right.get(), result_root, false));

            return result_root;
        }

        node_ptr merge(node_ptr lhs, node_ptr rhs) {
            if (lhs == nullptr) { return rhs; }
            if (rhs == nullptr) { return lhs; }

            if (lhs->prior > rhs->prior) {
                node_ptr r_kek = lhs->right.release();
                lhs->right.reset(merge(r_kek, rhs));

                lhs->right->upper = lhs;
                lhs->right->is_left = false;

                return lhs;
            } else {
                node_ptr l_kek = rhs->left.release();
                rhs->left.reset(merge(lhs, l_kek));

                rhs->left->upper = rhs;
                rhs->left->is_left = true;

                return rhs;
            }
        }

        std::pair<node_ptr, node_ptr> split(node_ptr root, T const& key) {
            if (root == nullptr) { return std::make_pair<node_ptr, node_ptr>(nullptr, nullptr); }

            if (t_leq(root->val, key)) {
                node_ptr rhs = root->right.release();

                std::pair<node_ptr, node_ptr> spl1;
                try {
                    spl1 = split(rhs, key);
                } catch (...) {
                    root->right.reset(rhs);
                    throw;
                }

                root->right.reset(spl1.first);

                if (root->right != nullptr) {
                    root->right->upper = root;
                    root->right->is_left = false;
                }

                return std::pair<node_ptr, node_ptr >(root, spl1.second);
            } else {
                node_ptr lhs = root->left.release();
                std::pair<node_ptr, node_ptr> spl1;
                try {
                    spl1 = split(lhs, key);
                } catch (...) {
                    root->left.reset(lhs);
                }
                root->left.reset(spl1.second);

                if (root->left != nullptr) {
                    root->left->upper = root;
                    root->left->is_left = true;
                }

                return std::pair<node_ptr, node_ptr >(spl1.first, root);
            }
        }

        unique_ptr<tree_node> root;
        size_t sz;
        std::less<T> t_less;

        bool t_greater(T const& lhs, T const& rhs) const {
            return rhs < lhs;
        }

        bool t_equal(T const& lhs, T const& rhs) const {
            return !(lhs < rhs || rhs < lhs);
        }

        bool t_leq(T const& lhs, T const& rhs) const {
            return !(rhs < lhs);
        }

        node_ptr next(node_ptr ptr) const {
            if (ptr == nullptr) {
                return nullptr;
            }

            if (ptr->right != nullptr) {
                return minimum(ptr->right.get()).ptr;
            }

            node_ptr y = ptr->upper;
            while (y != nullptr && ptr == y->right.get()) {
                ptr = y;
                y = y->upper;
            }

            return const_cast<node_ptr >(y);
        }

        node_ptr prev(node_ptr ptr) const {
            if (ptr == nullptr) {
                return nullptr;
            }

            if (ptr->left != nullptr) {
                return maximum(ptr->left.get()).ptr;
            }

            node_ptr y = ptr->upper;
            while (y != nullptr && ptr == y->left.get()) {
                ptr = y;
                y = y->upper;
            }

            return const_cast<node_ptr >(y);
        }

        const_iterator minimum(node_ptr ptr) const {
            const_iterator result{ptr, this};

            while (ptr != nullptr) {
                result = {ptr, this};
                ptr = ptr->left.get();
            }

            return result;
        }

        iterator minimum(node_ptr ptr) {
            iterator result{ptr, this};

            while (ptr != nullptr) {
                result = {ptr, this};
                ptr = ptr->left.get();
            }

            return result;
        }

        const_iterator maximum(node_ptr ptr) const {
            const_iterator result{ptr, this};

            while (ptr != nullptr) {
                result = {ptr, this};
                ptr = ptr->right.get();
            }

            return result;
        }

        iterator maximum(node_ptr ptr) {
            iterator result{ptr, this};

            while (ptr != nullptr) {
                result = {ptr, this};
                ptr = ptr->right.get();
            }

            return result;
        }

        bool leaf(node_ptr const ptr) const {
            return ptr->left == nullptr && ptr->right == nullptr;
        }

        void validate_tree(node_ptr ptr, node_ptr upper, bool is_left) {
            if (ptr == nullptr) { return; }

            assert(ptr->upper == upper && ptr->is_left == is_left);

            validate_tree(ptr->left.get(), ptr, true);
            validate_tree(ptr->right.get(), ptr, false);
        }

        void validate_tree() {
//            int mx;
//            int mn;
            validate_tree(root.get(), nullptr, false);
        }

    public:

        const_iterator minimum() const {
            return minimum(root.get());
        }

        const_iterator maximum() const {
            return maximum(root.get());
        }

        iterator minimum() {
            return minimum(root.get());
        }

        iterator maximum() {
            return maximum(root.get());
        }

        iterator begin() {
            return minimum();
        }

        const_iterator begin() const {
            return minimum();
        }

        iterator end() {
            return {nullptr, this};
        }

        const_iterator end() const {
            return {nullptr, this};
        }

        const_iterator cbegin() const {
            return minimum();
        }

        const_iterator cbegin() {
            return minimum();
        }

        const_iterator cend() const {
            return {nullptr, this};
        }

        const_iterator cend() {
            return {nullptr, this};
        }

        reverse_iterator rbegin() {
            return reverse_iterator(end());
        }

        const_reverse_iterator rbegin() const {
            return reverse_iterator(end());
        }

        reverse_iterator rend() {
            return reverse_iterator(begin());
        }

        const_reverse_iterator rend() const {
            return reverse_iterator(begin());
        }

        const_reverse_iterator crbegin() const {
            return const_reverse_iterator(cend());
        }

        const_reverse_iterator crend() const {
            return const_reverse_iterator(cbegin());
        }

        set() noexcept : sz(0) {};
        set(set<T> const& rhs) : root(deep_copy(rhs.root.get(), nullptr, false)), sz(rhs.sz) {}
        set(set<T>&& rhs) noexcept : root(std::move(rhs.root)), sz(rhs.sz) {};

        set<T>& operator=(set<T> const& rhs) {
            set<T> tmp(rhs);

            swap(*this, tmp);

            return *this;
        }

        set<T>& operator=(set<T>&& rhs) noexcept {
            root = std::move(rhs.root);
            sz = rhs.sz;

            rhs.sz = 0;
            return *this;
        }

        std::pair<iterator, bool> insert(T const& val) {
            validate_tree();

            const_iterator find_test = find(val);
            const_iterator end_iter = cend();
            if (find_test != end_iter) {
                return {iterator(nullptr, this), false};
            }

            node_ptr new_val = nullptr;
            try {
                new_val = new tree_node(T(val), nullptr, false);
            } catch (...) {
                validate_tree();
                return {iterator(nullptr, this), false};
            }

            node_ptr v = root.release();

            std::pair<iterator, bool> result = {iterator{new_val, this}, true};

            std::pair<node_ptr, node_ptr> spl;
            try {
                spl = split(v, val);
            } catch (...) {
                root.reset(v);

                validate_tree();
                return {iterator(nullptr, this), false};
            }

            root.reset(merge(merge(spl.first, new_val), spl.second));
            sz++;
            validate_tree();
            return result;
        }

        iterator erase(const_iterator x) {
            validate_tree();
            iterator result = x;
            ++result;

            if (x.ptr == nullptr) { return result; };

            if (x.ptr == root.get()) {
                unique_ptr<tree_node> v = std::move(root);
                node_ptr lhs = v.get()->left.release();
                node_ptr rhs = v.get()->right.release();

                if (lhs != nullptr) {
                    lhs->upper = nullptr;
                    lhs->is_left = false;
                }

                if (rhs != nullptr) {
                    rhs->upper = nullptr;
                    rhs->is_left = false;
                }

                root.reset(merge(lhs, rhs));

                --sz;
                validate_tree();
                return result;
            }

            std::pair<node_ptr, node_ptr> spl;

            node_ptr anc = const_cast<node_ptr >(x.ptr->upper);
            node_ptr lhs = const_cast<node_ptr >(x.ptr)->left.release();
            node_ptr rhs = const_cast<node_ptr >(x.ptr)->right.release();

            const bool is_left = x.ptr->is_left;
            if (lhs != nullptr) {
                lhs->upper = nullptr;
                lhs->is_left = false;
            }

            if (rhs != nullptr) {
                rhs->upper = nullptr;
                rhs->is_left = false;
            }

            if (is_left) {
                anc->left.reset(merge(lhs, rhs));
                if (anc->left != nullptr) {
                    anc->left->upper = anc;
                    anc->left->is_left = true;
                }
            } else {
                anc->right.reset(merge(lhs, rhs));
                if (anc->right != nullptr) {
                    anc->right->upper = anc;
                    anc->right->is_left = false;
                }
            }

            --sz;
            validate_tree();
            return result;
        }

        const_iterator find(T const& val) const {
            node_ptr ptr = root.get();

            if (ptr == nullptr) {
                return cend();
            }

            while (true) {
                if (ptr == nullptr) {
                    return cend();
                }

                if (t_equal(val, ptr->val)) {
                    return const_iterator{ptr, this};
                }

                if (t_less(val, ptr->val)) {
                    ptr = ptr->left.get();
                } else {
                    ptr = ptr->right.get();
                }
            }
        }

        const_iterator lower_bound(T const& val) const {
            node_ptr ptr = root.get();

            if (ptr == nullptr) {
                return cend();
            }

            const_iterator result = cend();

            while (ptr != nullptr) {
                if (t_leq(val, ptr->val)) {
                    result = const_iterator(const_cast<node_ptr>(ptr), this);
                    ptr = ptr->left.get();
                } else {
                    ptr = ptr->right.get();
                }
            }

            return result;
        }

        const_iterator upper_bound(T const& val) const {
            const_iterator result = lower_bound(val);

            if (result.ptr != nullptr && t_equal(result.ptr->val, val)) {
                return ++result;
            }

            return result;
        }

        bool empty() const {
            return (sz == 0);
        }

        void clear() {
            root.reset(nullptr);
            sz = 0;
        }

        ~set() noexcept = default;

        friend void swap(set<T> & x, set<T> & y) {
            set<T> tmp = std::move(x);
            x = std::move(y);
            y = std::move(tmp);
        }
    };
}

#endif //CPP_EXAM_SET_IMPL_H
