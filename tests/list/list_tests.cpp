#include <gtest/gtest.h>
#include <vector>
#include "List.hpp"

using mystd::list;

TEST(ListTest, PushBackAppendsInOrder) {
    list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    std::vector<int> seen;
    for (auto it = l.begin(); it != l.end(); ++it) {
        seen.push_back(*it);
    }
    ASSERT_EQ(seen.size(), 3u);
    EXPECT_EQ(seen[0], 1);
    EXPECT_EQ(seen[1], 2);
    EXPECT_EQ(seen[2], 3);
}

TEST(ListTest, PushFrontPrependsInOrder) {
    list<int> l;
    l.push_front(1);
    l.push_front(2);
    l.push_front(3);

    std::vector<int> seen;
    for (auto it = l.begin(); it != l.end(); ++it) {
        seen.push_back(*it);
    }
    ASSERT_EQ(seen.size(), 3u);
    EXPECT_EQ(seen[0], 3);
    EXPECT_EQ(seen[1], 2);
    EXPECT_EQ(seen[2], 1);
}

TEST(ListTest, FrontAndBackReturnCorrectElements) {
    list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    EXPECT_EQ(l.front(), 1);
    EXPECT_EQ(l.back(), 3);
}

TEST(ListTest, PopBackRemovesLastElement) {
    list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    l.pop_back();

    EXPECT_EQ(l.back(), 2);

    std::vector<int> seen;
    for (auto it = l.begin(); it != l.end(); ++it) {
        seen.push_back(*it);
    }
    ASSERT_EQ(seen.size(), 2u);
    EXPECT_EQ(seen[0], 1);
    EXPECT_EQ(seen[1], 2);
}

TEST(ListTest, PopFrontRemovesFirstElement) {
    list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    l.pop_front();

    EXPECT_EQ(l.front(), 2);

    std::vector<int> seen;
    for (auto it = l.begin(); it != l.end(); ++it) {
        seen.push_back(*it);
    }
    ASSERT_EQ(seen.size(), 2u);
    EXPECT_EQ(seen[0], 2);
    EXPECT_EQ(seen[1], 3);
}

TEST(ListTest, ContainsFindsExistingElement) {
    list<int> l;
    l.push_back(1);
    l.push_back(2);
    EXPECT_TRUE(l.contains(2));
}

TEST(ListTest, ContainsReturnsFalseForMissingElement) {
    list<int> l;
    l.push_back(1);
    l.push_back(2);
    EXPECT_FALSE(l.contains(99));
}

TEST(ListTest, ClearEmptiesTheList) {
    list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.clear();

    EXPECT_FALSE(l.contains(1));
    EXPECT_EQ(l.begin(), l.end());
}

TEST(ListTest, CopyConstructorProducesIndependentEqualCopy) {
    list<int> original;
    original.push_back(1);
    original.push_back(2);
    original.push_back(3);

    list<int> copy(original);

    std::vector<int> seen;
    for (auto it = copy.begin(); it != copy.end(); ++it) {
        seen.push_back(*it);
    }
    ASSERT_EQ(seen.size(), 3u);
    EXPECT_EQ(seen[0], 1);
    EXPECT_EQ(seen[1], 2);
    EXPECT_EQ(seen[2], 3);

    copy.push_back(4);
    EXPECT_FALSE(original.contains(4)) << "Mutating the copy should not affect the original";
}

TEST(ListTest, CopyAssignmentProducesIndependentEqualCopy) {
    list<int> original;
    original.push_back(1);
    original.push_back(2);

    list<int> copy;
    copy.push_back(99);
    copy = original;

    std::vector<int> seen;
    for (auto it = copy.begin(); it != copy.end(); ++it) {
        seen.push_back(*it);
    }
    ASSERT_EQ(seen.size(), 2u);
    EXPECT_EQ(seen[0], 1);
    EXPECT_EQ(seen[1], 2);

    copy.push_back(4);
    EXPECT_FALSE(original.contains(4)) << "Mutating the copy should not affect the original";
}

TEST(ListTest, ConstIteratorTraversalViaBeginEndVisitsElementsInOrder) {
    list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    const list<int>& cl = l;

    std::vector<int> seen;
    for (auto it = cl.begin(); it != cl.end(); ++it) {
        seen.push_back(*it);
    }
    ASSERT_EQ(seen.size(), 3u);
    EXPECT_EQ(seen[0], 1);
    EXPECT_EQ(seen[1], 2);
    EXPECT_EQ(seen[2], 3);
}

TEST(ListTest, ConstIteratorTraversalViaCbeginCendVisitsElementsInOrder) {
    list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    std::vector<int> seen;
    for (auto it = l.cbegin(); it != l.cend(); ++it) {
        seen.push_back(*it);
    }
    ASSERT_EQ(seen.size(), 3u);
    EXPECT_EQ(seen[0], 1);
    EXPECT_EQ(seen[1], 2);
    EXPECT_EQ(seen[2], 3);
}

TEST(ListTest, ReverseIteratorTraversalVisitsElementsInReverseOrder) {
    list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    std::vector<int> seen;
    for (auto it = l.rbegin(); it != l.rend(); ++it) {
        seen.push_back(*it);
    }
    ASSERT_EQ(seen.size(), 3u);
    EXPECT_EQ(seen[0], 3);
    EXPECT_EQ(seen[1], 2);
    EXPECT_EQ(seen[2], 1);
}

TEST(ListTest, EraseRemovesElementAndReturnsNext) {
    list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);

    auto it = l.begin();
    ++it; // points at 2
    auto next = l.erase(it);
    EXPECT_EQ(*next, 3);

    std::vector<int> seen;
    for (auto i = l.begin(); i != l.end(); ++i) {
        seen.push_back(*i);
    }
    ASSERT_EQ(seen.size(), 2u);
    EXPECT_EQ(seen[0], 1);
    EXPECT_EQ(seen[1], 3);
}

TEST(ListTest, EraseEndIteratorThrows) {
    list<int> l;
    l.push_back(1);
    auto end_it = l.end();
    EXPECT_THROW(l.erase(end_it), std::out_of_range);
}

namespace {

struct NonCopyableNonMovable {
    int a = 0;
    int b = 0;
    NonCopyableNonMovable() = default;
    NonCopyableNonMovable(int a, int b) : a(a), b(b) {}
    NonCopyableNonMovable(const NonCopyableNonMovable&) = delete;
    NonCopyableNonMovable(NonCopyableNonMovable&&) = delete;
};

}  // namespace

TEST(ListTest, EmplaceFrontConstructsInPlaceForNonMovableType) {
    // If emplace_front ever goes back to building a temporary and moving it
    // into the node, this test stops compiling.
    list<NonCopyableNonMovable> l;
    l.emplace_front(1, 2);

    EXPECT_EQ(l.front().a, 1);
    EXPECT_EQ(l.front().b, 2);
}

TEST(ListTest, EmplaceFrontAddsElementAtFront) {
    list<int> l;
    l.push_back(2);
    l.emplace_front(1);

    EXPECT_EQ(l.front(), 1);

    std::vector<int> seen;
    for (auto it = l.begin(); it != l.end(); ++it) {
        seen.push_back(*it);
    }
    ASSERT_EQ(seen.size(), 2u);
    EXPECT_EQ(seen[0], 1);
    EXPECT_EQ(seen[1], 2);
}
