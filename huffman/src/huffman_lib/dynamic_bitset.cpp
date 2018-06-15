//
// Created by starorpheus on 10/06/18.
//

#include <cassert>
#include "include/dynamic_bitset.h"

#include <iostream>

inline size_t byte_pos(size_t bitset_pos) {
    size_t result = (bitset_pos >> dynamic_bitset::block_sz_log);

    return result;
}

inline uint8_t bit_pos(size_t bitset_pos) {
    const size_t bytePos = byte_pos(bitset_pos);
    auto result = static_cast<uint8_t>(bitset_pos - (bytePos << dynamic_bitset::block_sz_log));
    return result;
}

template<typename T>
inline T clear_k_lower_bits(T x, uint8_t k) {
    return (x >> k) << k;
}

template<typename T>
inline T take_k_lower_bits(T val, uint8_t k) {
    return val - clear_k_lower_bits(val, k);
}

inline bool no_empty_tail(size_t bit_cnt) {
    return clear_k_lower_bits(bit_cnt, dynamic_bitset::block_sz_log) == bit_cnt;
}

dynamic_bitset::dynamic_bitset() : m_data(), bit_cnt(0) {}

dynamic_bitset &dynamic_bitset::operator+=(dynamic_bitset const &rhs) {
    if (rhs.empty()) {
        return *this;
    }

    uint8_t shift = take_k_lower_bits(static_cast<uint8_t>(bit_cnt), block_sz_log);

    if (shift == 0) {
        m_data.insert(m_data.end(), rhs.m_data.begin(), rhs.m_data.end());

        bit_cnt += rhs.bit_cnt;

        return *this;
    }

    m_data.back() = take_k_lower_bits(m_data.back(), shift);
    m_data.back() |= (rhs.m_data.front() << shift);

    for (size_t i = 0; i < rhs.m_data.size() - 1; ++i) {
        BlockT new_block = rhs.m_data[i] >> (block_bitlength - shift);
        new_block |= rhs.m_data[i + 1] << shift;

        m_data.push_back(new_block);
    }

    uint8_t rhs_bits_in_last_byte = take_k_lower_bits(static_cast<uint8_t>(rhs.bit_cnt), block_sz_log);
    if (rhs_bits_in_last_byte == 0) {
        rhs_bits_in_last_byte = block_bitlength;
    }

    if (rhs_bits_in_last_byte > (block_bitlength - shift)) {
        m_data.push_back(rhs.m_data.back() >> (block_bitlength - shift));
    }

    bit_cnt += rhs.bit_cnt;

    return *this;
}

void dynamic_bitset::push_back(bool f) {
    if (no_empty_tail(bit_cnt)) {
        m_data.push_back(f ? 1 : 0);
    } else {
        size_t pos = byte_pos(bit_cnt);
        uint8_t b_pos = bit_pos(bit_cnt);

        if (f) {
            m_data[pos] |= (BlockT(1) << b_pos);
        } else {
            m_data[pos] ^= m_data[pos] & (BlockT(1) << b_pos);
        }
    }

    bit_cnt++;
}

void dynamic_bitset::pop_back() {
    bit_cnt--;

    if (no_empty_tail(bit_cnt)) {
        m_data.pop_back();
    }
}

bool dynamic_bitset::operator[](size_t pos) const {
    size_t p1 = byte_pos(pos);
    uint8_t b_pos = bit_pos(pos);

    return (m_data[p1] & (BlockT(1) << b_pos)) != 0;
}

void dynamic_bitset::truncate() {
    if (no_empty_tail(bit_cnt)) {
        m_data.clear();
    } else {
        m_data[0] = m_data.back();
        m_data.resize(1);
    }

//    bit_cnt %= 32;
    bit_cnt = take_k_lower_bits(bit_cnt, block_sz_log);
}

std::string to_string(dynamic_bitset const& st) {
    std::string result;

    for (size_t i = 0; i < st.length(); i++) {
        result += std::to_string(st[i] ? 0u : 1u);
    }

    return std::move(result);
}

const dynamic_bitset::BlockT * dynamic_bitset::data() const {
    return m_data.data();
}

size_t dynamic_bitset::full_blocks() const {
    return bit_cnt >> block_sz_log;
}

bool dynamic_bitset::empty() const {
    return bit_cnt == 0;
}

dynamic_bitset::dynamic_bitset(dynamic_bitset::BlockT x, size_t bit_count) : m_data(1, x), bit_cnt(bit_count) {}
