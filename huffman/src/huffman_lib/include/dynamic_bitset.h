//
// Created by starorpheus on 10/06/18.
//

#ifndef HUFFMAN_DYNAMIC_BITSET_H
#define HUFFMAN_DYNAMIC_BITSET_H

#include <cstdint>
#include <vector>
#include <cstring>
#include <string>
#include <memory>
#include <limits>

class dynamic_bitset {
public:
    typedef uint64_t BlockT;
    static const uint8_t block_bitlength = sizeof(BlockT) * 8u;
    static const uint8_t block_sz_log = 6u;
private:
    std::vector<BlockT > m_data;
    size_t bit_cnt;

public:
    dynamic_bitset();
    dynamic_bitset(BlockT x, size_t bit_count);
    dynamic_bitset(dynamic_bitset const& rhs) = default;
    dynamic_bitset(dynamic_bitset&& rhs) noexcept = default;

    dynamic_bitset & operator=(dynamic_bitset const& rhs) = default;
    dynamic_bitset & operator=(dynamic_bitset && rhs) = default;

    dynamic_bitset & operator+=(dynamic_bitset const& rhs);
    void push_back(bool f);
    void pop_back();

    bool operator[](size_t pos) const;

    // leaves last unfilled byte
    void truncate();

    size_t length() const {
        return bit_cnt;
    }

    size_t full_blocks() const;

    bool empty() const;

    BlockT const * data() const;

    friend std::string to_string(dynamic_bitset const&);
};



#endif //HUFFMAN_DYNAMIC_BITSET_H
