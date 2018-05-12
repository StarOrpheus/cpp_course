#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <utility>
#include <functional>

#include "../include/vector.hpp"
#include "gtest/gtest.h"

template<typename T>
bool operator==(vector<T> const& a, std::vector<T> const& b) {
    if (a.size() != b.size()) {
        std::cerr << a.size() << " size != " << b.size() << " size" << std::endl;
        return false;
    }

    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            std::cerr << i << std::endl;
            return false;
        }
    }

    vector<int>::const_iterator it1 = a.begin();
    std::vector<int>::const_iterator it2 = b.begin();

    for (; it1 != a.end(); it1++, it2++) {
        if (*it1 != *it2) {
            std::cerr << *it1 << " " << *it2 << std::endl;
            return false;
        }
    }

    return true;
}

TEST(ctor, empty_eq) {
    vector<int > a;
    vector<int > b;

    ASSERT_EQ(a, b);
}

TEST(ctor, default_resized) {
    std::vector<int> a(5);
    vector<int> b(5);

    std::vector<int>::iterator it1 = a.begin();
    vector<int>::iterator it2 = b.begin();

    for (; it1 != a.end(); it1++, it2++) {
        ASSERT_TRUE(*it1 == *it2);
    }

    ASSERT_EQ(it1, a.end());
    ASSERT_EQ(it2, b.end());
}

TEST(push_back, simple_add) {
    std::vector<int> a;
    for (size_t i = 0; i < 100; i++) {
        a.push_back(i);
    }

    vector<int> b;
    for (size_t i = 0; i < 100; i++) {
        b.push_back(i);
    }

    ASSERT_TRUE(b == a);
}

TEST(swap, simple_swap) {
    vector<int> a;
    for (size_t i = 0; i < 2; i++) {
        a.push_back(i);
    }

    vector<int> b;
    for (size_t i = 13; i < 15; i++) {
        b.push_back(i);
    }

    swap(a, b);

    ASSERT_TRUE(a[0] == 13);
    ASSERT_TRUE(a[1] == 14);
    ASSERT_TRUE(b[0] == 0);
    ASSERT_TRUE(b[1] == 1);
}

TEST(merge, random) {
    std::vector<std::function<void() > > operations;
    vector<int> a;
    std::vector<int> b;

    operations.push_back([&] () {
        int x = rand();
        a.push_back(x);
        b.push_back(x);
    });

    operations.push_back([&] () {
        if (a.size() > 1) {
            a.pop_back();
            b.pop_back();
        }
    });

    operations.push_back([&] () {
        size_t newSize = rand() % 100 + rand() % 13;

        a.resize(newSize);
        b.resize(newSize);
    });

    operations.push_back([&] () {
        size_t len = rand() % 10;
        int val = rand();

        a.assign(len, val);
        b.assign(len, val);
    });

    operations.push_back([&] () {
        vector<int> tmp;
        for (size_t i = 0; i < rand() % 1000; ++i) {
            tmp.push_back(rand());
        }

        a.assign(tmp.begin(), tmp.end());
        b.assign(tmp.begin(), tmp.end());
    });

    operations.push_back([&] () {
        a.clear();
        b.clear();
    });

    for (size_t i = 0; i < 100000; i++) {
        operations[rand() % operations.size()]();
        ASSERT_TRUE(a == b);
    }
}