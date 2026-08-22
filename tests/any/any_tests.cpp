// This suite is written against the full std::any contract, not against what
// mystd::any currently implements. mystd::any (see src/Any/Any.hpp) only has:
// a default constructor, a value-constructing template constructor, a
// destructor, and a member `any_cast<T>()`. Copy/move construction and
// assignment are explicitly `= delete`d, and there is no has_value(), reset(),
// type(), emplace(), swap(), free any_cast(), make_any(), or bad_any_cast.
//
// That means large parts of this file will not compile until mystd::any grows
// to support them -- that is intentional. This file is the spec to build
// towards, not a report on what already passes.

#include <gtest/gtest.h>
#include <string>
#include <utility>
#include "Any.hpp"

using mystd::any;

namespace {

struct Tracker {
    static inline int constructions = 0;
    static inline int destructions = 0;
    static inline int copies = 0;
    static inline int moves = 0;
    int value = 0;

    Tracker() { ++constructions; }
    explicit Tracker(int v) : value(v) { ++constructions; }
    Tracker(const Tracker& other) : value(other.value) { ++constructions; ++copies; }
    Tracker(Tracker&& other) noexcept : value(other.value) { ++constructions; ++moves; }
    ~Tracker() { ++destructions; }

    static void reset() {
        constructions = destructions = copies = moves = 0;
    }
};

// Large enough to force the heap-allocated (non-SSO) storage path.
struct LargeTracker : Tracker {
    using Tracker::Tracker;
    char padding[64] = {};
};

} // namespace

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

TEST(AnyTest, DefaultConstructedHasNoValue) {
    any a;
    EXPECT_FALSE(a.has_value());
}

TEST(AnyTest, ValueConstructorHoldsValue) {
    any a(42);
    ASSERT_TRUE(a.has_value());
    EXPECT_EQ(mystd::any_cast<int>(a), 42);
}

TEST(AnyTest, ValueConstructorSupportsSmallTrivialType) {
    any a(short{7});
    EXPECT_EQ(mystd::any_cast<short>(a), 7);
}

TEST(AnyTest, ValueConstructorSupportsLargeType) {
    any a(std::string("this is a fairly long string that will not fit inline"));
    EXPECT_EQ(mystd::any_cast<std::string>(a),
              "this is a fairly long string that will not fit inline");
}

TEST(AnyTest, InPlaceConstructorForwardsSingleArg) {
    any a(std::in_place_type<std::string>, "hello");
    EXPECT_EQ(mystd::any_cast<std::string>(a), "hello");
}

TEST(AnyTest, InPlaceConstructorForwardsMultipleArgs) {
    any a(std::in_place_type<std::string>, 3, 'x');
    EXPECT_EQ(mystd::any_cast<std::string>(a), "xxx");
}

TEST(AnyTest, InPlaceConstructorWithInitializerList) {
    any a(std::in_place_type<std::string>, {'h', 'i'});
    EXPECT_EQ(mystd::any_cast<std::string>(a), "hi");
}

TEST(AnyTest, CopyConstructorCopiesValue) {
    any a(42);
    any b(a);
    ASSERT_TRUE(b.has_value());
    EXPECT_EQ(mystd::any_cast<int>(b), 42);
}

TEST(AnyTest, CopyConstructorProducesIndependentCopy) {
    Tracker::reset();
    any a(std::in_place_type<Tracker>, 1);
    any b(a);

    mystd::any_cast<Tracker&>(b).value = 2;

    EXPECT_EQ(mystd::any_cast<Tracker&>(a).value, 1)
        << "Mutating the copy should not affect the original";
}

TEST(AnyTest, MoveConstructorTransfersValue) {
    any a(std::string("movable"));
    any b(std::move(a));

    ASSERT_TRUE(b.has_value());
    EXPECT_EQ(mystd::any_cast<std::string>(b), "movable");
}

TEST(AnyTest, MoveConstructorLeavesSourceEmpty) {
    any a(42);
    any b(std::move(a));

    EXPECT_FALSE(a.has_value());
}

// ---------------------------------------------------------------------------
// Assignment
// ---------------------------------------------------------------------------

TEST(AnyTest, CopyAssignmentCopiesValue) {
    any a(1);
    any b;
    b = a;

    ASSERT_TRUE(b.has_value());
    EXPECT_EQ(mystd::any_cast<int>(b), 1);
}

TEST(AnyTest, CopyAssignmentReplacesExistingValue) {
    any a(std::string("new"));
    any b(1);
    b = a;

    EXPECT_EQ(mystd::any_cast<std::string>(b), "new");
}

TEST(AnyTest, MoveAssignmentTransfersValue) {
    any a(std::string("moved"));
    any b;
    b = std::move(a);

    ASSERT_TRUE(b.has_value());
    EXPECT_EQ(mystd::any_cast<std::string>(b), "moved");
}

TEST(AnyTest, MoveAssignmentLeavesSourceEmpty) {
    any a(1);
    any b;
    b = std::move(a);

    EXPECT_FALSE(a.has_value());
}

TEST(AnyTest, ValueAssignmentSetsValue) {
    any a;
    a = 99;

    ASSERT_TRUE(a.has_value());
    EXPECT_EQ(mystd::any_cast<int>(a), 99);
}

TEST(AnyTest, ValueAssignmentReplacesExistingValue) {
    any a(std::string("old"));
    a = 5;

    EXPECT_EQ(mystd::any_cast<int>(a), 5);
}

// ---------------------------------------------------------------------------
// emplace
// ---------------------------------------------------------------------------

TEST(AnyTest, EmplaceReplacesStoredValue) {
    any a(std::string("old"));
    a.emplace<int>(7);

    EXPECT_EQ(mystd::any_cast<int>(a), 7);
}

TEST(AnyTest, EmplaceWithInitializerList) {
    any a;
    a.emplace<std::string>({'h', 'i'});

    EXPECT_EQ(mystd::any_cast<std::string>(a), "hi");
}

TEST(AnyTest, EmplaceReturnsReferenceToNewValue) {
    any a;
    int& ref = a.emplace<int>(5);

    EXPECT_EQ(ref, 5);
    ref = 6;
    EXPECT_EQ(mystd::any_cast<int>(a), 6);
}

// ---------------------------------------------------------------------------
// reset
// ---------------------------------------------------------------------------

TEST(AnyTest, ResetClearsValue) {
    any a(1);
    a.reset();

    EXPECT_FALSE(a.has_value());
}

TEST(AnyTest, ResetOnEmptyAnyIsNoop) {
    any a;
    a.reset();

    EXPECT_FALSE(a.has_value());
}

TEST(AnyTest, ResetDestroysPreviouslyStoredValue) {
    Tracker::reset();
    any a(std::in_place_type<Tracker>, 1);
    a.reset();

    EXPECT_EQ(Tracker::destructions, 1);
}

// ---------------------------------------------------------------------------
// swap
// ---------------------------------------------------------------------------

TEST(AnyTest, MemberSwapExchangesValues) {
    any a(1);
    any b(std::string("two"));

    a.swap(b);

    EXPECT_EQ(mystd::any_cast<std::string>(a), "two");
    EXPECT_EQ(mystd::any_cast<int>(b), 1);
}

TEST(AnyTest, MemberSwapWithEmptyAny) {
    any a(1);
    any b;

    a.swap(b);

    EXPECT_FALSE(a.has_value());
    ASSERT_TRUE(b.has_value());
    EXPECT_EQ(mystd::any_cast<int>(b), 1);
}

TEST(AnyTest, FreeSwapFunctionExchangesValues) {
    any a(1);
    any b(2);

    swap(a, b);

    EXPECT_EQ(mystd::any_cast<int>(a), 2);
    EXPECT_EQ(mystd::any_cast<int>(b), 1);
}

// ---------------------------------------------------------------------------
// has_value / type
// ---------------------------------------------------------------------------

TEST(AnyTest, HasValueReflectsState) {
    any a;
    EXPECT_FALSE(a.has_value());

    a = 1;
    EXPECT_TRUE(a.has_value());

    a.reset();
    EXPECT_FALSE(a.has_value());
}

TEST(AnyTest, TypeReturnsTypeInfoOfStoredValue) {
    any a(42);
    EXPECT_EQ(a.type(), typeid(int));

    a = std::string("hi");
    EXPECT_EQ(a.type(), typeid(std::string));
}

TEST(AnyTest, TypeReturnsVoidTypeInfoWhenEmpty) {
    any a;
    EXPECT_EQ(a.type(), typeid(void));
}

// ---------------------------------------------------------------------------
// any_cast (free function)
// ---------------------------------------------------------------------------

TEST(AnyTest, AnyCastByValueReturnsStoredValue) {
    any a(42);
    EXPECT_EQ(mystd::any_cast<int>(a), 42);
}

TEST(AnyTest, AnyCastByReferenceAllowsMutation) {
    any a(1);
    mystd::any_cast<int&>(a) = 2;

    EXPECT_EQ(mystd::any_cast<int>(a), 2);
}

TEST(AnyTest, AnyCastOnConstAnyReturnsConstReference) {
    const any a(1);
    const int& ref = mystd::any_cast<const int&>(a);

    EXPECT_EQ(ref, 1);
}

TEST(AnyTest, AnyCastThrowsOnTypeMismatch) {
    any a(1);
    EXPECT_THROW(mystd::any_cast<std::string>(a), mystd::bad_any_cast);
}

TEST(AnyTest, AnyCastThrowsOnEmptyAny) {
    any a;
    EXPECT_THROW(mystd::any_cast<int>(a), mystd::bad_any_cast);
}

TEST(AnyTest, AnyCastPointerOverloadReturnsPointerToValue) {
    any a(42);
    int* p = mystd::any_cast<int>(&a);

    ASSERT_NE(p, nullptr);
    EXPECT_EQ(*p, 42);
}

TEST(AnyTest, AnyCastPointerOverloadReturnsNullptrForWrongType) {
    any a(42);
    std::string* p = mystd::any_cast<std::string>(&a);

    EXPECT_EQ(p, nullptr);
}

TEST(AnyTest, AnyCastPointerOverloadReturnsNullptrForEmptyAny) {
    any a;
    int* p = mystd::any_cast<int>(&a);

    EXPECT_EQ(p, nullptr);
}

TEST(AnyTest, AnyCastOnRvalueMovesValueOut) {
    any a(std::string("movable"));
    std::string s = mystd::any_cast<std::string>(std::move(a));

    EXPECT_EQ(s, "movable");
}

// ---------------------------------------------------------------------------
// make_any
// ---------------------------------------------------------------------------

TEST(AnyTest, MakeAnyConstructsValueInPlace) {
    any a = mystd::make_any<int>(42);
    EXPECT_EQ(mystd::any_cast<int>(a), 42);
}

TEST(AnyTest, MakeAnyForwardsMultipleArgs) {
    any a = mystd::make_any<std::string>(3, 'x');
    EXPECT_EQ(mystd::any_cast<std::string>(a), "xxx");
}

TEST(AnyTest, MakeAnyWithInitializerList) {
    any a = mystd::make_any<std::string>({'h', 'i'});
    EXPECT_EQ(mystd::any_cast<std::string>(a), "hi");
}

// ---------------------------------------------------------------------------
// Lifetime / storage semantics
// ---------------------------------------------------------------------------

TEST(AnyTest, DestructorDestroysHeldObject) {
    Tracker::reset();
    {
        any a(std::in_place_type<Tracker>, 1);
    }
    EXPECT_EQ(Tracker::destructions, 1);
}

TEST(AnyTest, ReassigningDestroysPreviousValue) {
    Tracker::reset();
    any a(std::in_place_type<Tracker>, 1);
    a = 5;

    EXPECT_EQ(Tracker::destructions, 1);
}

TEST(AnyTest, MoveOfLargeTypeTransfersPointerWithoutCopying) {
    Tracker::reset();
    any a(std::in_place_type<LargeTracker>, 1);
    any b(std::move(a));

    EXPECT_EQ(Tracker::copies, 0)
        << "Moving an any holding a heap-allocated object should not copy the object";
    EXPECT_EQ(mystd::any_cast<LargeTracker&>(b).value, 1);
}

TEST(AnyTest, HoldingDifferentTypesAcrossInstancesIsIndependent) {
    any a(1);
    any b(std::string("two"));
    any c(3.0);

    EXPECT_EQ(mystd::any_cast<int>(a), 1);
    EXPECT_EQ(mystd::any_cast<std::string>(b), "two");
    EXPECT_EQ(mystd::any_cast<double>(c), 3.0);
}
