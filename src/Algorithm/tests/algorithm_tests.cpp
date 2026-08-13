#include <gtest/gtest.h>
#include <algorithm>
#include <vector>
#include "Algorithm.hpp"

TEST(AlgorithmTest, FindReturnsIteratorToMatchingElement) {
    int arr[] = {1, 2, 3, 4, 5};
    auto it = mystd::find(arr, arr + 5, 3);
    ASSERT_NE(it, arr + 5);
    EXPECT_EQ(*it, 3);
}

TEST(AlgorithmTest, FindReturnsLastWhenNotFound) {
    int arr[] = {1, 2, 3};
    auto it = mystd::find(arr, arr + 3, 99);
    EXPECT_EQ(it, arr + 3);
}

TEST(AlgorithmTest, FindIfReturnsFirstMatch) {
    int arr[] = {1, 2, 3, 4, 5};
    auto it = mystd::find_if(arr, arr + 5, [](int x) { return x % 2 == 0; });
    ASSERT_NE(it, arr + 5);
    EXPECT_EQ(*it, 2);
}

TEST(AlgorithmTest, FindIfReturnsLastWhenNoMatch) {
    int arr[] = {1, 3, 5};
    auto it = mystd::find_if(arr, arr + 3, [](int x) { return x % 2 == 0; });
    EXPECT_EQ(it, arr + 3);
}

TEST(AlgorithmTest, FindIfNotReturnsFirstNonMatchingElement) {
    int arr[] = {2, 4, 5, 6};
    auto it = mystd::find_if_not(arr, arr + 4, [](int x) { return x % 2 == 0; });
    ASSERT_NE(it, arr + 4);
    EXPECT_EQ(*it, 5);
}

TEST(AlgorithmTest, FindIfNotReturnsLastWhenAllMatch) {
    int arr[] = {2, 4, 6};
    auto it = mystd::find_if_not(arr, arr + 3, [](int x) { return x % 2 == 0; });
    EXPECT_EQ(it, arr + 3);
}

TEST(AlgorithmTest, CountCountsMatchingElements) {
    int arr[] = {1, 2, 2, 3, 2};
    EXPECT_EQ(mystd::count(arr, arr + 5, 2), 3);
}

TEST(AlgorithmTest, CountReturnsZeroWhenNoMatch) {
    int arr[] = {1, 3, 5};
    EXPECT_EQ(mystd::count(arr, arr + 3, 2), 0);
}

TEST(AlgorithmTest, CountIfCountsMatchingPredicate) {
    int arr[] = {1, 2, 3, 4, 5, 6};
    int evens = mystd::count_if(arr, arr + 6, [](int x) { return x % 2 == 0; });
    EXPECT_EQ(evens, 3);
}

TEST(AlgorithmTest, ForEachVisitsEveryElementExactlyOnce) {
    int arr[] = {1, 2, 3, 4};
    int sum = 0;
    int visits = 0;
    mystd::for_each(arr, arr + 4, [&](int x) { sum += x; ++visits; });
    EXPECT_EQ(sum, 10);
    EXPECT_EQ(visits, 4);
}

TEST(AlgorithmTest, PartitionGroupsMatchingElementsBeforeBoundary) {
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    auto boundary = mystd::partition(v.begin(), v.end(), [](int x) { return x % 2 == 0; });

    for (auto it = v.begin(); it != boundary; ++it) {
        EXPECT_EQ(*it % 2, 0) << "Element before the partition point should satisfy the predicate";
    }
    for (auto it = boundary; it != v.end(); ++it) {
        EXPECT_NE(*it % 2, 0) << "Element at/after the partition point should not satisfy the predicate";
    }
}

TEST(AlgorithmTest, PartitionPreservesAllElements) {
    std::vector<int> v{5, 1, 4, 2, 3};
    mystd::partition(v.begin(), v.end(), [](int x) { return x < 3; });

    std::vector<int> sorted = v;
    std::sort(sorted.begin(), sorted.end());
    EXPECT_EQ(sorted, (std::vector<int>{1, 2, 3, 4, 5}));
}
