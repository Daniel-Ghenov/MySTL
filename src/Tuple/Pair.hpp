#ifndef MYSTD_TUPLE_PAIR_HPP
#define MYSTD_TUPLE_PAIR_HPP

#include <compare>
#include <type_traits>

namespace mystd {


    template <typename T1, typename T2>
    struct pair{

        T1 first;
        T2 second;
    

        void swap(pair<T1,T2>& other) noexcept(std::is_nothrow_swappable_v<T1> &&
                                               std::is_nothrow_swappable_v<T2>) {
            std::swap(first, other.first);
            std::swap(second, other.second);
        }
        template<typename U1, typename U2 >
        constexpr std::common_comparison_category_t<std::compare_three_way_result_t<T1, U1>, std::compare_three_way_result_t<T2, U2>>
            operator<=>( const pair<U1, U2>& rhs ) const{
            if (auto cmp = first <=> rhs.first; cmp != 0) {
                return cmp;
            }
            return second <=> rhs.second;
        }
    };

    template<std::size_t I, typename T>
    struct tuple_element;

    template<std::size_t I, typename T1, typename T2>
    struct tuple_element<I, pair<T1, T2>>
    {
        static_assert(I < 2, "Pair has only 2 elements!");
    };

    template<typename T1, typename T2>
    struct tuple_element<0, pair<T1, T2>>
    {
        using type = T1;
    };

    template<typename T1, typename T2>
    struct tuple_element<1, pair<T1, T2>>
    {
        using type = T2;
    };

    template <typename T1, typename T2>
    constexpr pair<std::unwrap_ref_decay_t<T1>, std::unwrap_ref_decay_t<T2>> make_pair(T1&& first, T2&& second) {
        return pair<std::unwrap_ref_decay_t<T1>, std::unwrap_ref_decay_t<T2>>(std::forward<T1>(first), std::forward<T2>(second));
    }

    template<typename T1, typename T2>
    constexpr T1& get(pair<T1,T2>& pair) noexcept {
        return pair.first;
    }

    template<typename T1, typename T2>
    constexpr const T1& get(const pair<T1,T2>& pair) noexcept {
        return pair.first;
    }

    template<typename T1, typename T2>
    constexpr T1&& get(pair<T1,T2>&& pair) noexcept {
        return std::forward(pair.first);
    }

    template<typename T1, typename T2>
    constexpr const T1&& get(const pair<T1,T2>&& pair) noexcept {
        return std::forward(pair.first);
    }


    template<typename T1, typename T2>
    constexpr T1& get(pair<T2,T1>& pair) noexcept {
        return pair.second;
    }

    template<typename T1, typename T2>
    constexpr const T1& get(const pair<T2,T1>& pair) noexcept {
        return pair.second;
    }

    template<typename T1, typename T2>
    constexpr T1&& get(pair<T2,T1>&& pair) noexcept {
        return std::forward(pair.second);
    }

    template<typename T1, typename T2>
    constexpr const T1&& get(const pair<T2,T1>&& pair) noexcept {
        return std::forward(pair.second);
    }
    
    template<size_t I , typename T1, typename T2>
    constexpr tuple_element<I, pair<T1,T2>>::type& get(pair<T1,T2>& pair) noexcept {
        static_assert("Pair has only 2 elements!");
        if constexpr (I == 0) {
            return pair.first;
        } else if constexpr (I == 1) {
            return pair.second;
        }
    } 

    template<size_t I , typename T1, typename T2>
    constexpr const tuple_element<I, pair<T1,T2>>::type& get(const pair<T1,T2>& pair) noexcept {
        static_assert("Pair has only 2 elements!");
        if constexpr (I == 0) {
            return pair.first;
        } else if constexpr (I == 1) {
            return pair.second;
        }
    } 

    template<size_t I , typename T1, typename T2>
    constexpr tuple_element<I, pair<T1,T2>>::type&& get(pair<T1,T2>&& pair) noexcept {
        static_assert("Pair has only 2 elements!");
        if constexpr (I == 0) {
            return pair.first;
        } else if constexpr (I == 1) {
            return pair.second;
        }
    } 

    template<size_t I , typename T1, typename T2>
    constexpr const tuple_element<I, pair<T1,T2>>::type&& get(const pair<T1,T2>&& pair) noexcept {
        static_assert("Pair has only 2 elements!");
        if constexpr (I == 0) {
            return pair.first;
        } else if constexpr (I == 1) {
            return pair.second;
        }
    } 

    template<typename T1, typename T2>
    void swap(pair<T1,T2>& l, pair<T1,T2>& r) {
        l.swap(r);
    }


}

#endif // MYSTD_TUPLE_PAIR_HPP
