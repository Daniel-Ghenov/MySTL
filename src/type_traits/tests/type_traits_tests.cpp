#include <gtest/gtest.h>
#include <functional>
#include <type_traits>
#include "type_traits.hpp"

TEST(TypeTraitsTest, UnwrapReferenceLeavesPlainTypeUnchanged) {
    static_assert(std::is_same_v<mystd::unwrap_reference_t<int>, int>);
    static_assert(std::is_same_v<mystd::unwrap_reference<int>::type, int>);
    SUCCEED();
}

TEST(TypeTraitsTest, UnwrapReferenceUnwrapsReferenceWrapper) {
    static_assert(std::is_same_v<mystd::unwrap_reference_t<std::reference_wrapper<int>>, int&>);
    static_assert(std::is_same_v<mystd::unwrap_reference<std::reference_wrapper<double>>::type, double&>);
    SUCCEED();
}
