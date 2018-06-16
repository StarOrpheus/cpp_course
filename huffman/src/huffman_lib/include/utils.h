#ifndef HUFFMAN_UTILS_H
#define HUFFMAN_UTILS_H

#include <cstdint>
#include <limits>

typedef uint8_t symbol; // UB with signed.

const uint8_t max_symbol
        = std::numeric_limits<symbol>::max();

#endif //HUFFMAN_UTILS_H
