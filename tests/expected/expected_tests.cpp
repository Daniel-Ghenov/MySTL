#include <gtest/gtest.h>
#include <string>
#include "Expected.hpp"

using mystd::expected;
using mystd::unexpected;
using mystd::unexpect;

// ---------------------------------------------------------------------------
// unexpected<E>
// ---------------------------------------------------------------------------

TEST(UnexpectedTest, ConstructedFromValueHoldsError) {
    unexpected<int> u(42);
    EXPECT_EQ(u.error(), 42);
}

TEST(UnexpectedTest, ConstructedFromValueHoldsStringError) {
    unexpected<std::string> u(std::string("failure"));
    EXPECT_EQ(u.error(), "failure");
}

TEST(UnexpectedTest, ConstructedInPlaceForwardsArgs) {
    unexpected<std::string> u(std::in_place, "hello");
    EXPECT_EQ(u.error(), "hello");
}

TEST(UnexpectedTest, ConstructedInPlaceForwardsMultipleArgs) {
    unexpected<std::string> u(std::in_place, 3, 'x');
    EXPECT_EQ(u.error(), "xxx");
}

TEST(UnexpectedTest, ConstructedInPlaceWithInitializerList) {
    unexpected<std::string> u(std::in_place, {'h', 'i'});
    EXPECT_EQ(u.error(), "hi");
}

TEST(UnexpectedTest, CopyConstructorCopiesError) {
    unexpected<int> a(1);
    unexpected<int> b(a);
    EXPECT_EQ(b.error(), 1);
}

TEST(UnexpectedTest, MoveConstructorTransfersError) {
    unexpected<std::string> a(std::string("movable"));
    unexpected<std::string> b(std::move(a));
    EXPECT_EQ(b.error(), "movable");
}

TEST(UnexpectedTest, ErrorAllowsMutation) {
    unexpected<int> u(1);
    u.error() = 2;
    EXPECT_EQ(u.error(), 2);
}

TEST(UnexpectedTest, SwapExchangesErrors) {
    unexpected<int> a(1);
    unexpected<int> b(2);
    a.swap(b);

    EXPECT_EQ(a.error(), 2);
    EXPECT_EQ(b.error(), 1);
}

TEST(UnexpectedTest, EqualityComparesUnderlyingError) {
    unexpected<int> a(1);
    unexpected<int> b(1);
    unexpected<int> c(2);

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
}

// ---------------------------------------------------------------------------
// expected<T, E> -- construction
// ---------------------------------------------------------------------------

TEST(ExpectedTest, DefaultConstructedHasValue) {
    expected<int, std::string> e;
    EXPECT_TRUE(e.has_value());
}

TEST(ExpectedTest, ValueConstructorHoldsValue) {
    expected<int, std::string> e(42);
    ASSERT_TRUE(e.has_value());
    EXPECT_EQ(e.value(), 42);
}

TEST(ExpectedTest, InPlaceConstructorForwardsArgs) {
    expected<std::string, int> e(std::in_place, "hello");
    ASSERT_TRUE(e.has_value());
    EXPECT_EQ(e.value(), "hello");
}

TEST(ExpectedTest, InPlaceConstructorForwardsMultipleArgs) {
    expected<std::string, int> e(std::in_place, 3, 'x');
    ASSERT_TRUE(e.has_value());
    EXPECT_EQ(e.value(), "xxx");
}

TEST(ExpectedTest, InPlaceConstructorWithInitializerList) {
    expected<std::string, int> e(std::in_place, {'h', 'i'});
    ASSERT_TRUE(e.has_value());
    EXPECT_EQ(e.value(), "hi");
}

TEST(ExpectedTest, ConstructedFromUnexpectedHoldsError) {
    unexpected<std::string> u("bad");
    expected<int, std::string> e(u);

    ASSERT_FALSE(e.has_value());
    EXPECT_EQ(e.error(), "bad");
}

TEST(ExpectedTest, ConstructedFromUnexpectedRvalueHoldsError) {
    expected<int, std::string> e(unexpected<std::string>("bad"));

    ASSERT_FALSE(e.has_value());
    EXPECT_EQ(e.error(), "bad");
}

TEST(ExpectedTest, UnexpectConstructorForwardsArgsToError) {
    expected<int, std::string> e(unexpect, "bad");

    ASSERT_FALSE(e.has_value());
    EXPECT_EQ(e.error(), "bad");
}

TEST(ExpectedTest, UnexpectConstructorWithInitializerList) {
    expected<int, std::string> e(unexpect, {'h', 'i'});

    ASSERT_FALSE(e.has_value());
    EXPECT_EQ(e.error(), "hi");
}

TEST(ExpectedTest, CopyConstructorCopiesValue) {
    expected<int, std::string> a(5);
    expected<int, std::string> b(a);

    ASSERT_TRUE(b.has_value());
    EXPECT_EQ(b.value(), 5);
}

TEST(ExpectedTest, CopyConstructorCopiesError) {
    expected<int, std::string> a(unexpected<std::string>("bad"));
    expected<int, std::string> b(a);

    ASSERT_FALSE(b.has_value());
    EXPECT_EQ(b.error(), "bad");
}

TEST(ExpectedTest, MoveConstructorTransfersValue) {
    expected<std::string, int> a(std::string("movable"));
    expected<std::string, int> b(std::move(a));

    ASSERT_TRUE(b.has_value());
    EXPECT_EQ(b.value(), "movable");
}

TEST(ExpectedTest, MoveConstructorTransfersError) {
    expected<int, std::string> a(unexpected<std::string>("bad"));
    expected<int, std::string> b(std::move(a));

    ASSERT_FALSE(b.has_value());
    EXPECT_EQ(b.error(), "bad");
}

// ---------------------------------------------------------------------------
// expected<T, E> -- assignment
// ---------------------------------------------------------------------------

TEST(ExpectedTest, CopyAssignmentCopiesValue) {
    expected<int, std::string> a(1);
    expected<int, std::string> b(unexpected<std::string>("bad"));
    b = a;

    ASSERT_TRUE(b.has_value());
    EXPECT_EQ(b.value(), 1);
}

TEST(ExpectedTest, MoveAssignmentTransfersValue) {
    expected<int, std::string> a(1);
    expected<int, std::string> b(unexpected<std::string>("bad"));
    b = std::move(a);

    ASSERT_TRUE(b.has_value());
    EXPECT_EQ(b.value(), 1);
}

TEST(ExpectedTest, AssignmentFromUnexpectedSetsError) {
    expected<int, std::string> e(1);
    e = unexpected<std::string>("bad");

    ASSERT_FALSE(e.has_value());
    EXPECT_EQ(e.error(), "bad");
}

TEST(ExpectedTest, AssignmentFromUnexpectedRvalueSetsError) {
    expected<int, std::string> e(1);
    e = unexpected<std::string>(std::string("bad"));

    ASSERT_FALSE(e.has_value());
    EXPECT_EQ(e.error(), "bad");
}

// ---------------------------------------------------------------------------
// expected<T, E> -- access
// ---------------------------------------------------------------------------

TEST(ExpectedTest, DereferenceReturnsValue) {
    expected<int, std::string> e(42);
    EXPECT_EQ(*e, 42);
}

TEST(ExpectedTest, ArrowOperatorAccessesMembers) {
    struct Point { int x; int y; };
    expected<Point, std::string> e(std::in_place, Point{3, 4});

    EXPECT_EQ(e->x, 3);
    EXPECT_EQ(e->y, 4);
}

TEST(ExpectedTest, BoolConversionReflectsHasValue) {
    expected<int, std::string> ok(1);
    expected<int, std::string> bad(unexpected<std::string>("bad"));

    EXPECT_TRUE(static_cast<bool>(ok));
    EXPECT_FALSE(static_cast<bool>(bad));
}

TEST(ExpectedTest, HasValueReflectsState) {
    expected<int, std::string> e(1);
    EXPECT_TRUE(e.has_value());

    e = unexpected<std::string>("bad");
    EXPECT_FALSE(e.has_value());
}

TEST(ExpectedTest, ValueReturnsStoredValue) {
    expected<int, std::string> e(7);
    EXPECT_EQ(e.value(), 7);
}

TEST(ExpectedTest, ValueOrReturnsValueWhenPresent) {
    expected<int, std::string> e(7);
    EXPECT_EQ(e.value_or(0), 7);
}

TEST(ExpectedTest, ValueOrReturnsDefaultWhenError) {
    expected<int, std::string> e(unexpected<std::string>("bad"));
    EXPECT_EQ(e.value_or(99), 99);
}

TEST(ExpectedTest, ErrorReturnsStoredError) {
    expected<int, std::string> e(unexpected<std::string>("bad"));
    EXPECT_EQ(e.error(), "bad");
}

TEST(ExpectedTest, ErrorOrReturnsDefaultWhenValuePresent) {
    expected<int, std::string> e(1);
    EXPECT_EQ(e.error_or("fallback"), "fallback");
}

TEST(ExpectedTest, ErrorOrReturnsErrorWhenPresent) {
    expected<int, std::string> e(unexpected<std::string>("bad"));
    EXPECT_EQ(e.error_or("fallback"), "bad");
}

// ---------------------------------------------------------------------------
// expected<T, E> -- emplace / swap
// ---------------------------------------------------------------------------

TEST(ExpectedTest, EmplaceSetsNewValue) {
    expected<std::string, int> e(std::string("old"));
    e.emplace("new");

    ASSERT_TRUE(e.has_value());
    EXPECT_EQ(e.value(), "new");
}

TEST(ExpectedTest, EmplaceOnErroredExpectedSetsValue) {
    expected<std::string, int> e(unexpected<int>(1));
    e.emplace("recovered");

    ASSERT_TRUE(e.has_value());
    EXPECT_EQ(e.value(), "recovered");
}

TEST(ExpectedTest, EmplaceWithInitializerList) {
    expected<std::string, int> e;
    e.emplace({'h', 'i'});

    ASSERT_TRUE(e.has_value());
    EXPECT_EQ(e.value(), "hi");
}

TEST(ExpectedTest, SwapExchangesTwoValues) {
    expected<int, std::string> a(1);
    expected<int, std::string> b(2);
    a.swap(b);

    EXPECT_EQ(a.value(), 2);
    EXPECT_EQ(b.value(), 1);
}

TEST(ExpectedTest, SwapExchangesTwoErrors) {
    expected<int, std::string> a(unexpected<std::string>("a"));
    expected<int, std::string> b(unexpected<std::string>("b"));
    a.swap(b);

    EXPECT_EQ(a.error(), "b");
    EXPECT_EQ(b.error(), "a");
}

TEST(ExpectedTest, SwapBetweenValueAndError) {
    expected<int, std::string> a(1);
    expected<int, std::string> b(unexpected<std::string>("bad"));
    a.swap(b);

    ASSERT_FALSE(a.has_value());
    EXPECT_EQ(a.error(), "bad");
    ASSERT_TRUE(b.has_value());
    EXPECT_EQ(b.value(), 1);
}

// ---------------------------------------------------------------------------
// expected<T, E> -- monadic operations
// ---------------------------------------------------------------------------

TEST(ExpectedTest, AndThenChainsOnValue) {
    expected<int, std::string> e(5);
    auto result = e.and_then([](int v) -> expected<int, std::string> {
        return expected<int, std::string>(v * 2);
    });

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 10);
}

TEST(ExpectedTest, AndThenPropagatesError) {
    expected<int, std::string> e(unexpected<std::string>("fail"));
    auto result = e.and_then([](int v) -> expected<int, std::string> {
        return expected<int, std::string>(v * 2);
    });

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), "fail");
}

TEST(ExpectedTest, TransformMapsValue) {
    expected<int, std::string> e(5);
    auto result = e.transform([](int v) { return v * 2; });

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 10);
}

TEST(ExpectedTest, TransformPropagatesError) {
    expected<int, std::string> e(unexpected<std::string>("fail"));
    auto result = e.transform([](int v) { return v * 2; });

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), "fail");
}

TEST(ExpectedTest, OrElseRecoversFromError) {
    expected<int, std::string> e(unexpected<std::string>("fail"));
    auto result = e.or_else([](const std::string&) -> expected<int, std::string> {
        return expected<int, std::string>(99);
    });

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 99);
}

TEST(ExpectedTest, OrElsePassesThroughValue) {
    expected<int, std::string> e(5);
    auto result = e.or_else([](const std::string&) -> expected<int, std::string> {
        return expected<int, std::string>(99);
    });

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 5);
}

TEST(ExpectedTest, TransformErrorMapsError) {
    expected<int, std::string> e(unexpected<std::string>("fail"));
    auto result = e.transform_error([](const std::string& err) { return err + "!"; });

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), "fail!");
}

TEST(ExpectedTest, TransformErrorPassesThroughValue) {
    expected<int, std::string> e(5);
    auto result = e.transform_error([](const std::string& err) { return err + "!"; });

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 5);
}
