#ifndef BIG_INT_STORAGE_H
#define BIG_INT_STORAGE_H

#include <cinttypes>
#include <cstdlib>
#include <memory>
#include <functional>
#include <algorithm>

using std::shared_ptr;
using std::size_t;
using std::nullptr_t;

class big_int_storage;

struct big_int_storage_node
{
//    uint32_t m_links_from_cnt;
    size_t m_size;

    static const size_t SMALL_SIZE = (sizeof(uint32_t) + sizeof(size_t)) / sizeof(uint32_t);

    union
    {
        struct {
            uint32_t * m_link;
            size_t capacity;
        } long_ver;

        uint32_t small_ver[SMALL_SIZE];
    } m_data;

    enum {
        LONG_VERSION_STATE,
        SMALL_VERSION_STATE
    } current_state;

    big_int_storage_node();

    big_int_storage_node* clone() const;

    ~big_int_storage_node();

    void move_to_long();
    void move_to_short();
    void ensure_long_capacity(size_t new_size);
};

uint32_t* const get_data_ptr(big_int_storage_node & cur_storage);

class big_int_storage
{
    shared_ptr<big_int_storage_node> m_ptr;
public:
    big_int_storage();
    big_int_storage(big_int_storage const& rhs) = default;
    big_int_storage(size_t sz, uint32_t value);

    big_int_storage& operator=(big_int_storage const& rhs) = default;

    uint32_t& operator[](size_t pos) = delete;

    uint32_t get_at(size_t pos) const;
    void set_at(size_t pos, uint32_t value);

    void push_back(uint32_t value);
    void pop_back();
    void resize(size_t new_size);

    size_t size() const;
    uint32_t back() const;

    void reset(uint32_t* t_data, size_t sz);
    void clone();

    friend bool operator==(big_int_storage const& lhs, big_int_storage const& rhs);
};

bool operator==(big_int_storage const& lhs, big_int_storage const& rhs);
bool operator!=(big_int_storage const& lhs, big_int_storage const& rhs);

#endif // BIG_INT_STORAGE_H
