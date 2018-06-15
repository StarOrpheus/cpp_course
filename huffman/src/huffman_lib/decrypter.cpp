#include <vector>

#include "include/decrypter.h"

decrypter::decrypter(h_tree const& huff_tree)
        : dfa(huff_tree), current_vertex(dfa.root()) {}

void decrypter::naive_decrypt(dynamic_bitset const &input, vector<symbol> &result) {
    h_tree_DFA::DFA_step step = dfa.bit_go(current_vertex, input);
    current_vertex = step.destination;

    result.reserve(result.size() + step.visited_symbols.size());
    if (!step.visited_symbols.empty()) {
        std::copy(step.visited_symbols.begin(),
                  step.visited_symbols.end(),
                  std::back_inserter(result));
    }
}

void decrypter::decrypt(const void *ptr, size_t sz, std::vector<symbol> &result) {
    static const uint8_t lower4bits_mask = 15; // 0b00001111

    for (size_t i = 0; i < sz; ++i) {
        const uint8_t cur_byte = (static_cast<uint8_t const*>(ptr))[i];
        h_tree_DFA::DFA_step const& first_step
                = dfa.go(current_vertex, cur_byte & lower4bits_mask);

        current_vertex = first_step.destination;

        h_tree_DFA::DFA_step const& second_step
                = dfa.go(current_vertex, cur_byte >> 4u);

        current_vertex = second_step.destination;

        result.reserve(result.size()
                       + first_step.visited_symbols.size()
                       + second_step.visited_symbols.size());

        if (!first_step.visited_symbols.empty()) {
            std::copy(first_step.visited_symbols.begin(),
                      first_step.visited_symbols.end(),
                      std::back_inserter(result));
        }

        if (!second_step.visited_symbols.empty()) {
            std::copy(second_step.visited_symbols.begin(),
                      second_step.visited_symbols.end(),
                      std::back_inserter(result));
        }
    }
}

decrypter::decrypter(freq_counter const& f_counter)
        : decrypter(h_tree(f_counter)) {}