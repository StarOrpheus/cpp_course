//
// Created by starorpheus on 26/05/18.
//

#ifndef HUFFMAN_ENCRYPTER_H
#define HUFFMAN_ENCRYPTER_H

#include "utils.h"

class encrypter {
    vector<dynamic_bitset> codes;

public:
    explicit
    encrypter(h_tree const& tr);

    explicit
    encrypter(freq_counter const& f_counter);

    void encrypt(symbol const* ptr,
                 size_t byte_count,
                 dynamic_bitset & result);
};


#endif //HUFFMAN_ENCRYPTER_H
