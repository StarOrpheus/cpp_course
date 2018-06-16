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
        bool symbol_achieved;
        symbol c;
        uint8_t skipped_bits = 0;

        DFA_step(uint32_t destination, bool symbol_achieved, symbol c, uint8_t skipped_bits);

        DFA_step();
    };

private:
    h_tree const& m_tr;
    vector<DFA_step > edges;
    uint32_t current_vertex;

public:

    explicit h_tree_DFA(const h_tree &m_tr);

    const uint32_t root() const;
    const uint32_t condition() const;

    const DFA_step & root_go(uint16_t way);

    // overloaded to skip unnecessary dynamic_bitset instances and
    void bit_go(dynamic_bitset const &way, std::vector<symbol> &result);
    DFA_step bit_go(uint16_t way);
    DFA_step bit_go(uint8_t way);
    DFA_step bit_go(bool way);

};


#endif //HUFFMAN_H_TREE_DFA_H
