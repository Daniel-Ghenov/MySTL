#include <gtest/gtest.h>
#include <string>
#include "Optional.hpp"

using mystd::optional;
using mystd::nullopt;

TEST(OptionalTest, DefaultConstructedHasNoValue) {
    optional<int> o;
    EXPECT_FALSE(o.has_value());
    EXPECT_FALSE(static_cast<bool>(o));
}

TEST(OptionalTest, NulloptConstructedHasNoValue) {
    optional<int> o(nullopt);
    EXPECT_FALSE(o.has_value());
}

TEST(OptionalTest, InPlaceConstructorHoldsValue) {
    optional<std::string> o(std::in_place, "hello");
    ASSERT_TRUE(o.has_value());
    EXPECT_EQ(*o, "hello");
}

TEST(OptionalTest, InPlaceConstructorForwardsMultipleArgs) {
    optional<std::string> o(std::in_place, 3, 'x');
    ASSERT_TRUE(o.has_value());
    EXPECT_EQ(*o, "xxx");
}

TEST(OptionalTest, CopyConstructorProducesIndependentEqualCopy) {
    optional<int> original(std::in_place, 42);
    optional<int> copy(original);

    ASSERT_TRUE(copy.has_value());
    EXPECT_EQ(*copy, 42);

    *copy = 99;
    EXPECT_EQ(*original, 42) << "Mutating the copy should not affect the original";
}

TEST(OptionalTest, MoveConstructorTransfersValue) {
    optional<std::string> original(std::in_place, "movable");
    optional<std::string> moved(std::move(original));

    ASSERT_TRUE(moved.has_value());
    EXPECT_EQ(*moved, "movable");
}

TEST(OptionalTest, CopyAssignmentFromNulloptClearsValue) {
    optional<int> o(std::in_place, 1);
    o = nullopt;
    EXPECT_FALSE(o.has_value());
}

TEST(OptionalTest, CopyAssignmentCopiesValue) {
    optional<int> a(std::in_place, 1);
    optional<int> b;
    b = a;

    ASSERT_TRUE(b.has_value());
    EXPECT_EQ(*b, 1);
}

TEST(OptionalTest, DereferenceReturnsStoredValue) {
    optional<int> o(std::in_place, 7);
    EXPECT_EQ(*o, 7);
}

TEST(OptionalTest, ArrowOperatorAccessesMembers) {
    struct Point { int x; int y; };
    optional<Point> o(std::in_place, Point{3, 4});
    EXPECT_EQ(o->x, 3);
    EXPECT_EQ(o->y, 4);
}

TEST(OptionalTest, ValueReturnsStoredValue) {
    optional<int> o(std::in_place, 5);
    EXPECT_EQ(o.value(), 5);
}

TEST(OptionalTest, ValueThrowsBadOptionalAccessWhenEmpty) {
    optional<int> o;
    EXPECT_THROW(o.value(), mystd::bad_optional_access);
}

TEST(OptionalTest, EmplaceReplacesStoredValue) {
    optional<int> o(std::in_place, 1);
    o.emplace(99);
    ASSERT_TRUE(o.has_value());
    EXPECT_EQ(*o, 99);
}

TEST(OptionalTest, EmplaceOnEmptyOptionalSetsValue) {
    optional<int> o;
    o.emplace(42);
    ASSERT_TRUE(o.has_value());
    EXPECT_EQ(*o, 42);
}

TEST(OptionalTest, ResetClearsValue) {
    optional<int> o(std::in_place, 1);
    o.reset();
    EXPECT_FALSE(o.has_value());
}

TEST(OptionalTest, SwapExchangesTwoEngagedOptionals) {
    optional<int> a(std::in_place, 1);
    optional<int> b(std::in_place, 2);
    a.swap(b);

    EXPECT_EQ(*a, 2);
    EXPECT_EQ(*b, 1);
}

TEST(OptionalTest, SwapBetweenEngagedAndEmptyMovesValueAcross) {
    optional<int> a(std::in_place, 1);
    optional<int> b;
    a.swap(b);

    EXPECT_FALSE(a.has_value());
    ASSERT_TRUE(b.has_value());
    EXPECT_EQ(*b, 1);
}

TEST(OptionalTest, EqualityComparesUnderlyingValues) {
    optional<int> a(std::in_place, 1);
    optional<int> b(std::in_place, 1);
    optional<int> c(std::in_place, 2);
    optional<int> empty;

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
    EXPECT_FALSE(a == empty);
}

TEST(OptionalTest, TwoEmptyOptionalsCompareEqual) {
    optional<int> a;
    optional<int> b;
    EXPECT_TRUE(a == b);
}

TEST(OptionalTest, LessThanTreatsEmptyAsLessThanEngaged) {
    optional<int> empty;
    optional<int> engaged(std::in_place, 0);

    EXPECT_TRUE(empty < engaged);
    EXPECT_FALSE(engaged < empty);
}

TEST(OptionalTest, LessThanComparesValuesWhenBothEngaged) {
    optional<int> a(std::in_place, 1);
    optional<int> b(std::in_place, 2);

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b > a);
}

TEST(OptionalTest, MakeOptionalWithExplicitTypeConstructsInPlace) {
    auto o = mystd::make_optional<std::string>("hello");
    ASSERT_TRUE(o.has_value());
    EXPECT_EQ(*o, "hello");
}

TEST(OptionalTest, RangeForOverEngagedOptionalVisitsOnce) {
    optional<int> o(std::in_place, 42);
    int visits = 0;
    for (auto& v : o) {
        EXPECT_EQ(v, 42);
        ++visits;
    }
    EXPECT_EQ(visits, 1);
}

TEST(OptionalTest, RangeForOverEmptyOptionalVisitsNever) {
    optional<int> o;
    int visits = 0;
    for (auto& v : o) {
        (void)v;
        ++visits;
    }
    EXPECT_EQ(visits, 0);
}
