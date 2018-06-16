#include <queue>
#include <functional>
#include <iostream>

#include "include/h_tree.h"

using std::pair;

const uint32_t h_tree::devil_vertex = std::numeric_limits<uint32_t >::max();

h_tree::h_tree(freq_counter const &f_c) {
    std::priority_queue<pair<uint32_t, uint32_t >,
            std::vector<pair<uint32_t, uint32_t > >,
            std::greater<pair<uint32_t, uint32_t > > > q;

    for (uint32_t c = 0; c <= max_symbol; ++c) {
        g.emplace_back(h_tree::devil_vertex,
                       h_tree::devil_vertex,
                       f_c.cnt[c], c, true);
        q.emplace(f_c.cnt[c], c);
    }

    while (q.size() > 1u) {
        pair<uint32_t, uint32_t > lhs = q.top(); q.pop();
        pair<uint32_t, uint32_t > rhs = q.top(); q.pop();

        g.emplace_back(lhs.second, rhs.second,
                       lhs.first + rhs.first, max_symbol, false);

        q.emplace(lhs.first + rhs.first, g.size() - 1);
    }

    root = q.top().second;

    g.shrink_to_fit();
}

void h_tree::__code_table_generator::operator()(uint32_t vertex) {
    __node const& cur_node = g[vertex];

    if (cur_node.terminal) {
        result[cur_node.c] = buffer;
    }

    if (cur_node.left != devil_vertex) {
        buffer.push_back(0);

        operator()(cur_node.left);

        buffer.pop_back();
    }

    if (cur_node.right != devil_vertex) {
        buffer.push_back(1);

        operator()(cur_node.right);

        buffer.pop_back();
    }
}

h_tree::__code_table_generator::__code_table_generator(dynamic_bitset &buffer, vector<dynamic_bitset> &result,
                                                       const vector<h_tree::__node> &g)
        : buffer(buffer), result(result), g(g) {}


vector<dynamic_bitset> h_tree::code_table() const {
    dynamic_bitset buffer;
    vector<dynamic_bitset> result;
    result.resize(max_symbol + 1);

    __code_table_generator(buffer, result, g)(root);

    return std::move(result);
}

h_tree::__node::__node() : left(devil_vertex),
                           right(devil_vertex),
                           weight(std::numeric_limits<uint32_t >::max()),
                           c(max_symbol) {}

h_tree::__node::__node(uint32_t left, uint32_t right, uint32_t weight, symbol c, bool terminal)
        : left(left), right(right),
          weight(weight), c(c), terminal(terminal) {}
