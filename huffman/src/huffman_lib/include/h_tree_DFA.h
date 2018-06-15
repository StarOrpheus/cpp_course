//
// Created by starorpheus on 09/06/18.
//

#ifndef HUFFMAN_H_TREE_DFA_H
#define HUFFMAN_H_TREE_DFA_H

#include <vector>

#include "h_tree.h"

using std::vector;

class h_tree_DFA {
public:
    struct DFA_step {
        uint32_t destination = h_tree::devil_vertex;
        vector<symbol> visited_symbols;

        DFA_step(uint32_t destination, const vector<symbol> &visited_symbols);
        DFA_step();
    };

private:
    h_tree const& m_tr;
    vector<vector<DFA_step > > edges;

public:

    explicit h_tree_DFA(const h_tree &m_tr);

    const uint32_t root() const;

    const DFA_step & go(uint32_t vertex_dicriptor,
                        uint8_t data);

    DFA_step bit_go(uint32_t vertex_dicriptor,
                    dynamic_bitset const &way);

    // overloaded to skip unnecessary dynamic_bitset instances
    DFA_step bit_go(uint32_t cur_v,
                    uint8_t way);
};


#endif //HUFFMAN_H_TREE_DFA_H
