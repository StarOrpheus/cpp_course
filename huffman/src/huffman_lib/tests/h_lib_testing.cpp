//
// Created by starorpheus on 10/06/18.
//

#include "../../../libs/gtest/gtest.h"

#include "../include/encrypter.h"
#include "../include/decrypter.h"
#include "../include/utils.h"

#include <string>
#include <vector>

using std::string;

bool test_eq(dynamic_bitset const& lhs, vector<bool> const& rhs) {
    if (lhs.length() != rhs.size()) {
        return false;
    }

    for (size_t i = 0; i < lhs.length(); ++i) {
        if (lhs[i] != rhs[i]) {
//            std::cerr << "At pos " << i << std::endl;
            return false;
        }
    }

    return true;
}

bool bitset_test(vector<bool> const& test) {
    dynamic_bitset bs;
    vector<bool> cur;

    for (size_t i = 0; i < test.size(); i++) {
        if (!test_eq(bs, cur)) {
            return false;
        }

        bs.push_back(test[i]);
        cur.push_back(test[i]);
    }

    if (!test_eq(bs, cur)) {
        return false;
    }

    for (size_t i = 0; i < test.size(); i++) {
        if (!test_eq(bs, cur)) {
            return false;
        }

        bs.pop_back();
        cur.pop_back();
    }

    return true;
}

//TEST(bitset, positioning) {
//    vector<std::pair<size_t, vector<size_t > > > tests {
//        {0, {0, 0}},
//        {1, {0, 1}},
//        {31, {0, 31}},
//        {32, {0, 32}},
//        {33, {0, 33}},
//        {63, {0, 63}},
//        {64, {1, 0}},
//        {65, {1, 1}},
//        {66, {1, 2}},
//        {127, {1, 63}},
//        {128, {2, 0}},
//        {129, {2, 1}}};
//
//    for (auto test: tests) {
//        vector<size_t > testing{dynamic_bitset::byte_pos(test.first), dynamic_bitset::bit_pos(test.first)};
//
//        ASSERT_TRUE(testing[0] == test.second[0]);
//        ASSERT_TRUE(testing[1] == test.second[1]);
//    }
//
//}

TEST(bitset, simple_zeros) {
    vector<bool> test;

    for (size_t i = 0; i < 6; i++) {
        test.push_back(0);
    }

    ASSERT_TRUE(bitset_test(test));
}

TEST(bitset, simple_ones) {
    vector<bool> test;

    for (size_t i = 0; i < 32; i++) {
        test.push_back(1);
    }

    ASSERT_TRUE(bitset_test(test));
}

TEST(bitset, hundr_zeros) {
    vector<bool> test;

    for (size_t i = 0; i < 100; i++) {
        test.push_back(0);
    }

    ASSERT_TRUE(bitset_test(test));
}

TEST(bitset, hundr_ones) {
    vector<bool> test;

    for (size_t i = 0; i < 100; i++) {
        test.push_back(1);
    }

    ASSERT_TRUE(bitset_test(test));
}

std::pair<vector<bool>, dynamic_bitset > generate_mask(size_t len) {
    vector<bool > result_first;
    dynamic_bitset result_second;

    for (size_t done = 0; done < len; ) {
        int generated = rand();

        for (size_t j = 0; done < len && j < sizeof(generated) * 8; ++j, ++done) {
            result_first.push_back((generated & (1u << j)) > 0u);
            result_second.push_back((generated & (1u << j)) > 0u);
        }
    }

    return std::make_pair(std::move(result_first), std::move(result_second));
}

TEST(bitset, append_empty_and_small) {
    dynamic_bitset curbs;
    vector<bool> curv;
    auto add = generate_mask(14);

    curbs += add.second;
    curv.insert(curv.end(), std::begin(add.first), std::end(add.first));

    ASSERT_TRUE(test_eq(curbs, curv));
}

TEST(bitset, append_small_and_empty) {
    auto cur = generate_mask(14);

    cur.second += dynamic_bitset();

    ASSERT_TRUE(test_eq(cur.second, cur.first));
}

TEST(bitset, append_small_and_small_small_result) {
    auto cur = generate_mask(7);
    dynamic_bitset curbs(cur.second);
    vector<bool> curv(cur.first);
    auto add = generate_mask(14);


    curbs += add.second;
    curv.insert(curv.end(), std::begin(add.first), std::end(add.first));

    ASSERT_TRUE(test_eq(curbs, curv));
}

TEST(bitset, append_small_and_small_big_result) {
    auto cur = generate_mask(28);
    dynamic_bitset curbs(cur.second);
    vector<bool> curv(cur.first);
    auto add = generate_mask(14);


    curbs += add.second;
    curv.insert(curv.end(), std::begin(add.first), std::end(add.first));

    ASSERT_TRUE(test_eq(curbs, curv));
}

TEST(bitset, append_big_and_small) {
    auto cur = generate_mask(150);
    dynamic_bitset curbs(cur.second);
    vector<bool> curv(cur.first);
    auto add = generate_mask(14);


    curbs += add.second;
    curv.insert(curv.end(), std::begin(add.first), std::end(add.first));

    ASSERT_TRUE(test_eq(curbs, curv));
}

TEST(bitset, append_big_and_big) {
    auto cur = generate_mask(150);
    dynamic_bitset curbs(cur.second);
    vector<bool> curv(cur.first);
    auto add = generate_mask(218);


    curbs += add.second;
    curv.insert(curv.end(), std::begin(add.first), std::end(add.first));

    ASSERT_TRUE(test_eq(curbs, curv));
}


//TEST(bitset, random) {
//    vector<bool> cur_v;
//    dynamic_bitset cur_bs;
//
//    for (size_t iter = 0; iter < 1000; iter++) {
//        int val = rand();
//
//        for (size_t i = 0; i < 32; i++) {
//            if ((rand() & 1) || cur_v.empty()) {
//                if (rand() & 1) {
//                    const bool cur_bit = (val & (1u << i)) > 0;
//                    cur_v.push_back(cur_bit);
//                    cur_bs.push_back(cur_bit);
//                } else {
//                    auto add = generate_mask(rand() % 35);
//
//                    cur_bs += add.second;
//                    cur_v.insert(cur_v.end(), add.first.begin(), add.first.end());
//                }
//            } else {
//                cur_v.pop_back();
//                cur_bs.pop_back();
//            }
//
//            bool result = test_eq(cur_bs, cur_v);
//            if (!result) {
//                std::cerr << iter << " " << i << std::endl;
//            }
//            ASSERT_TRUE(result);
//        }
//    }
//}

bool test_naive(string const& test_str) {
    freq_counter counter;
    counter.count(reinterpret_cast<const symbol *>(test_str.data()),
                  test_str.size());

    h_tree tr(counter);

    encrypter encr(tr);
    decrypter decr(tr);

    dynamic_bitset buf;
    encr.encrypt(reinterpret_cast<const symbol *>(test_str.data()),
                 test_str.size(),
                 buf);

    vector<symbol > s2;
    decr.naive_decrypt(buf, s2);

    if (test_str.size() != s2.size()) {
        return false;
    }

    for (size_t i = 0; i < s2.size(); ++i) {
        if (test_str[i] != s2[i]) {
            return false;
        }
    }

    return true;
}

bool test_dfa(string const& test_str) {
    freq_counter counter;
    counter.count(reinterpret_cast<const symbol *>(test_str.data()),
                  test_str.size());

    h_tree tr(counter);

    encrypter encr(tr);
    decrypter decr(tr);

    dynamic_bitset buf;
    encr.encrypt(reinterpret_cast<const symbol *>(test_str.data()),
                 test_str.size(),
                 buf);

    vector<symbol > s2;
    decr.decrypt(buf.data(), buf.full_blocks() * sizeof(dynamic_bitset::BlockT), s2);
    buf.truncate();
    decr.naive_decrypt(buf, s2);

    if (test_str.size() != s2.size()) {
        return false;
    }

    for (size_t i = 0; i < s2.size(); ++i) {
        if (test_str[i] != s2[i]) {
            return false;
        }
    }

    return true;
}

TEST(correctness, one_dig) {
    const string s1 = "0";

    ASSERT_TRUE(test_naive(s1));
    ASSERT_TRUE(test_dfa(s1));
}

TEST(correctness, one_symb_limits) {
    string s1;
    s1.push_back(char(0));

    ASSERT_TRUE(test_naive(s1));
    ASSERT_TRUE(test_dfa(s1));

    s1.clear();
    s1.push_back(char(255));

    ASSERT_TRUE(test_naive(s1));
    ASSERT_TRUE(test_dfa(s1));
}

TEST(correctness, simple_digits) {
    const string s1 = "01233456789";

    ASSERT_TRUE(test_naive(s1));
    ASSERT_TRUE(test_dfa(s1));
}

TEST(correctness, lilechka) {
    string s1 = "Дым табачный воздух выел.\n"
                "Комната —\n"
                "глава в крученыховском аде.\n"
                "Вспомни —\n"
                "за этим окном\n"
                "впервые\n"
                "руки твои, исступленный, гладил.\n"
                "Сегодня сидишь вот,\n"
                "сердце в железе.\n"
                "День еще —\n"
                "выгонишь,\n"
                "можешь быть, изругав.\n"
                "В мутной передней долго не влезет\n"
                "сломанная дрожью рука в рукав.\n"
                "Выбегу,\n"
                "тело в улицу брошу я.\n"
                "Дикий,\n"
                "обезумлюсь,\n"
                "отчаяньем иссечась.\n"
                "Не надо этого,\n"
                "дорогая,\n"
                "хорошая,\n"
                "дай простимся сейчас.\n"
                "Все равно\n"
                "любовь моя —\n"
                "тяжкая гиря ведь —\n"
                "висит на тебе,\n"
                "куда ни бежала б.\n"
                "Дай в последнем крике выреветь\n"
                "горечь обиженных жалоб.\n"
                "Если быка трудом уморят —\n"
                "он уйдет,\n"
                "разляжется в холодных водах.\n"
                "Кроме любви твоей,\n"
                "мне\n"
                "нету моря,\n"
                "а у любви твоей и плачем не вымолишь отдых.\n"
                "Захочет покоя уставший слон —\n"
                "царственный ляжет в опожаренном песке.\n"
                "Кроме любви твоей,\n"
                "мне\n"
                "нету солнца,\n"
                "а я и не знаю, где ты и с кем.\n"
                "Если б так поэта измучила,\n"
                "он\n"
                "любимую на деньги б и славу выменял,\n"
                "а мне\n"
                "ни один не радостен звон,\n"
                "кроме звона твоего любимого имени.\n"
                "И в пролет не брошусь,\n"
                "и не выпью яда,\n"
                "и курок не смогу над виском нажать.\n"
                "Надо мною,\n"
                "кроме твоего взгляда,\n"
                "не властно лезвие ни одного ножа.\n"
                "Завтра забудешь,\n"
                "что тебя короновал,\n"
                "что душу цветущую любовью выжег,\n"
                "и суетных дней взметенный карнавал\n"
                "растреплет страницы моих книжек…\n"
                "Слов моих сухие листья ли\n"
                "заставят остановиться,\n"
                "жадно дыша?\n"
                "\n"
                "Дай хоть\n"
                "последней нежностью выстелить\n"
                "твой уходящий шаг.";

    ASSERT_TRUE(test_naive(s1));
    ASSERT_TRUE(test_dfa(s1));
}