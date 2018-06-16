//
// Created by starorpheus on 16/06/18.
//

#ifndef HUFFMAN_FREQUENCY_COUNTER_H
#define HUFFMAN_FREQUENCY_COUNTER_H

#include <cstdlib>

#include "utils.h"

using std::size_t;

struct freq_counter {
    size_t cnt[max_symbol + 1];

    freq_counter();

    explicit freq_counter(size_t const* t_cnt);

    void count(symbol const * ptr, size_t sz);
};

size_t hash(freq_counter const & f_counter);

#endif //HUFFMAN_FREQUENCY_COUNTER_H
