//
// Created by starorpheus on 14/06/18.
//
#include <string>
#include <memory>
#include <cassert>
#include <vector>
#include <vector>

#include "include/utils.h"
#include "../huffman_lib/include/decrypter.h"

using std::size_t;
using std::unique_ptr;
using std::vector;

void write_decrypted(vector<symbol> & decrypted_buf, FILE * output, size_t & bytes_written);
void read_meta(size_t char_buf_sz, size_t * freqs, FILE * input);

int main(int argc, char ** argv) try {
    size_t bytes_written = 0;

    unique_ptr<std::FILE, FILE_deleter> input;
    unique_ptr<std::FILE, FILE_deleter> output;

    char const *        input_f_name;
    char const *        output_f_name;

    const size_t        char_buf_sz = 1u << 16u;
    char                char_buf[char_buf_sz];
    vector<symbol >     decrypted_buf;
    size_t              freqs[max_symbol + 1];

    static_assert(char_buf_sz > 0, "Not-empty buffer expected!");
    decrypted_buf.reserve(char_buf_sz);
    clock_t start = clock();

    if (argc != 4) {
        puts("decrypter \"IFNAME\" -o \"OFNAME\"");
        return 0;
    }

    input_f_name = argv[1];
    output_f_name = argv[3];

    input.reset(except_fopen(input_f_name, "r"));

    fprintf(stderr, "Reading H-Tree meta.. ");

    read_meta(char_buf_sz, freqs, input.get());

    freq_counter f_counter(const_cast<size_t const *>(freqs));
    fprintf(stderr, "OK hash=%zu\n", hash(f_counter));

    h_tree tr(f_counter);
    decrypter decr(tr);

    fprintf(stderr, "Starting decryption.. ");
    dynamic_bitset::BlockT last_block = 0;

    bool has_last = false;
    output.reset(except_fopen(output_f_name, "w"));

    static_assert(char_buf_sz % sizeof(dynamic_bitset::BlockT) == 0,
                  "Char buf must be divisible by sizeof(dynamic_bitset::BlockT)");
    static_assert(sizeof(dynamic_bitset::BlockT) > 1,
                  "dynamic_bitset::BlockT should be bigger that uint8_t");

    for (size_t read; (read = (fread(char_buf, sizeof(dynamic_bitset::BlockT),
                          char_buf_sz / sizeof(dynamic_bitset::BlockT), input.get()))) > 0;) {

        dynamic_bitset::BlockT const * const read_data
                = const_cast<dynamic_bitset::BlockT const *>(reinterpret_cast<dynamic_bitset::BlockT *>(char_buf));

        if (has_last) {
            decr.decrypt(&last_block, sizeof(last_block), decrypted_buf);

            write_decrypted(decrypted_buf, output.get(), bytes_written);
        }

        decr.decrypt(read_data, (read - 1) * sizeof(dynamic_bitset::BlockT),
                                                         decrypted_buf);
        last_block = read_data[read - 1];
        has_last = true;

        write_decrypted(decrypted_buf, output.get(), bytes_written);
    }

    fseek(input.get(), -1, SEEK_CUR);
    if (has_last) {
        const size_t read = fread(char_buf, 1, 1, input.get());
        if (read != 1) {
            throw std::runtime_error("Tail meta not found, last bytes could be corrupted; exiting.");
        }

        decr.naive_decrypt(dynamic_bitset(last_block,
                                          static_cast<size_t>(char_buf[0])),
                           decrypted_buf);

        write_decrypted(decrypted_buf, output.get(), bytes_written);
    }

    fprintf(stderr, "OK\n");

    fprintf(stderr, "Done! Bytes written: %zu, in %.5Lfsec\n", bytes_written,
            (clock() - start) / static_cast<long double>(CLOCKS_PER_SEC));

    return 0;
} catch (const std::runtime_error& e) {
    fprintf(stderr, "Following error: %s\n", e.what());
    std::exit(-1);
}

void write_decrypted(vector<symbol> & decrypted_buf, FILE * const output, size_t & bytes_written)  {
    if (!decrypted_buf.empty()) {
        except_fwrite(decrypted_buf.data(), decrypted_buf.size(), output, bytes_written);
        decrypted_buf.clear();
    }
}

void read_meta(const size_t char_buf_sz, size_t * const freqs, FILE * input) {
    if (char_buf_sz >= 2048) {
        const size_t read = fread(freqs, sizeof(size_t), (max_symbol + 1), input);
        if (read != max_symbol + 1) {
            throw std::runtime_error("Error reading H-Tree meta: expected at least "
                                     + std::to_string(sizeof(size_t) * (max_symbol + 1))
                                     + " bytes, but " + std::to_string(read * sizeof(size_t)) + " bytes read.");
        }
    }
}