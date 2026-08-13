#include <gtest/gtest.h>
#include "SharedPtr.hpp"
#include "UniquePtr.hpp"
#include "WeakPtr.hpp"

using mystd::shared_ptr;
using mystd::unique_ptr;
using mystd::weak_ptr;

TEST(SharedPtrTest, DefaultConstructedIsFalsy) {
    shared_ptr<int> p;
    EXPECT_FALSE(static_cast<bool>(p));
}

TEST(SharedPtrTest, ConstructedFromRawPointerIsTruthy) {
    shared_ptr<int> p(new int(42));
    EXPECT_TRUE(static_cast<bool>(p));
}

TEST(SharedPtrTest, DereferenceReturnsPointedToValue) {
    shared_ptr<int> p(new int(42));
    EXPECT_EQ(*p, 42);
}

TEST(SharedPtrTest, ArrowOperatorAccessesMembers) {
    struct Point { int x; int y; };
    shared_ptr<Point> p(new Point{3, 4});
    EXPECT_EQ(p->x, 3);
    EXPECT_EQ(p->y, 4);
}

TEST(SharedPtrTest, GetReturnsUnderlyingPointer) {
    int* raw = new int(7);
    shared_ptr<int> p(raw);
    EXPECT_EQ(p.get(), raw);
}

TEST(SharedPtrTest, CopyConstructorSharesSameUnderlyingObject) {
    shared_ptr<int> a(new int(5));
    shared_ptr<int> b(a);

    EXPECT_TRUE(a == b);
    EXPECT_EQ(a.get(), b.get());
    EXPECT_EQ(*b, 5);
}

TEST(SharedPtrTest, MoveConstructorTransfersOwnership) {
    shared_ptr<int> a(new int(9));
    int* raw = a.get();
    shared_ptr<int> b(std::move(a));

    EXPECT_EQ(b.get(), raw);
    EXPECT_EQ(*b, 9);
}

TEST(SharedPtrTest, EqualityComparesUnderlyingPointer) {
    shared_ptr<int> a(new int(1));
    shared_ptr<int> b(new int(1));

    EXPECT_TRUE(a == a);
    EXPECT_FALSE(a == b) << "Distinct allocations should not compare equal";
}

TEST(SharedPtrTest, CopiesKeepObjectAliveUntilLastOneDestroyed) {
    shared_ptr<int> a(new int(123));
    {
        shared_ptr<int> b(a);
        EXPECT_EQ(*b, 123);
    }
    // b went out of scope; a should still be valid.
    EXPECT_EQ(*a, 123);
}

TEST(UniquePtrTest, ConstructedFromRawPointerOwnsIt) {
    unique_ptr<int> p(new int(42));
    EXPECT_EQ(*p, 42);
}

TEST(UniquePtrTest, ArrowOperatorAccessesMembers) {
    struct Point { int x; int y; };
    unique_ptr<Point> p(new Point{1, 2});
    EXPECT_EQ(p->x, 1);
    EXPECT_EQ(p->y, 2);
}

TEST(UniquePtrTest, MoveConstructorTransfersOwnership) {
    unique_ptr<int> a(new int(5));
    unique_ptr<int> b(std::move(a));
    EXPECT_EQ(*b, 5);
}

TEST(UniquePtrTest, MoveAssignmentTransfersOwnership) {
    unique_ptr<int> a(new int(1));
    unique_ptr<int> b(new int(2));
    b = std::move(a);
    EXPECT_EQ(*b, 1);
}

TEST(WeakPtrTest, DefaultConstructedIsExpired) {
    weak_ptr<int> w;
    EXPECT_FALSE(static_cast<bool>(w));
}

TEST(WeakPtrTest, ConstructedFromSharedPtrIsNotExpired) {
    shared_ptr<int> s(new int(42));
    weak_ptr<int> w(s);
    EXPECT_TRUE(static_cast<bool>(w));
}

TEST(WeakPtrTest, DereferenceReturnsSameValueAsSharedPtr) {
    shared_ptr<int> s(new int(42));
    weak_ptr<int> w(s);
    EXPECT_EQ(*w, 42);
}

TEST(WeakPtrTest, CopyConstructorFromWeakPtrSharesState) {
    shared_ptr<int> s(new int(7));
    weak_ptr<int> w1(s);
    weak_ptr<int> w2(w1);

    EXPECT_TRUE(w1 == w2);
    EXPECT_EQ(*w2, 7);
}
