#include <gtest/gtest.h>
#include <string>
#include <type_traits>
#include "Pair.hpp"

using mystd::pair;

TEST(PairTest, MembersAreDirectlyAccessible) {
    pair<int, std::string> p{1, "one"};
    EXPECT_EQ(p.first, 1);
    EXPECT_EQ(p.second, "one");
}

TEST(PairTest, MakePairConstructsCorrectPair) {
    auto p = mystd::make_pair(1, std::string("one"));
    EXPECT_EQ(p.first, 1);
    EXPECT_EQ(p.second, "one");
}

TEST(PairTest, MemberSwapExchangesBothMembers) {
    pair<int, int> a{1, 2};
    pair<int, int> b{3, 4};
    a.swap(b);

    EXPECT_EQ(a.first, 3);
    EXPECT_EQ(a.second, 4);
    EXPECT_EQ(b.first, 1);
    EXPECT_EQ(b.second, 2);
}

TEST(PairTest, FreeSwapExchangesBothMembers) {
    pair<int, int> a{1, 2};
    pair<int, int> b{3, 4};
    mystd::swap(a, b);

    EXPECT_EQ(a.first, 3);
    EXPECT_EQ(b.first, 1);
}

TEST(PairTest, ThreeWayComparisonOrdersByFirstThenSecond) {
    pair<int, int> a{1, 2};
    pair<int, int> b{1, 3};
    pair<int, int> c{2, 0};

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b < c);
    EXPECT_TRUE(a < c);
    EXPECT_FALSE(b < a);
}

TEST(PairTest, EqualPairsCompareEqualViaThreeWay) {
    pair<int, int> a{1, 2};
    pair<int, int> b{1, 2};
    EXPECT_TRUE((a <=> b) == 0);
}

TEST(PairTest, GetByIndexReturnsCorrectMember) {
    pair<int, std::string> p{42, "hello"};
    EXPECT_EQ(mystd::get<0>(p), 42);
    EXPECT_EQ(mystd::get<1>(p), "hello");
}

TEST(PairTest, GetByIndexAllowsMutation) {
    pair<int, int> p{1, 2};
    mystd::get<0>(p) = 99;
    EXPECT_EQ(p.first, 99);
}

TEST(PairTest, TupleElementResolvesCorrectTypes) {
    static_assert(std::is_same_v<mystd::tuple_element<0, pair<int, std::string>>::type, int>);
    static_assert(std::is_same_v<mystd::tuple_element<1, pair<int, std::string>>::type, std::string>);
    SUCCEED();
}
