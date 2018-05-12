#include "big_int_storage.h"

#include <cstring>
#include <cassert>
#include <string>
#include <cmath>

big_int_storage::big_int_storage() {
    m_ptr.reset(new big_int_storage_node());
    m_ptr->m_size = 0;
    m_ptr->current_state = big_int_storage_node::SMALL_VERSION_STATE;
}

big_int_storage::big_int_storage(size_t const sz, uint32_t const value) : big_int_storage() {
    resize(sz);
    for (size_t i = 0; i < sz; i++) {
        this->set_at(i, value);
    }
}

big_int_storage_node::big_int_storage_node()  : m_size(0), current_state(big_int_storage_node::SMALL_VERSION_STATE) {}

big_int_storage_node* big_int_storage_node::clone() const {
    big_int_storage_node* tmp = new big_int_storage_node();
    tmp->m_size = m_size;
    tmp->current_state = current_state;

    if (m_size <= big_int_storage_node::SMALL_SIZE) {
        memcpy(&tmp->m_data.small_ver,
               &m_data.small_ver,
               big_int_storage_node::SMALL_SIZE * sizeof(uint32_t));
    } else {
        tmp->m_data.long_ver.m_link = new uint32_t[m_size];
        tmp->m_data.long_ver.capacity = m_size;

        memcpy(tmp->m_data.long_ver.m_link, m_data.long_ver.m_link, m_size * sizeof(uint32_t));
    }

    return tmp;
}

big_int_storage_node::~big_int_storage_node() {
    if (current_state == LONG_VERSION_STATE) {
        delete[] m_data.long_ver.m_link;
    }
}

inline uint32_t* const get_data_ptr(big_int_storage_node & cur_storage) {
    return (cur_storage.current_state == big_int_storage_node::LONG_VERSION_STATE ?
            cur_storage.m_data.long_ver.m_link : cur_storage.m_data.small_ver);
}

uint32_t big_int_storage::get_at(size_t pos) const {
    big_int_storage_node& current_storage = *m_ptr;

    if (pos < 0 || pos >= current_storage.m_size) {
        throw std::out_of_range(std::to_string(pos) + " is out of range[0; "
                                + std::to_string(current_storage.m_size) + "]");
    }

    return get_data_ptr(current_storage)[pos];
}

void big_int_storage::set_at(size_t pos, uint32_t value) {
    if (pos < 0 || pos >= m_ptr->m_size) {
        throw std::out_of_range(std::to_string(pos) + " is out of range[0; "
                                + std::to_string(m_ptr->m_size) + "]");
    }

    clone();

    big_int_storage_node& current_storage = *m_ptr;
    get_data_ptr(current_storage)[pos] = value;
}

void big_int_storage_node::move_to_long() {
    assert(current_state == big_int_storage_node::SMALL_VERSION_STATE);

    uint32_t * new_array = new uint32_t[big_int_storage_node::SMALL_SIZE + 4];
    memcpy(new_array, m_data.small_ver, m_size * sizeof(uint32_t));

    current_state = big_int_storage_node::LONG_VERSION_STATE;

    m_data.long_ver.m_link = new_array;
    m_data.long_ver.capacity = big_int_storage_node::SMALL_SIZE + 1;
}

void big_int_storage_node::move_to_short() {
    assert(current_state == big_int_storage_node::LONG_VERSION_STATE);
    assert(m_size <= big_int_storage_node::SMALL_SIZE);

    uint32_t * tmp_array = new uint32_t[m_size];
    memcpy(tmp_array, m_data.long_ver.m_link, m_size * sizeof(uint32_t));

    delete[] m_data.long_ver.m_link;
    memcpy(m_data.small_ver, tmp_array, m_size * sizeof(uint32_t));

    current_state = big_int_storage_node::SMALL_VERSION_STATE;

    delete[] tmp_array;
}

void big_int_storage_node::ensure_long_capacity(size_t new_size) {
    assert(current_state == big_int_storage_node::LONG_VERSION_STATE);

    if (new_size > m_data.long_ver.capacity) {
        size_t new_cap = m_data.long_ver.capacity;

        while (new_cap < new_size) {
            new_cap = ceil(new_cap * 1.7l);
        }

        uint32_t* new_array = new uint32_t[new_cap];
        memcpy(new_array, m_data.long_ver.m_link, m_data.long_ver.capacity
                                                  * sizeof(uint32_t));
        delete[] m_data.long_ver.m_link;
        m_data.long_ver.m_link = new_array;
        m_data.long_ver.capacity = new_cap;
    }

    if ((m_data.long_ver.capacity >> 2) > new_size) {
        size_t new_cap = m_data.long_ver.capacity;

        while ((new_cap >> 2) >= new_size) {
            new_cap = floor(new_cap / 1.5l);
        }

        new_cap = ceil(new_cap * 1.5l);

        uint32_t* new_array = new uint32_t[new_cap];
        memcpy(new_array, m_data.long_ver.m_link, std::min(m_data.long_ver.capacity, new_cap)
                                                  * sizeof(uint32_t));
        delete[] m_data.long_ver.m_link;
        m_data.long_ver.m_link = new_array;
        m_data.long_ver.capacity = new_cap;
    }
}

void big_int_storage::push_back(uint32_t value) {
    if (m_ptr.use_count() != 1) {
        m_ptr.reset(m_ptr->clone());
    }

    big_int_storage_node& current_storage = *m_ptr;
    size_t cur_size = current_storage.m_size;

    if (cur_size + 1 <= big_int_storage_node::SMALL_SIZE) {
        current_storage.m_data.small_ver[current_storage.m_size++] = value;
    } else if (cur_size == big_int_storage_node::SMALL_SIZE) {
        current_storage.move_to_long();
        current_storage.ensure_long_capacity(cur_size + 1);

        current_storage.m_data.long_ver.m_link[current_storage.m_size++] = value;
    } else {
        current_storage.ensure_long_capacity(cur_size + 1);

        current_storage.m_data.long_ver.m_link[current_storage.m_size++] = value;
    }
}

void big_int_storage::pop_back() {
    resize(size() - 1);
}

void big_int_storage::clone() {
    if (m_ptr.use_count() != 1) {
        m_ptr.reset(m_ptr->clone());
    }
}

void big_int_storage::resize(size_t new_size) {
    clone();

    big_int_storage_node& current_storage = *m_ptr;

    if (current_storage.current_state == current_storage.big_int_storage_node::SMALL_VERSION_STATE) {
        if (new_size <= big_int_storage_node::SMALL_SIZE) {
            current_storage.m_size = new_size;
        } else {
            current_storage.move_to_long();
            current_storage.ensure_long_capacity(new_size);
            current_storage.m_size = new_size;
        }
    } else {
        if (new_size <= big_int_storage_node::SMALL_SIZE) {
            current_storage.m_size = new_size;
            current_storage.move_to_short();
        } else {
            current_storage.ensure_long_capacity(new_size);
            current_storage.m_size = new_size;
        }
    }
}

size_t big_int_storage::size() const {
    return m_ptr->m_size;
}

uint32_t big_int_storage::back() const {
    return get_data_ptr(*m_ptr)[size() - 1];
}

void big_int_storage::reset(uint32_t* t_data, size_t sz) {
    clone();

    if (sz <= big_int_storage_node::SMALL_SIZE
        && m_ptr->current_state == big_int_storage_node::SMALL_VERSION_STATE) {
        memcpy(m_ptr->m_data.small_ver, t_data, sz * sizeof(uint32_t));

        delete[] t_data;
    } else if (sz <= big_int_storage_node::SMALL_SIZE
               && m_ptr->current_state == big_int_storage_node::LONG_VERSION_STATE) {
        delete[] m_ptr->m_data.long_ver.m_link;
        memcpy(m_ptr->m_data.small_ver, t_data, sz * sizeof(uint32_t));
        m_ptr->current_state = big_int_storage_node::SMALL_VERSION_STATE;
        delete[] t_data;
    } else if (sz > big_int_storage_node::SMALL_SIZE
               && m_ptr->current_state == big_int_storage_node::SMALL_VERSION_STATE) {
        m_ptr->current_state = big_int_storage_node::LONG_VERSION_STATE;
        m_ptr->m_data.long_ver.capacity = sz;
        m_ptr->m_data.long_ver.m_link = t_data;
    } else if (sz > big_int_storage_node::SMALL_SIZE
               && m_ptr->current_state == big_int_storage_node::LONG_VERSION_STATE) {
        delete[] m_ptr->m_data.long_ver.m_link;
        m_ptr->m_data.long_ver.capacity = sz;
        m_ptr->m_data.long_ver.m_link = t_data;
    }

    m_ptr->m_size = sz;
}

bool operator==(big_int_storage const& lhs, big_int_storage const& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }

    int res = memcmp(get_data_ptr(*lhs.m_ptr),
                     get_data_ptr(*rhs.m_ptr),
                     lhs.size() * sizeof(uint32_t));

    return res == 0;
}

bool operator!=(big_int_storage const& lhs, big_int_storage const& rhs) {
    return !(lhs == rhs);
}
