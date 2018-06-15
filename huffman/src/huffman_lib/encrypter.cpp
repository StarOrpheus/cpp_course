//
// Created by starorpheus on 26/05/18.
//

#include "include/encrypter.h"

#include <iostream>
#include <cassert>

void encrypter::encrypt(symbol const* s_ptr, size_t byte_count, dynamic_bitset & result) {
    for (symbol const* ptr = s_ptr; ptr < s_ptr + byte_count; ++ptr) {
        result += codes[*ptr];
    }
}

encrypter::encrypter(h_tree const& tr)
        : codes(std::move(tr.code_table())) {}

encrypter::encrypter(freq_counter const& f_counter)
        : encrypter(h_tree(f_counter)) {}
