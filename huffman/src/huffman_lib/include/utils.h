//
// Created by starorpheus on 27/05/18.
//

#ifndef HUFFMAN_UTILS_H
#define HUFFMAN_UTILS_H

#include <cstdint>
#include <vector>
#include <cstring>
#include <string>
#include <memory>
#include <limits>

using std::size_t ;

typedef uint8_t symbol; // UB with signed.

const uint8_t max_symbol
        = std::numeric_limits<symbol>::max();

struct freq_counter {
    size_t cnt[max_symbol + 1];

    freq_counter();

    freq_counter(size_t const* t_cnt);

    void count(symbol const * ptr, size_t sz);
};

size_t hash(freq_counter const & f_counter);

#include "dynamic_bitset.h"
#include "h_tree.h"
#include "h_tree_DFA.h"

#endif //HUFFMAN_UTILS_H
