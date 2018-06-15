//
// Created by starorpheus on 09/06/18.
//

#include <vector>
#include <cassert>

#include "include/utils.h"

h_tree_DFA::h_tree_DFA(const h_tree &m_tr)
        : m_tr(m_tr),
          edges(m_tr.g.size(), std::vector<DFA_step>(1u << 4u)) {

    // Let's do it naively
    for (uint32_t start_vertex = 0; start_vertex < m_tr.g.size(); ++start_vertex) {
        if (m_tr.g[start_vertex].terminal) continue;

        for (uint8_t way = 0; way < (1u << 4u); ++way) {
            edges[start_vertex][way] = bit_go(start_vertex, static_cast<uint8_t>(way));
        }
    }
}

h_tree_DFA::DFA_step const & h_tree_DFA::go(uint32_t vertex_dicriptor,
                                            uint8_t data) {
    return edges[vertex_dicriptor][data];
}

h_tree_DFA::DFA_step h_tree_DFA::bit_go(uint32_t cur_v, dynamic_bitset const &way) {
    DFA_step result{h_tree::devil_vertex, std::vector<symbol >()};

    assert (cur_v != h_tree::devil_vertex);

    for (size_t i = 0; i < way.length(); ++i) {
        if (!way[i]) {
            cur_v = m_tr.g[cur_v].left;
        } else {
            cur_v = m_tr.g[cur_v].right;
        }

        if (cur_v == h_tree::devil_vertex) {
            cur_v = m_tr.root;
        }

        if (m_tr.g[cur_v].terminal) {
            result.visited_symbols.push_back(m_tr.g[cur_v].c);
            cur_v = m_tr.root;
        }
    }

    result.destination = cur_v;

    return std::move(result);
}


// overloaded to skip unnecessary dynamic_bitset instances
h_tree_DFA::DFA_step h_tree_DFA::bit_go(uint32_t cur_v, uint8_t way) {
    DFA_step result{h_tree::devil_vertex, std::vector<symbol >()};

    assert (cur_v != h_tree::devil_vertex);

    for (uint8_t i = 0; i < 4; ++i) {
        if ((way & (1u << i)) == 0) {
            cur_v = m_tr.g[cur_v].left;
        } else {
            cur_v = m_tr.g[cur_v].right;
        }

        if (cur_v == h_tree::devil_vertex) {
            cur_v = m_tr.root;
        }

        if (m_tr.g[cur_v].terminal) {
            result.visited_symbols.push_back(m_tr.g[cur_v].c);
            cur_v = m_tr.root;
        }
    }

    result.destination = cur_v;

    return std::move(result);
}

const uint32_t h_tree_DFA::root() const {
    return m_tr.root;
}

h_tree_DFA::DFA_step::DFA_step(uint32_t destination, vector<symbol> const& visited_symbols)
        : destination(destination), visited_symbols(visited_symbols) {}

h_tree_DFA::DFA_step::DFA_step()
        : destination(h_tree::devil_vertex) {
//    visited_symbols.reserve(4);
}
