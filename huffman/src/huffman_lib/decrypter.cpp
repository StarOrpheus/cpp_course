#include <vector>

#include "include/decrypter.h"

decrypter::decrypter(h_tree const& huff_tree)
        : dfa(huff_tree) {}

void decrypter::naive_decrypt(dynamic_bitset const &input, vector<symbol> &result) {
    dfa.bit_go(input, result);
}

inline size_t byte_pos(size_t const& bit_pos) {
    return (bit_pos >> 3u);
}

inline uint8_t bit_pos(size_t const& bitset_pos) {
    const size_t bytePos = byte_pos(bitset_pos);
    const auto result = static_cast<uint8_t>(bitset_pos - (bytePos << 3u));
    return result;
}

template<typename T>
inline T clear_k_lower_bits(T x, uint8_t k) {
    return (x >> k) << k;
}

inline uint16_t slice2bytes(const uint8_t *const &ptr, size_t const &start_bit) {
    size_t byte_p = byte_pos(start_bit);
    if (clear_k_lower_bits(start_bit, 3) == start_bit) {
        uint16_t result = ptr[byte_p];
        result |= uint16_t(ptr[++byte_p]) << 8u;

        return result;
    }

    uint8_t bit_p = bit_pos(start_bit);
    uint16_t result = ptr[byte_p] >> bit_p;
    result |= uint16_t(ptr[++byte_p]) << (8u - bit_p);
    result |= uint16_t(ptr[++byte_p]) << (8u - bit_p + 8u);

    return result;
}

inline bool ith_bit(const uint8_t * const & ptr, size_t const& i) {
    const size_t byte_p = byte_pos(i);
    const uint8_t bit_p = bit_pos(i);
    const uint8_t current_byte = ptr[byte_p];
    return (current_byte & (1u << bit_p)) != 0;
}

void decrypter::decrypt(const void *ptr, size_t sz, std::vector<symbol> &result) {
    sz <<= 3; // sz in bits

    for (size_t i = 0; i < sz; ) {
        if (dfa.condition() == dfa.root() && i + 16 < sz) {
            const uint16_t slice = slice2bytes((const uint8_t *const &) ptr, i);
            h_tree_dfa::DFA_step const& step = dfa.root_go(slice);

            i += 16 - step.skipped_bits;
            if (step.symbol_achieved) {
                result.push_back(step.c);
            }

            continue;
        }

        // not root || i + 16 < sz

        if (i + 16 <= sz) {
            const uint16_t slice = slice2bytes((const uint8_t *const &) ptr, i);
            h_tree_dfa::DFA_step step = dfa.bit_go(slice);

            i += 16 - step.skipped_bits;
            if (step.symbol_achieved) {
                result.push_back(step.c);
            }

            continue;
        }

        h_tree_dfa::DFA_step step = dfa.bit_go(ith_bit((const uint8_t *const &) ptr, i));
        ++i;
        if (step.symbol_achieved) {
            result.push_back(step.c);
        }
    }
}

decrypter::decrypter(freq_counter const& f_counter)
        : decrypter(h_tree(f_counter)) {}