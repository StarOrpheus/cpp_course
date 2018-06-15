#include <string>
#include <memory>
#include <cstdio>
#include <type_traits>

#include "../../../libs/gtest/gtest.h"
#include "../../cli/include/utils.h"

using std::unique_ptr;
using std::string;

string make_file(string const& data) {
    string filename = "/tmp/test" + std::to_string(rand()) + std::to_string(rand());

    unique_ptr<std::FILE, FILE_deleter> file(except_fopen(filename.data(), "w"));

    if (size_t tmp = fwrite(data.data(), 1, data.size(), file.get()); tmp != data.size()) {
        int error = ferror(file.get());
        perror("The folowwing error occured: ");
        throw std::runtime_error("Write error.");
    }

    return filename;
}

int del_file(string filename) {
    return remove(filename.data());
}

struct auto_delete {
    string filename;
    auto_delete(string const& filename) : filename(filename) {};

    ~auto_delete() {
        del_file(filename);
    }
};

void eq_files(string const& fname1, string const& fname2) {
    char buf1[256];
    char buf2[256];
    size_t iter = 0;

    unique_ptr<std::FILE, FILE_deleter> file1(except_fopen(fname1.data(), "r"));
    unique_ptr<std::FILE, FILE_deleter> file2(except_fopen(fname2.data(), "r"));

    for (size_t read1, read2; (read1 = fread(buf1, 1, 256, file1.get())) > 0
                              && (read2 = fread(buf2, 1, 256, file2.get())); ) {
        for (size_t i = 0; i < std::min(read1, read2); ++i) {
            if (buf1[i] != buf2[i]) {
                throw std::runtime_error("Files differ on byte: " + std::to_string(iter));
            }

            ++iter;
        }

        if (read1 != read2) {
            throw std::runtime_error(read1 < read2 ? "First file is prefix of second file"
                                                     : "Second file is prefix of first file");
        }
    }

    fputs(("Okay, files " + fname1 + " " + fname2 + " are equal.\n").data(), stderr);
}

#define __STR_IMPL__(S) #S
#define __STR__(S) __STR_IMPL__(S)

const string encrypter_exec(__STR__(__ENCRYPTER_EXEC));
const string decrypter_exec(__STR__(__DECRYPTER_EXEC));

TEST(cli_common, cyphers) {
    string input = make_file("0123456789");
    string huf_name = input + ".huf";
    string decr_name = input + ".decr";
    auto_delete ac(input);

    system((encrypter_exec + " "
            + input + " -o " + huf_name).data());
    system((decrypter_exec + " "
            + huf_name + " -o " + decr_name).data());

    eq_files(input, decr_name);

    del_file(input);
    del_file(huf_name);
    del_file(decr_name);
}

TEST(cli_common, ascii_table) {
    string data;
    for (uint32_t i = 0; i < std::numeric_limits<uint8_t >::max(); ++i) {
        data.push_back(char(i));
    }

    string input = make_file(data);
    string huf_name = input + ".huf";
    string decr_name = input + ".decr";

    system((encrypter_exec + " "
            + input + " -o " + huf_name).data());
    system((decrypter_exec + " "
            + huf_name + " -o " + decr_name).data());

    eq_files(input, decr_name);

    del_file(input);
    del_file(huf_name);
    del_file(decr_name);
}

TEST(cli_common, lengths_randomized) {
    string data;
    data.reserve(10000);

    for (size_t len = 0; len < 1000; len += 10) {
        for (uint32_t i = 0; i < len; ++i) {
            data.push_back(static_cast<char>((rand() % 256)));
        }

        string input = make_file(data);
        string huf_name = input + ".huf";
        string decr_name = input + ".decr";

        system((encrypter_exec + " " + input + " -o " + huf_name).data());
        system((decrypter_exec + " " + huf_name + " -o " + decr_name).data());

        eq_files(input, decr_name);

        del_file(input);
        del_file(huf_name);
        del_file(decr_name);
        data.clear();
    }
}

