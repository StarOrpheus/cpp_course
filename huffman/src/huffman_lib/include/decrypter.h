//
// Created by starorpheus on 26/05/18.
//

#ifndef HUFFMAN_DECRYPTER_H
#define HUFFMAN_DECRYPTER_H

#include <vector>

#include "utils.h"

class decrypter {
    h_tree_DFA dfa;

public:
    explicit
    decrypter(h_tree const& huff_tree);

    explicit
    decrypter(freq_counter const& f_counter);

    void naive_decrypt(dynamic_bitset const &input, std::vector<symbol> &result);
    void decrypt(const void *ptr, size_t sz, std::vector<symbol> & result);
};


#endif //HUFFMAN_DECRYPTER_H
