//
// Created by starorpheus on 14/06/18.
//
#include <string>
#include <memory>
#include <cassert>
#include <vector>
#include <vector>

#include "include/utils.h"
#include "../huffman_lib/include/utils.h"
#include "../huffman_lib/include/decrypter.h"

using std::size_t;
using std::unique_ptr;
using std::vector;

size_t bytes_written = 0;

unique_ptr<std::FILE, FILE_deleter> input;
unique_ptr<std::FILE, FILE_deleter> output;

char const * input_f_name;
char const * output_f_name;

const size_t char_buf_sz = 1u << 16u;
char char_buf[char_buf_sz];
vector<symbol > decrypted_buf;
size_t freqs[max_symbol + 1];

bool have_head = false;
size_t head_size;

void read_meta();
void write_decrypted();

int main(int argc, char ** argv) {
    static_assert(char_buf_sz > 0, "Not-empty buffer expected!");
    decrypted_buf.reserve(char_buf_sz);
    clock_t start = clock();

    if (argc != 4) {
        puts("decrypter \"IFNAME\" -o \"OFNAME\"");
    }

    input_f_name = argv[1];
    output_f_name = argv[3];

    input.reset(except_fopen(input_f_name, "r"));

    fprintf(stderr, "Reading H-Tree meta.. ");

    read_meta();

    freq_counter f_counter(const_cast<size_t const *>(freqs));
    fprintf(stderr, "OK hash=%zu\n", hash(f_counter));

    h_tree tr(f_counter);
    decrypter decr(tr);

    fprintf(stderr, "Starting decryption.. ");
    dynamic_bitset::BlockT last_block = 0;

    bool has_last = false;
    output.reset(except_fopen(output_f_name, "w"));

    if (have_head) {
        decr.decrypt(char_buf + (char_buf_sz - head_size), head_size, decrypted_buf);

        write_decrypted();
    }

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

            write_decrypted();
        }

        decr.decrypt(read_data, (read - 1) * sizeof(dynamic_bitset::BlockT),
                                                         decrypted_buf);
        last_block = read_data[read - 1];
        has_last = true;

        write_decrypted();
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

        write_decrypted();
    }
    fprintf(stderr, "OK\n");

    fprintf(stderr, "Done! Bytes written: %zu, in %.5Lfsec\n", bytes_written,
            (clock() - start) / static_cast<long double>(CLOCKS_PER_SEC));
    return 0;
}

void write_decrypted()  {
    if (!decrypted_buf.empty()) {
        except_fwrite(decrypted_buf.data(), decrypted_buf.size(), output.get(), bytes_written);
        decrypted_buf.clear();
    }
}

void read_meta() {
    if (char_buf_sz >= 2048) {
        const size_t read = fread(freqs, sizeof(size_t), (max_symbol + 1), input.get());
        if (read != max_symbol + 1) {
            throw std::runtime_error("Error reading H-Tree meta: expected at least "
                                     + std::to_string(sizeof(size_t) * (max_symbol + 1))
                                     + " bytes, but " + std::to_string(read * sizeof(size_t)) + " bytes read.");
        }
    } else {
        size_t iter_cnt = 2048 / char_buf_sz;
        const size_t size_t_per_iter = char_buf_sz / sizeof(size_t);
        size_t byte_read = 0;

        for (size_t iter = 0; iter < iter_cnt; ++iter) {
            const size_t read = fread(freqs + byte_read / sizeof(size_t), sizeof(size_t), size_t_per_iter, input.get());

            if (read != size_t_per_iter) {
                throw std::runtime_error("Error reading H-Tree meta: expected at least "
                                         + std::to_string(sizeof(size_t) * (max_symbol + 1))
                                         + " bytes, but " + std::to_string(read * sizeof(size_t)) + " bytes read.");
            }

            byte_read += size_t_per_iter * sizeof(size_t);
        }

        if (byte_read != 2048) {
            assert(byte_read < 2048);

            const size_t rest = (2048 - byte_read) / sizeof(size_t);
            const size_t read = fread(freqs + byte_read, sizeof(size_t), rest, input.get());

            if (read != rest) {
                throw std::runtime_error("Error reading H-Tree meta: expected at least "
                                         + std::to_string(sizeof(size_t) * (max_symbol + 1))
                                         + " bytes, but " + std::to_string(read * sizeof(size_t)) + " bytes read.");
            }

            byte_read += rest * sizeof(size_t);

            have_head = true;
            head_size = char_buf_sz - rest * sizeof(size_t);
        }
    }
}