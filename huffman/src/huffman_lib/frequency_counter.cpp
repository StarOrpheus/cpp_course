#include <cstring>

#include "include/frequency_counter.h"

size_t hash(freq_counter const & f_counter) {
    const size_t modulo = 172573565537ll;
    const size_t k = 31;

    size_t result = 0;

    for (size_t c : f_counter.cnt) {
        result += c % modulo;
        result %= modulo;
        result = (result * k) % modulo;
    }

    return result;
}

freq_counter::freq_counter() {
    memset(cnt, 0, sizeof cnt);
}


freq_counter::freq_counter(size_t const *t_cnt) {
    memcpy(cnt, t_cnt, (max_symbol + 1) * sizeof(size_t));
}


void freq_counter::count(symbol const *ptr, size_t sz) {
    for (size_t i = 0; i < sz; i++) {
        cnt[ptr[i]]++;
    }
}
