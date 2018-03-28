//
// Created by starorpheus on 15/03/18.
//

#ifndef PROJECT_BIG_INT_H
#define PROJECT_BIG_INT_H


#include <bits/unique_ptr.h>
#include <vector>
#include <functional>

class big_integer {
    std::vector<uint32_t > data;
public:
    bool sign;

    big_integer();
    big_integer(big_integer const& x);
    big_integer(int x);
    explicit big_integer(const std::string & str);
    ~big_integer() = default;


    big_integer& operator=(big_integer const& other) = default;

    big_integer& operator+=(big_integer const& rhs);
    big_integer& operator-=(big_integer const& rhs);
    big_integer& operator*=(big_integer const& rhs);
    big_integer& operator/=(big_integer const& rhs);
    big_integer& operator%=(big_integer const& rhs);
    std::pair<big_integer, uint32_t > div_mod(uint32_t devidend) const;

    big_integer& operator&=(big_integer const& rhs);
    big_integer& operator|=(big_integer const& rhs);
    big_integer& operator^=(big_integer const& rhs);

    big_integer& operator<<=(int rhs);
    big_integer& operator>>=(int rhs);

    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;

    big_integer& operator++();
    big_integer operator++(int);

    big_integer& operator--();
    big_integer operator--(int);

    friend bool operator==(big_integer const& a, big_integer const& b);
    friend bool operator!=(big_integer const& a, big_integer const& b);
    friend bool operator<(big_integer const& a, big_integer const& b);
    friend bool operator>(big_integer const& a, big_integer const& b);
    friend bool operator<=(big_integer const& a, big_integer const& b);
    friend bool operator>=(big_integer const& a, big_integer const& b);

    friend std::string to_string(big_integer const& a);

    bool is_zero() const;

    void trim_zeros();
    void set_zero();

    size_t length() const;
    std::vector<uint32_t > get_data() const;

    friend big_integer apply_with_complement(big_integer const& x,
                                             big_integer const& y,
                                             std::function<uint32_t(uint32_t, uint32_t)> F);
    friend std::vector<uint32_t> get_complement_data(big_integer const& x);
    friend big_integer get_decomplement_data(std::vector<uint32_t> const& x);
};

std::vector<uint32_t> get_complement_data(big_integer const& x);
big_integer get_decomplement_data(std::vector<uint32_t> const& x);

std::vector<uint32_t> apply_to_vectors(std::vector<uint32_t> const& x,
                                       std::vector<uint32_t> const& y,
                                       std::function<uint32_t(uint32_t, uint32_t)> F);

big_integer apply_with_complement(big_integer const& x,
                                  big_integer const& y,
                                  std::function<uint32_t(uint32_t, uint32_t)> F);

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);

big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer operator<<(big_integer a, int b);
big_integer operator>>(big_integer a, int b);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

std::string to_string(big_integer const& a);
std::ostream& operator<<(std::ostream& s, big_integer const& a);

#endif //PROJECT_BIG_INT_H
