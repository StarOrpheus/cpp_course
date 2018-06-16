//
// Created by starorpheus on 26/05/18.
//

#ifndef HUFFMAN_H_TREE_H
#define HUFFMAN_H_TREE_H

#include <memory>
#include <utility>
#include <limits>

#include "utils.h"
#include "frequency_counter.h"
#include "dynamic_bitset.h"

using std::shared_ptr;
using std::weak_ptr;
using std::vector;

class h_tree {
protected:
    struct __node {
        uint32_t left;
        uint32_t right;

        uint32_t weight;
        symbol c;
        bool terminal;

        __node();

        __node(uint32_t left, uint32_t right,
               uint32_t weight, symbol c, bool terminal);
    };

    struct __code_table_generator {
        dynamic_bitset & buffer;
        vector<dynamic_bitset> & result;
        vector<__node> const& g;

        __code_table_generator(dynamic_bitset &buffer,
                                vector<dynamic_bitset> &result,
                                vector<__node> const& g);

        void operator()(uint32_t);
    };

    static const uint32_t devil_vertex;

    vector<__node> g;
    uint32_t root;

public:

    explicit
    h_tree(freq_counter const& f_c);

    h_tree& operator=(h_tree const&) = delete;

    vector<dynamic_bitset> code_table() const;

    friend class h_tree_dfa;
};

#endif //HUFFMAN_H_TREE_H
