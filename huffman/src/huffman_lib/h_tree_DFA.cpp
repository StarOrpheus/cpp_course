//
// Created by starorpheus on 09/06/18.
//

#include <vector>
#include <cassert>

#include "include/utils.h"

h_tree_DFA::h_tree_DFA(const h_tree &m_tr)
        : m_tr(m_tr),
          edges(1u << 16u) {

    for (uint32_t way = 0; way < (1u << 16u); ++way) {
        current_vertex = m_tr.root;
        edges[way] = bit_go(static_cast<uint16_t >(way));
    }

    current_vertex = m_tr.root;
}

void h_tree_DFA::bit_go(dynamic_bitset const &way, std::vector<symbol> &result) {
    for (size_t i = 0; i < way.length(); ++i) {
        if (!way[i]) {
            current_vertex = m_tr.g[current_vertex].left;
        } else {
            current_vertex = m_tr.g[current_vertex].right;
        }

        if (current_vertex == h_tree::devil_vertex) {
            current_vertex = m_tr.root;
        }

        if (m_tr.g[current_vertex].terminal) {
            result.push_back(m_tr.g[current_vertex].c);
            current_vertex = m_tr.root;
        }
    }
}



// overloaded to skip unnecessary dynamic_bitset instances
h_tree_DFA::DFA_step h_tree_DFA::bit_go(uint16_t way) {
    assert (current_vertex != h_tree::devil_vertex);

    for (uint8_t i = 0; i < 16; ++i) {
        if ((way & (1u << i)) == 0) {
            current_vertex = m_tr.g[current_vertex].left;
        } else {
            current_vertex = m_tr.g[current_vertex].right;
        }

        if (current_vertex == h_tree::devil_vertex) {
            current_vertex = m_tr.root;
        }

        if (m_tr.g[current_vertex].terminal) {
            const DFA_step result{m_tr.root, true,
                            m_tr.g[current_vertex].c,
                            static_cast<uint8_t>(16 - i - 1)};

            current_vertex = m_tr.root;

            return result;
        }
    }

    return DFA_step{current_vertex, false, 0, 0};
}

const uint32_t h_tree_DFA::root() const {
    return m_tr.root;
}

const uint32_t h_tree_DFA::condition() const {
    return current_vertex;
}

h_tree_DFA::DFA_step const& h_tree_DFA::root_go(uint16_t way) {
    DFA_step const& step = edges[way];

    current_vertex = step.destination;

    return step;
}

h_tree_DFA::DFA_step h_tree_DFA::bit_go(bool way) {
    if (!way) {
        current_vertex = m_tr.g[current_vertex].left;
    } else {
        current_vertex = m_tr.g[current_vertex].right;
    }

    if (current_vertex == h_tree::devil_vertex) {
        current_vertex = m_tr.root;
    }

    if (m_tr.g[current_vertex].terminal) {
        const DFA_step result{m_tr.root, true, m_tr.g[current_vertex].c, 0};
        current_vertex = m_tr.root;
        return result;
    } else {
        return DFA_step{current_vertex, false, 0, 0};
    }
}

h_tree_DFA::DFA_step::DFA_step() : destination(h_tree::devil_vertex) {}

h_tree_DFA::DFA_step::DFA_step(uint32_t destination,
                               bool symbol_achieved,
                               symbol c,
                               uint8_t skipped_bits)
        : destination(destination), symbol_achieved(symbol_achieved),
          c(c), skipped_bits(skipped_bits) {}
