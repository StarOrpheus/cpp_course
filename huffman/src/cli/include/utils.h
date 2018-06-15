//
// Created by starorpheus on 14/06/18.
//

#ifndef HUFFMAN_CLI_UTILS_H
#define HUFFMAN_CLI_UTILS_H

#include <cstdio>
#include <stdexcept>

struct FILE_deleter {
    void operator()(FILE * const ptr) {
        fclose(ptr);
    }
};

std::FILE * except_fopen(const char * filename, const char * mode);

template<typename T>
void except_fwrite(const T * buffer, size_t count, std::FILE *stream, size_t& bytes_written) {
    size_t cur_written = fwrite(buffer, sizeof(T), count, stream);
    if (cur_written != count) {
        throw std::runtime_error("Error writing, "
                                 + std::to_string(bytes_written + cur_written)
                                 + " bytes written. Exiting.\n");
    }

    bytes_written += cur_written * sizeof(T);
}

#endif //HUFFMAN_UTILS_H
