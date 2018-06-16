#include <cstdio>
#include <memory>
#include <cassert>

#include "../huffman_lib/include/encrypter.h"
#include "include/utils.h"

using std::unique_ptr;

void read_and_count(freq_counter & f_counter, char * char_buf, size_t char_buf_sz, FILE * input);

int main(int argc, char** argv) try {
    size_t bytes_written = 0;

    unique_ptr<std::FILE, FILE_deleter> input;
    unique_ptr<std::FILE, FILE_deleter> output;

    char const * input_f_name;
    char const * output_f_name;

    const size_t    char_buf_sz = 4096;
    char            char_buf[char_buf_sz ];
    dynamic_bitset  bit_buffer;

    clock_t start = clock();

    if (argc != 4) {
        puts("encrypter \"IFNAME\" -o \"OFNAME\"");
        return 0;
    }

    input_f_name = argv[1];
    output_f_name = argv[3];

    fprintf(stderr, "Counting frequencies.. ");
    freq_counter f_counter;
    input.reset(except_fopen(input_f_name, "r"));
    read_and_count(f_counter, char_buf, char_buf_sz, input.get());
    input.release();
    fprintf(stderr, "OK hash=%zu\n", hash(f_counter));

    h_tree tr(f_counter);
    encrypter encr(tr);

    input.reset(except_fopen(input_f_name, "r"));
    output.reset(except_fopen(output_f_name, "w"));

    fprintf(stderr, "Writing H-Tree meta.. ");
    except_fwrite(f_counter.cnt, max_symbol + 1, output.get(), bytes_written);
    fprintf(stderr, "OK\n");

    fprintf(stderr, "Encrypting data, writing.. ");
    for (size_t read; (read = fread(char_buf, sizeof(symbol), char_buf_sz / sizeof(symbol), input.get())) > 0; ) {
        encr.encrypt(reinterpret_cast<const symbol *>(char_buf), read, bit_buffer);

        size_t blocks_to_write = bit_buffer.full_blocks();
        dynamic_bitset::BlockT const * data_ptr = bit_buffer.data();

        except_fwrite(data_ptr, blocks_to_write, output.get(), bytes_written);

        bytes_written += blocks_to_write * sizeof(dynamic_bitset::BlockT);
        bit_buffer.truncate();
    }

    auto last_byte_bits_used = static_cast<uint8_t>(bit_buffer.length());
    if (bytes_written > 2048 && last_byte_bits_used == 0) {
        last_byte_bits_used = dynamic_bitset::block_bitlength;
    }

    if (last_byte_bits_used > 0) {
        size_t blocks_to_write = 1;
        dynamic_bitset::BlockT const * data_ptr = bit_buffer.data();

        if (bit_buffer.length() > 0) {
            except_fwrite(data_ptr, blocks_to_write, output.get(), bytes_written);
        }
    }

    except_fwrite(&last_byte_bits_used, 1, output.get(), bytes_written);
    fprintf(stdout, "Done! Bytes written: %zu, in %.5Lfsec\n", bytes_written,
            (clock() - start) / static_cast<long double>(CLOCKS_PER_SEC));

    return 0;
} catch (std::runtime_error const & e) {
    fprintf(stderr, "Following error: %s\n", e.what());
    std::exit(-1);
}

void read_and_count(freq_counter & f_counter,
                    char * const char_buf,
                    const size_t char_buf_sz,
                    FILE * const input) {
    for (size_t read; (read = fread(char_buf, sizeof(symbol), char_buf_sz, input)) > 0; ) {
        f_counter.count(reinterpret_cast<symbol const*>(char_buf), read);
    }
}