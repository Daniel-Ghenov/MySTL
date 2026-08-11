#include <gtest/gtest.h>
#include <string>
#include "UnorderedMap.hpp"

using mystd::unordered_map;
using mystd::pair;

TEST(UnorderedMapTest, DefaultConstructedMapIsEmpty) {
    unordered_map<int, std::string> m(16);
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0u);
}

TEST(UnorderedMapTest, InsertLvalueAddsElementAndReportsSuccess) {
    unordered_map<int, std::string> m(16);
    pair<int, std::string> p{1, "one"};
    auto result = m.insert(p);

    EXPECT_TRUE(result.second);
    EXPECT_EQ(m.size(), 1u);
    EXPECT_EQ(m.at(1), "one");
}

TEST(UnorderedMapTest, InsertRvalueAddsElementAndReportsSuccess) {
    unordered_map<int, std::string> m(16);
    auto result = m.insert(pair<int, std::string>{2, "two"});

    EXPECT_TRUE(result.second);
    EXPECT_EQ(m.size(), 1u);
    EXPECT_EQ(m.at(2), "two");
}

TEST(UnorderedMapTest, InsertDuplicateKeyReportsFailureAndKeepsOriginalValue) {
    unordered_map<int, std::string> m(16);
    m.insert(pair<int, std::string>{1, "one"});
    auto result = m.insert(pair<int, std::string>{1, "uno"});

    EXPECT_FALSE(result.second);
    EXPECT_EQ(m.size(), 1u);
    EXPECT_EQ(m.at(1), "one");
}

TEST(UnorderedMapTest, InsertMultipleDistinctKeysAllRetrievable) {
    unordered_map<int, std::string> m(16);
    m.insert(pair<int, std::string>{1, "one"});
    m.insert(pair<int, std::string>{2, "two"});
    m.insert(pair<int, std::string>{3, "three"});

    EXPECT_EQ(m.size(), 3u);
    EXPECT_EQ(m.at(1), "one");
    EXPECT_EQ(m.at(2), "two");
    EXPECT_EQ(m.at(3), "three");
}

TEST(UnorderedMapTest, TryEmplaceConstructsValueInPlace) {
    unordered_map<int, std::string> m(16);
    auto result = m.try_emplace(1, "one");

    EXPECT_TRUE(result.second);
    EXPECT_EQ(m.at(1), "one");
}

TEST(UnorderedMapTest, TryEmplaceDuplicateKeyReportsFailure) {
    unordered_map<int, std::string> m(16);
    m.try_emplace(1, "one");
    auto result = m.try_emplace(1, "uno");

    EXPECT_FALSE(result.second);
    EXPECT_EQ(m.at(1), "one");
}

TEST(UnorderedMapTest, AtThrowsForMissingKey) {
    unordered_map<int, std::string> m(16);
    m.insert(pair<int, std::string>{1, "one"});
    EXPECT_THROW(m.at(99), std::out_of_range);
}

TEST(UnorderedMapTest, AtConstOverloadReturnsCorrectValue) {
    unordered_map<int, std::string> m(16);
    m.insert(pair<int, std::string>{1, "one"});
    const auto& cm = m;
    EXPECT_EQ(cm.at(1), "one");
}

TEST(UnorderedMapTest, AtConstOverloadThrowsForMissingKey) {
    unordered_map<int, std::string> m(16);
    m.insert(pair<int, std::string>{1, "one"});
    const auto& cm = m;
    EXPECT_THROW(cm.at(99), std::out_of_range);
}

TEST(UnorderedMapTest, IndexOperatorInsertsDefaultForMissingKey) {
    unordered_map<int, std::string> m(16);
    std::string& val = m[1];
    val = "one";

    EXPECT_EQ(m.at(1), "one");
    EXPECT_EQ(m.size(), 1u);
}

TEST(UnorderedMapTest, IndexOperatorReturnsExistingValueWithoutDuplicating) {
    unordered_map<int, std::string> m(16);
    m.insert(pair<int, std::string>{1, "one"});
    EXPECT_EQ(m[1], "one");
    EXPECT_EQ(m.size(), 1u);
}

TEST(UnorderedMapTest, FindReturnsIteratorToExistingElement) {
    unordered_map<int, std::string> m(16);
    m.insert(pair<int, std::string>{1, "one"});
    auto it = m.find(1);

    ASSERT_NE(it, m.end());
    EXPECT_EQ(it->first, 1);
    EXPECT_EQ(it->second, "one");
}

TEST(UnorderedMapTest, FindReturnsEndForMissingElement) {
    unordered_map<int, std::string> m(16);
    m.insert(pair<int, std::string>{1, "one"});
    EXPECT_EQ(m.find(99), m.end());
}

TEST(UnorderedMapTest, ContainsFindsExistingKey) {
    unordered_map<int, std::string> m(16);
    m.insert(pair<int, std::string>{1, "one"});
    EXPECT_TRUE(m.contains(1));
}

TEST(UnorderedMapTest, ContainsReturnsFalseForMissingKey) {
    unordered_map<int, std::string> m(16);
    m.insert(pair<int, std::string>{1, "one"});
    EXPECT_FALSE(m.contains(99));
}

TEST(UnorderedMapTest, CountReturnsZeroOrOne) {
    unordered_map<int, std::string> m(16);
    m.insert(pair<int, std::string>{1, "one"});
    EXPECT_EQ(m.count(1), 1u);
    EXPECT_EQ(m.count(99), 0u);
}

TEST(UnorderedMapTest, EraseSingleIteratorRemovesElement) {
    unordered_map<int, std::string> m(16);
    m.insert(pair<int, std::string>{1, "one"});
    m.insert(pair<int, std::string>{2, "two"});

    auto it = m.find(1);
    m.erase(it);

    EXPECT_FALSE(m.contains(1));
    EXPECT_TRUE(m.contains(2));
    EXPECT_EQ(m.size(), 1u);
}

TEST(UnorderedMapTest, ClearRemovesAllElements) {
    unordered_map<int, std::string> m(16);
    m.insert(pair<int, std::string>{1, "one"});
    m.insert(pair<int, std::string>{2, "two"});
    m.clear();

    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0u);
    EXPECT_FALSE(m.contains(1));
}

TEST(UnorderedMapTest, IterationVisitsEveryInsertedElement) {
    unordered_map<int, std::string> m(16);
    m.insert(pair<int, std::string>{1, "one"});
    m.insert(pair<int, std::string>{2, "two"});
    m.insert(pair<int, std::string>{3, "three"});

    size_t count = 0;
    std::string concat_keys;
    for (auto it = m.begin(); it != m.end(); ++it) {
        ++count;
    }
    EXPECT_EQ(count, m.size());
}

TEST(UnorderedMapTest, SwapExchangesContents) {
    unordered_map<int, std::string> a(16);
    unordered_map<int, std::string> b(16);
    a.insert(pair<int, std::string>{1, "one"});
    b.insert(pair<int, std::string>{2, "two"});

    a.swap(b);

    EXPECT_TRUE(a.contains(2));
    EXPECT_FALSE(a.contains(1));
    EXPECT_TRUE(b.contains(1));
    EXPECT_FALSE(b.contains(2));
}

TEST(UnorderedMapTest, BeginSkipsLeadingEmptyBuckets) {
    unordered_map<int, std::string> m(32);
    m.insert(pair<int, std::string>{1, "one"});

    auto it = m.begin();
    ASSERT_NE(it, m.end());
    EXPECT_EQ(it->first, 1);
}

TEST(UnorderedMapTest, EraseRangeRemovesEverythingBetweenBeginAndEnd) {
    unordered_map<int, std::string> m(32);
    m.insert(pair<int, std::string>{1, "one"});
    m.insert(pair<int, std::string>{2, "two"});
    m.insert(pair<int, std::string>{3, "three"});

    m.erase(m.begin(), m.end());

    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0u);
}

TEST(UnorderedMapTest, ReverseIterationViaDecrementVisitsEveryElement) {
    unordered_map<int, std::string> m(32);
    m.insert(pair<int, std::string>{1, "one"});
    m.insert(pair<int, std::string>{2, "two"});
    m.insert(pair<int, std::string>{3, "three"});

    auto it = m.end();
    size_t count = 0;
    while (it != m.begin()) {
        --it;
        ++count;
        ASSERT_LT(count, 100u) << "Decrementing did not terminate";
    }
    EXPECT_EQ(count, 3u);
}

TEST(UnorderedMapTest, ConstIteratorFromCbeginCendVisitsEveryElement) {
    unordered_map<int, std::string> m(16);
    m.insert(pair<int, std::string>{1, "one"});
    m.insert(pair<int, std::string>{2, "two"});

    size_t count = 0;
    for (auto it = m.cbegin(); it != m.cend(); ++it) {
        ++count;
    }
    EXPECT_EQ(count, 2u);
}

TEST(UnorderedMapTest, ConstMapIteratesViaBeginEndConstOverloads) {
    unordered_map<int, std::string> m(16);
    m.insert(pair<int, std::string>{1, "one"});
    m.insert(pair<int, std::string>{2, "two"});
    const auto& cm = m;

    size_t count = 0;
    for (auto it = cm.begin(); it != cm.end(); ++it) {
        ++count;
    }
    EXPECT_EQ(count, 2u);
}

TEST(UnorderedMapTest, ConstIteratorIsConstructibleFromIterator) {
    unordered_map<int, std::string> m(16);
    m.insert(pair<int, std::string>{1, "one"});

    unordered_map<int, std::string>::iterator it = m.begin();
    unordered_map<int, std::string>::const_iterator cit = it;

    ASSERT_NE(cit, m.cend());
    EXPECT_EQ(cit->first, 1);
}

TEST(UnorderedMapTest, RehashKeepsIteratorsReturnedByInsertUsable) {
    unordered_map<int, std::string> m(4);

    // Insert enough to guarantee this specific insert crosses the load
    // factor threshold and triggers a rehash inside insert() itself.
    for (int i = 0; i < 20; ++i) {
        auto result = m.insert(pair<int, std::string>{i, std::to_string(i)});
        ASSERT_TRUE(result.second);
        // The iterator returned by insert() must still point at the
        // just-inserted element, even if this call triggered a rehash.
        EXPECT_EQ(result.first->first, i);
        EXPECT_EQ(result.first->second, std::to_string(i));
    }
}

TEST(UnorderedMapTest, OperatorIndexOnLvalueKeyAfterRehashReturnsUsableReference) {
    // Regression test: operator[](const Key&) previously called _insert
    // with the wrong number of arguments (a compile error only surfaced
    // when called with an lvalue key, since an rvalue key prefers the
    // Key&& overload).
    unordered_map<int, std::string> m(4);
    for (int i = 0; i < 20; ++i) {
        int key = i;  // lvalue, binds to operator[](const Key&)
        m[key] = std::to_string(i);
    }

    for (int i = 0; i < 20; ++i) {
        int key = i;
        EXPECT_EQ(m[key], std::to_string(i));
    }
}
