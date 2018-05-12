    //
// Created by starorpheus on 15/03/18.
//

#include "big_integer.h"
#include <algorithm>
#include <iostream>

big_integer::big_integer() : big_integer(0) {}

big_integer::big_integer(big_integer const &x) {
    this->m_data = x.m_data;
    this->sign = x.sign;
}

big_integer::big_integer(big_integer && x) {
    this->m_data = std::move(x.m_data);
    this->sign = x.sign;
}

big_integer::big_integer(int x) : m_data(1, std::abs(x)), sign(x >= 0) {}

big_integer::big_integer(uint32_t x) : m_data(1, x), sign(true) {}

big_integer::big_integer(std::string const &str) {
    if (str.length() == 0) {
        throw std::invalid_argument("Non-zero length std::string const& expected");
    }

    big_integer tmp;
    int i = 0;
    bool sign = true;

    if (str[i] == '-' || str[i] == '+') {
        sign = (str[i] == '+');
        i++;
    }

    for (; i < str.length(); i++) {
        if (!isdigit(str[i])) {
            throw std::invalid_argument("Parse error: unexpected char " + str[i]);
        }
        tmp *= 10;
        tmp += str[i] - '0';
    }

    m_data = std::move(tmp.m_data);
    if (!tmp.is_zero()) {
        this->sign = sign;
    }
}

big_integer& big_integer::operator=(big_integer && other) {
    this->m_data = std::move(other.m_data);
    this->sign = other.sign;

    return *this;
}

big_integer &big_integer::operator+=(big_integer const &rhs) {
    if (this->sign == rhs.sign) {
        uint32_t carry = 0;
        for (size_t i = 0; i < std::max(length(), rhs.length()) || carry; i++) {
            if (i == length()) {
                m_data.push_back(0);
            }

            uint64_t tmp = m_data[i];
            tmp += ((uint64_t) 1) * carry + (i < rhs.length() ? rhs.m_data[i] : 0);
            carry = (tmp >> 32);
//            m_data[i] = (tmp << 32) >> 32;
            m_data[i] = tmp;
        }
    } else {
        bool s1 = sign;
        bool s2 = rhs.sign;

        big_integer trhs(rhs);

        sign = true;
        trhs.sign = true;

        if (*this >= trhs) {
            int carry = 0;
            for (size_t i = 0; i < trhs.length() || carry; i++) {
                int64_t tmp = m_data[i];
                tmp -= carry;
                tmp -= (i < trhs.length() ? trhs.m_data[i] : 0);
                carry = tmp < 0;
                if (carry) tmp += ((int64_t) 1ll << 32);
                m_data[i] = tmp;
            }

            sign = s1;
        } else {
            int carry = 0;
            for (size_t i = 0; i < length() || carry; i++) {
                int64_t tmp = trhs.m_data[i];
                tmp -= carry;
                tmp -= (i < length() ? m_data[i] : 0);
                carry = tmp < 0;
                if (carry) tmp += ((int64_t) 1ll << 32);
                trhs.m_data[i] = tmp;
            }

            sign = s2;
            std::swap(m_data, trhs.m_data);
        }
    }

    trim_zeros();

    if (is_zero()) {
        sign = true;
    }

    return *this;
}

void big_integer::trim_zeros() {
    while (length() > 1 && m_data.back() == 0) {
        m_data.pop_back();
    }
}

big_integer &big_integer::operator-=(const big_integer &rhs) {
    this->sign ^= 1;
    operator+=(rhs);
    this->sign ^= 1;

    return *this;
}

big_integer &big_integer::operator*=(const big_integer &rhs) {
    if (rhs.is_zero() || is_zero()) {
        set_zero();
        return *this; // without allocating ram
    }

    std::vector<uint32_t> tmp(rhs.length() + length(), 0);

    for (int i = 0; i < length(); i++) {
        uint32_t carry = 0;
        for (int j = 0; j < rhs.length() || carry; j++) {
            uint64_t cur = tmp[i + j]
                           + m_data[i] * ((uint64_t) 1) * (j < rhs.length() ? rhs.m_data[j] : 0)
                           + carry;

            tmp[i + j] = (cur << 32) >> 32;
            carry = cur >> 32;
        }
    }

    std::swap(m_data, tmp);
    sign ^= rhs.sign;
    sign ^= 1;

    trim_zeros();

    return *this;
}

void big_integer::set_zero() {
    move(big_integer());
}

big_integer abs(big_integer const &x) {
    big_integer res(x);
    res.sign = true;
    return res;
}

std::pair<big_integer, big_integer> big_integer::div_mod(big_integer const &b) const {
    const uint32_t max_dig = 0xFFFFFFFF;

    if (b.length() == 1 && b.m_data[0] == 0) {
        throw std::runtime_error("Division by zero");
    }

    big_integer tempr(*this);
    tempr.sign ^= (b.sign ^ 1);

    if (b.length() == 1) {
        auto k = tempr.div_mod(b.m_data[0]);
        big_integer c2 = big_integer(k.second);
        if (!c2.is_zero()) {
            c2.sign = this->sign;
        }

        return std::make_pair(std::move(k.first), std::move(c2));
    }

    big_integer abs_x(abs(tempr));
    big_integer abs_y(abs(b));

    if (abs_x < abs_y) {
        tempr.set_zero();
        return std::make_pair(tempr, tempr); // with move or not??
    }

    uint32_t f = (((uint64_t) 1) << 32) / ((uint64_t) b.m_data.back() + 1);

    abs_x *= f;
    abs_y *= f;

    std::vector<uint32_t> result(abs_x.length() - abs_y.length() + 1);

    big_integer h;
    uint64_t d1 = abs_y.m_data.back();

    for (std::ptrdiff_t k = abs_x.length() - 1; k > abs_x.length() - abs_y.length(); k--) {
        h <<= 32;
        h += abs_x.m_data[k];
    }

    for (size_t k = abs_x.length() - abs_y.length() + 1; k--;) {
        h <<= 32;
        h += abs_x.m_data[k];

        uint64_t r2 = h.m_data.back();
        if (h.length() > abs_y.length()) {
            r2 <<= 32;
            r2 += h.m_data[h.length() - 2];
        }

        //       quotient
        uint64_t qt = std::min(r2 / d1, (uint64_t) max_dig);
        big_integer dq = abs_y * (uint32_t) (qt & max_dig);
        while (h < dq) {
            qt--;
            dq -= abs_y;
        }
        result[k] = qt & max_dig;
        h -= dq;
    }

    tempr.m_data = std::move(result);
    tempr.trim_zeros();

    h.sign = this->sign;

    return std::make_pair(std::move(tempr), std::move(h)); // with move or not??
}

big_integer &big_integer::operator/=(big_integer const &b) {
    move(div_mod(b).first);
    return *this;
}

big_integer &big_integer::operator%=(big_integer const &b) {
    move(div_mod(b).second);
    return *this;
}

std::pair<big_integer, uint32_t> big_integer::div_mod(uint32_t dividend) const {
    if (dividend == 0) {
        throw std::invalid_argument("Division by zero: second argument is 0");
    }

    std::pair<big_integer, uint32_t> res;

    res.second = 0;
    res.first.sign = sign;
    res.first.m_data.resize(length());

    for (std::ptrdiff_t i = length() - 1; i >= 0; i--) {
        uint64_t cur = m_data[i] + ((res.second * ((uint64_t) 1)) << 32);
        res.first.m_data[i] = cur / dividend;
        res.second = cur % dividend;
    }

    res.first.trim_zeros();
    return res;
}

big_integer &big_integer::operator&=(big_integer const &rhs) {
    move(apply_with_complement(*this,
                               rhs,
                               [](uint32_t x, uint32_t y) -> uint32_t {
                                   return x & y;
                               }));

    if (is_zero()) {
        sign = true;
    }

    return *this;
}

big_integer &big_integer::operator|=(big_integer const &rhs) {
    move(apply_with_complement(*this,
                               rhs,
                               [](uint32_t x, uint32_t y) -> uint32_t {
                                   return x | y;
                               }));

    if (is_zero()) {
        sign = true;
    }

    return *this;
}

big_integer &big_integer::operator^=(big_integer const &rhs) {
    move(apply_with_complement(*this,
                               rhs,
                               [](uint32_t x, uint32_t y) -> uint32_t {
                                   return x ^ y;
                               }));

    if (is_zero()) {
        sign = true;
    }

    return *this;
}

big_integer &big_integer::operator<<=(int rhs) {
    big_integer tmp(*this);
    tmp.m_data.resize(length() + rhs / 32 + 2);

    int c1 = rhs >> 5;
    int c2 = (rhs << 27) >> 27;

    if (c1) {
        for (std::ptrdiff_t i = length() + c1 - 1; i >= (rhs >> 5); i--) {
            tmp.m_data[i] = tmp.m_data[i - c1];
        }

        for (int i = (rhs >> 5) - 1; i >= 0; i--) {
            tmp.m_data[i] = 0;
        }
    }

    if (c2) {
        for (std::ptrdiff_t i = length() + c1 - 1; i >= 0; i--) {
            tmp.m_data[i + 1] |= (tmp.m_data[i] >> (32 - c2));
            tmp.m_data[i] <<= c2;
        }
    }

    move(tmp);
    trim_zeros();

    return *this;
}

big_integer &big_integer::operator>>=(int rhs) {
    std::vector<uint32_t> tmp(get_complement_data(*this));

    int c1 = rhs >> 5;
    int c2 = rhs & 0x1F;

    if (c1) {
        for (std::ptrdiff_t i = 0; i + c1 < length(); i++) {
            tmp[i] = tmp[i + c1];
        }

        for (std::ptrdiff_t i = length() - c1; i < length(); i++) {
            tmp[i] = (tmp[length() - 1] >> 31) ? 0xFFFFFFFF : 0;
        }
    }

    if (c2) {
        uint32_t complement = (tmp.back() >> (32 - c2)) << (32 - c2);
        for (std::ptrdiff_t i = 0; i < length() - c1; i++) {
            if (i) {
                tmp[i - 1] |= (tmp[i] << (32 - c2));
            }
            tmp[i] >>= c2;
        }

        tmp[length() - c1 - 1] |= complement;
    }

    move(get_decomplement_data(tmp));

    trim_zeros();

    return *this;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    if (is_zero()) {
        return *this;
    }

    big_integer tmp(*this);

    tmp.sign ^= 1;
    return tmp;
}

big_integer big_integer::operator~() const {
    return -(*this) - 1;
}

big_integer &big_integer::operator++() {
    return operator+=(1);
}

big_integer big_integer::operator++(int) {
    big_integer tmp(*this);
    operator+=(1);
    return tmp;
}

big_integer &big_integer::operator--() {
    return operator-=(1);
}

big_integer big_integer::operator--(int) {
    big_integer tmp(*this);
    operator-=(1);
    return tmp;
}

bool operator==(big_integer const &a, big_integer const &b) {
    return a.sign == b.sign && a.m_data == b.m_data;
}

bool operator!=(big_integer const &a, big_integer const &b) {
    return a.sign != b.sign || a.m_data != b.m_data;
}

bool operator<(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign) { return !a.sign; }
    if (a.length() != b.length()) { return a.length() < b.length(); }

    for (std::ptrdiff_t i = a.length() - 1; i >= 0; i--) {
        if (a.m_data[i] != b.m_data[i]) {
            return a.m_data[i] < b.m_data[i];
        }
    }

    return false;
}

bool operator>(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign) { return a.sign; }
    if (a.length() != b.length()) { return a.length() > b.length(); }

    for (std::ptrdiff_t i = a.length() - 1; i >= 0; i--) {
        if (a.m_data[i] != b.m_data[i]) {
            return a.m_data[i] > b.m_data[i];
        }
    }

    return false;
}

bool operator<=(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign) { return !a.sign; }
    if (a.length() != b.length()) { return a.length() < b.length(); }

    for (std::ptrdiff_t i = a.length() - 1; i >= 0; i--) {
        if (a.m_data[i] != b.m_data[i]) {
            return a.m_data[i] < b.m_data[i];
        }
    }

    return true;
}

bool operator>=(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign) { return a.sign; }
    if (a.length() != b.length()) { return a.length() > b.length(); }

    for (std::ptrdiff_t i = a.length() - 1; i >= 0; i--) {
        if (a.m_data[i] != b.m_data[i]) {
            return a.m_data[i] > b.m_data[i];
        }
    }

    return true;
}

std::string to_string(big_integer const &a) {
    if (a.is_zero()) {
        return "0";
    }

    std::string s;
    big_integer tmp(a);

    while (!tmp.is_zero()) {
        std::pair<big_integer, uint32_t> res = tmp.div_mod(10);
        s += (char) res.second + '0';
        tmp = res.first;
    }

    if (!a.sign) {
        s += '-';
    }

    std::reverse(s.begin(), s.end());

    return s;
}

std::ostream &operator<<(std::ostream &s, big_integer const &a) {
    return s << to_string(a);
}

bool big_integer::is_zero() const {
    return length() == 1 && m_data[0] == 0;
}

big_integer operator+(big_integer a, big_integer const &b) {
    a += b;
    return a;
}

big_integer operator-(big_integer a, big_integer const &b) {
    a -= b;
    return a;
}

big_integer operator*(big_integer a, big_integer const &b) {
    a *= b;
    return a;
}

big_integer operator/(big_integer a, big_integer const &b) {
    a /= b;
    return a;
}

big_integer operator%(big_integer a, big_integer const &b) {
    a %= b;
    return a;
}

big_integer operator&(big_integer a, big_integer const &b) {
    a &= b;
    return a;
}

big_integer operator|(big_integer a, big_integer const &b) {
    a |= b;
    return a;
}

big_integer operator^(big_integer a, big_integer const &b) {
    a ^= b;
    return a;
}

big_integer operator<<(big_integer a, int b) {
    return a <<= b;
}

big_integer operator>>(big_integer a, int b) {
    return a >>= b;
}

// integer length in radix 2^32, without sign
size_t big_integer::length() const {
    return m_data.size();
}

std::vector<uint32_t> big_integer::get_data() const {
    return m_data;
}

std::vector<uint32_t> get_complement_data(big_integer const &x) {
    if (x.sign) {
        return x.m_data;
    }

    big_integer tmp(x);
    tmp.sign = true;

    for (int i = 0; i < tmp.length(); i++) {
        tmp.m_data[i] = ~tmp.m_data[i];
    }

    tmp++;
    return std::move(tmp.m_data);
}

big_integer get_decomplement_data(std::vector<uint32_t> const &x) {
    if (x.back() & (1 << 31)) {
        big_integer tmp;
        tmp.m_data = x;

        tmp--;
        for (int i = 0; i < tmp.length(); i++) {
            tmp.m_data[i] = ~tmp.m_data[i];
        }

        tmp.sign = false;
        return tmp;
    } else {
        big_integer tmp;
        tmp.m_data = x;
        return tmp;
    }
}

std::vector<uint32_t> apply_to_vectors(std::vector<uint32_t> const &x,
                                       std::vector<uint32_t> const &y,
                                       std::function<uint32_t(uint32_t, uint32_t)> F) {
    std::vector<uint32_t> tmp(x);
    if (tmp.size() < y.size()) {
        tmp.resize(y.size(), 0);
    }

    for (int i = 0; i < tmp.size(); i++) {
        tmp[i] = F(tmp[i], (i < y.size() ? y[i] : 0));
    }

    return tmp;
}

big_integer apply_with_complement(big_integer const &x,
                                  big_integer const &y,
                                  std::function<uint32_t(uint32_t, uint32_t)> F) {
    return get_decomplement_data(apply_to_vectors(get_complement_data(x),
                                                  get_complement_data(y),
                                                  std::move(F)));
}

void big_integer::move(big_integer const& from) {
    this->m_data = std::move(from.m_data);
    this->sign = from.sign;
}

