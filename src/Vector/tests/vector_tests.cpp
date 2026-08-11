#include <gtest/gtest.h>
#include <string>
#include "Vector.hpp"

using mystd::vector;

TEST(VectorTest, DefaultConstructedVectorIsEmpty) {
    vector<int> v;
    EXPECT_EQ(v.size(), 0u);
    EXPECT_TRUE(v.empty());
}

TEST(VectorTest, SizeFillConstructorPopulatesElements) {
    vector<int> v(4, 7);
    ASSERT_EQ(v.size(), 4u);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 7);
    }
}

TEST(VectorTest, InitializerListConstructorPopulatesElementsInOrder) {
    vector<int> v{1, 2, 3, 4, 5};
    ASSERT_EQ(v.size(), 5u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);
}

TEST(VectorTest, PushBackIncreasesSizeAndStoresValue) {
    vector<int> v;
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    ASSERT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);
}

TEST(VectorTest, PushBackRvalueStoresMovedValue) {
    vector<std::string> v;
    std::string s = "hello";
    v.push_back(std::move(s));

    ASSERT_EQ(v.size(), 1u);
    EXPECT_EQ(v[0], "hello");
}

TEST(VectorTest, PopBackRemovesLastElementAndDecreasesSize) {
    vector<int> v{1, 2, 3};
    v.pop_back();

    ASSERT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
}

TEST(VectorTest, FrontAndBackReturnCorrectElements) {
    vector<int> v{5, 6, 7, 8};
    EXPECT_EQ(v.front(), 5);
    EXPECT_EQ(v.back(), 8);
}

TEST(VectorTest, AtThrowsOutOfRangeForInvalidIndex) {
    vector<int> v{1, 2, 3};
    EXPECT_NO_THROW(v.at(2));
    EXPECT_THROW(v.at(3), std::out_of_range);
    EXPECT_THROW(v.at(100), std::out_of_range);
}

TEST(VectorTest, AtReturnsCorrectElement) {
    vector<int> v{1, 2, 3};
    EXPECT_EQ(v.at(0), 1);
    EXPECT_EQ(v.at(2), 3);
}

TEST(VectorTest, ClearEmptiesTheVector) {
    vector<int> v{1, 2, 3};
    v.clear();

    EXPECT_EQ(v.size(), 0u);
    EXPECT_TRUE(v.empty());
}

TEST(VectorTest, ContainsFindsExistingElement) {
    vector<int> v{1, 2, 3, 4};
    EXPECT_TRUE(v.contains(3));
}

TEST(VectorTest, ContainsReturnsFalseForMissingElement) {
    vector<int> v{1, 2, 3, 4};
    EXPECT_FALSE(v.contains(42));
}

TEST(VectorTest, FindReturnsIndexOfExistingElement) {
    vector<int> v{10, 20, 30, 40};
    EXPECT_EQ(v.find(30), 2u);
}

TEST(VectorTest, FindReturnsNposForMissingElement) {
    vector<int> v{10, 20, 30, 40};
    EXPECT_EQ(v.find(99), vector<int>::npos);
}

TEST(VectorTest, SwapExchangesTwoElements) {
    vector<int> v{1, 2, 3, 4};
    v.swap(0, 3);

    EXPECT_EQ(v[0], 4);
    EXPECT_EQ(v[3], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST(VectorTest, ReserveIncreasesCapacityWithoutChangingSize) {
    vector<int> v{1, 2, 3};
    size_t sizeBefore = v.size();
    v.reserve(100);

    EXPECT_EQ(v.size(), sizeBefore);
    EXPECT_GE(v.capacity(), 100u);
}

TEST(VectorTest, CopyConstructorProducesIndependentEqualCopy) {
    vector<int> original{1, 2, 3};
    vector<int> copy(original);

    ASSERT_EQ(copy.size(), original.size());
    for (size_t i = 0; i < original.size(); ++i) {
        EXPECT_EQ(copy[i], original[i]);
    }

    copy[0] = 99;
    EXPECT_EQ(original[0], 1) << "Mutating the copy should not affect the original";
}

TEST(VectorTest, CopyAssignmentProducesIndependentEqualCopy) {
    vector<int> original{1, 2, 3};
    vector<int> copy;
    copy = original;

    ASSERT_EQ(copy.size(), original.size());
    for (size_t i = 0; i < original.size(); ++i) {
        EXPECT_EQ(copy[i], original[i]);
    }

    copy[0] = 99;
    EXPECT_EQ(original[0], 1) << "Mutating the copy should not affect the original";
}

TEST(VectorTest, MoveConstructorTransfersElements) {
    vector<int> original{1, 2, 3};
    vector<int> moved(std::move(original));

    ASSERT_EQ(moved.size(), 3u);
    EXPECT_EQ(moved[0], 1);
    EXPECT_EQ(moved[1], 2);
    EXPECT_EQ(moved[2], 3);
}

TEST(VectorTest, MoveAssignmentTransfersElements) {
    vector<int> original{1, 2, 3};
    vector<int> moved;
    moved = std::move(original);

    ASSERT_EQ(moved.size(), 3u);
    EXPECT_EQ(moved[0], 1);
    EXPECT_EQ(moved[1], 2);
    EXPECT_EQ(moved[2], 3);
}

TEST(VectorTest, IteratorTraversalVisitsElementsInOrder) {
    vector<int> v{1, 2, 3, 4};

    int expected = 1;
    for (auto it = v.begin(); it < v.end(); ++it) {
        EXPECT_EQ(*it, expected);
        ++expected;
    }
    EXPECT_EQ(expected, 5);
}

TEST(VectorTest, ConstIteratorTraversalVisitsElementsInOrder) {
    const vector<int> v{1, 2, 3, 4};

    int expected = 1;
    for (auto it = v.cbegin(); it < v.cend(); ++it) {
        EXPECT_EQ(*it, expected);
        ++expected;
    }
    EXPECT_EQ(expected, 5);
}

TEST(VectorTest, ReverseIteratorTraversalVisitsElementsInReverseOrder) {
    vector<int> v{1, 2, 3, 4};

    int expected = 4;
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        EXPECT_EQ(*it, expected);
        --expected;
    }
    EXPECT_EQ(expected, 0);
}

TEST(VectorTest, ConstReverseIteratorTraversalVisitsElementsInReverseOrder) {
    const vector<int> v{1, 2, 3, 4};

    int expected = 4;
    for (auto it = v.crbegin(); it != v.crend(); ++it) {
        EXPECT_EQ(*it, expected);
        --expected;
    }
    EXPECT_EQ(expected, 0);
}

TEST(VectorTest, InsertAtBeginningShiftsExistingElements) {
    vector<int> v{2, 3, 4};
    v.insert(0, 1);

    ASSERT_EQ(v.size(), 4u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
}

TEST(VectorTest, InsertInMiddleShiftsTailElements) {
    vector<int> v{1, 2, 4, 5};
    v.insert(2, 3);

    ASSERT_EQ(v.size(), 5u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);
}

TEST(VectorTest, EraseRemovesElementAndShiftsTail) {
    vector<int> v{1, 2, 3, 4, 5};
    v.erase(2);

    ASSERT_EQ(v.size(), 4u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 4);
    EXPECT_EQ(v[3], 5);
}

TEST(VectorTest, ResizeGrowFillsNewElementsWithGivenValue) {
    vector<int> v{1, 2, 3};
    v.resize(6, 9);

    ASSERT_EQ(v.size(), 6u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 9);
    EXPECT_EQ(v[4], 9);
    EXPECT_EQ(v[5], 9);
}

TEST(VectorTest, ResizeShrinkTruncatesVector) {
    vector<int> v{1, 2, 3, 4, 5};
    v.resize(2);

    ASSERT_EQ(v.size(), 2u);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
}

TEST(VectorTest, BoolConversionReflectsNonEmptiness) {
    vector<int> empty;
    vector<int> nonEmpty{1};

    EXPECT_FALSE(static_cast<bool>(empty));
    EXPECT_TRUE(static_cast<bool>(nonEmpty));
}
