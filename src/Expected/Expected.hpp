#ifndef EXPECTED_HPP
#define EXPECTED_HPP

#include <initializer_list>
#include <type_traits>
#include <utility>
#include <functional>

namespace mystd {

struct unexpect_t { explicit unexpect_t() = default; };

inline constexpr unexpect_t unexpect{};

template <typename E>
class unexpected {
private:
    E unex;
public:

    constexpr unexpected( const unexpected& ) = default;
    constexpr unexpected( unexpected&& ) = default;
    template< class Err = E >
        requires (!std::is_same_v<std::remove_cvref_t<Err>, unexpected>)
    constexpr explicit unexpected( Err&& e ): unex(std::forward<Err>(e)) {}
    template< class... Args >
    constexpr explicit unexpected( std::in_place_t, Args&&... args ): unex(std::forward<Args>(args) ...) {}
    template< class U, class... Args >
    constexpr explicit unexpected( std::in_place_t,
                                   std::initializer_list<U> il, Args&&... args ): unex(il, std::forward<Args>(args) ...) {}

    constexpr const E& error() const& noexcept {return unex;}
    constexpr E& error() & noexcept {return unex;}
    constexpr const E&& error() const&& noexcept {return unex;}
    constexpr E&& error() && noexcept {return unex;}


    constexpr void swap( unexpected& other ) noexcept(std::is_nothrow_swappable_v<E>) {
        std::swap(unex, other.unex);
    }
    
    template< typename E1, typename E2 >
    friend constexpr bool operator==( unexpected<E1>& x, unexpected<E2>& y );
};

template< typename E, typename E2 >
constexpr bool operator==( unexpected<E>& x, unexpected<E2>& y ) {
    return x.unex == y.unex;
}

template< class E >
unexpected( E ) -> unexpected<E>;


template <typename T, typename E>
class expected {
private:

    union {
        T val;
        E unex;
    };

    bool hasValue = true;

public:

    expected(): val() {}
    expected(const T& expected): val(expected) {}

    expected( const expected& other);
    expected( expected&& other);

    template< class... Args >
    constexpr explicit expected( std::in_place_t, Args&&... args ): val(std::forward<Args>(args)...) {}

    template< class U, class... Args >
    constexpr explicit expected( std::in_place_t, std::initializer_list<U> il, Args&&... args ): val(il, std::forward<Args>(args)...) {}

    constexpr expected( const unexpected<E>& e): unex(e.error()), hasValue(false) {}
    constexpr explicit expected( unexpected<E>&& e ): unex(std::move(e.error())), hasValue(false) {}

    template< class... Args >
    constexpr explicit expected( unexpect_t, Args&&... args ): unex(std::forward<Args>(args)...), hasValue(false) {}

    template< class U, class... Args >
    constexpr explicit expected( unexpect_t, std::initializer_list<U> il, Args&&... args ): unex(il, std::forward<Args>(args)...), hasValue(false) {}

    expected& operator=( const expected& other );
    expected& operator=( expected&& other );

    expected& operator=( const unexpected<E>& other );
    expected& operator=( unexpected<E>&& other );

    constexpr ~expected();

    constexpr const T* operator->() const noexcept {return &val;}
    constexpr T* operator->() noexcept {return &val;}
    constexpr const T& operator*() const& noexcept {return val;}
    constexpr T& operator*() & noexcept {return val;}
    constexpr const T&& operator*() const&& noexcept {return val;}
    constexpr T&& operator*() && noexcept {return val;}

    constexpr explicit operator bool() const noexcept {return hasValue;}
    constexpr bool has_value() const noexcept {return hasValue;}

    constexpr T& value() & {return val;}
    constexpr const T& value() const& {return val;}
    constexpr T&& value() && {return val;}
    constexpr const T&& value() const&& {return val;}

    template< class U = std::remove_cv_t<T> > 
    constexpr T value_or( U&& default_value ) const&;
    template< class U = std::remove_cv_t<T> > 
    constexpr T value_or( U&& default_value ) &&;

    
    constexpr const E& error() const& noexcept {return unex;}
    constexpr E& error() & noexcept {return unex;}
    constexpr const E&& error() const&& noexcept {return unex;}
    constexpr E&& error() && noexcept {return unex;}

    
    template< class G = E > 
    constexpr E error_or( G&& default_value ) const&;

    template< class G = E > 
    constexpr E error_or( G&& default_value ) &&;

    template< class... Args >
    constexpr T& emplace( Args&&... args ) noexcept;
    template< class U, class... Args >
    constexpr T& emplace( std::initializer_list<U> il, Args&&... args ) noexcept;

    constexpr void swap( expected& other );

    template< class F >
    constexpr auto and_then( F&& f ) &;

    template< class F >
    constexpr auto and_then( F&& f ) const&;

    template< class F >
    constexpr auto and_then( F&& f ) &&;

    template< class F >
    constexpr auto and_then( F&& f ) const&&;

    template< class F >
    constexpr auto transform( F&& f ) &;

    template< class F >
    constexpr auto transform( F&& f ) const&;

    template< class F >
    constexpr auto transform( F&& f ) &&;

    template< class F >
    constexpr auto transform( F&& f ) const&&;


    template< class F >
    constexpr auto or_else( F&& f ) &;
    template< class F >
    constexpr auto or_else( F&& f ) const&;
    template< class F >
    constexpr auto or_else( F&& f ) &&;
    template< class F >
    constexpr auto or_else( F&& f ) const&&;

    template< class F >
    constexpr auto transform_error( F&& f ) &;
    template< class F >
    constexpr auto transform_error( F&& f ) const&;
    template< class F >
    constexpr auto transform_error( F&& f ) &&;
    template< class F >
    constexpr auto transform_error( F&& f ) const&&;

};

template <typename T, typename E>
constexpr expected<T, E>::~expected() {
    if (hasValue)
        val.~T();
    else
        unex.~E();
}

template <typename T, typename E>
expected<T, E>::expected(const expected<T, E>& other) {
    if (other.hasValue) {
        hasValue = true;
        val = other.val;
    }
    else {
        hasValue = false;
        unex = other.unex;
    }
}
template <typename T, typename E>
expected<T, E>::expected(expected<T, E>&& other) {
    if (other.hasValue) {
        hasValue = true;
        val = std::move(other.val);
    }
    else {
        hasValue = false;
        unex = std::move(other.unex);
    }
}

template <typename T, typename E>
expected<T, E>& expected<T, E>::operator=(const expected<T, E>& other ) {
    if (this != &other) {
        expected{other}.swap(*this);
    }
    return *this;
}

template <typename T, typename E>
expected<T, E>& expected<T, E>::operator=(expected<T, E>&& other ) {
    if (this != &other) {
        expected{std::move(other)}.swap(*this);
    }
    return *this;
}

template <typename T, typename E>
expected<T, E>& expected<T, E>::operator=(const unexpected<E>& other ) {
    if (this != &other) {
        expected{other}.swap(*this);
    }
    return *this;
}

template <typename T, typename E>
expected<T, E>& expected<T, E>::operator=(unexpected<E>&& other ) {

    expected{std::move(other)}.swap(*this);
    return *this;
}



template <typename T, typename E>
template< class U> 
constexpr T expected<T, E>::value_or( U&& default_value ) const& {
    return hasValue ? val : default_value;
}


template <typename T, typename E>
template< class U> 
constexpr T expected<T, E>::value_or( U&& default_value ) && {
    return hasValue ? val : default_value;
}

template <typename T, typename E>
template< class G> 
constexpr E expected<T, E>::error_or( G&& default_value ) const& {
    return hasValue ? default_value : unex;
}

template <typename T, typename E>
template< class G > 
constexpr E expected<T, E>::error_or( G&& default_value ) && {
    return hasValue ? default_value : unex;
}

template <typename T, typename E>
template< class... Args >
constexpr T& expected<T, E>::emplace( Args&&... args ) noexcept {
    val = T(std::forward<Args>(args)...);
    hasValue = true;
    return val;
}
template <typename T, typename E>
template< class U, class... Args >
constexpr T& expected<T, E>::emplace( std::initializer_list<U> il, Args&&... args ) noexcept {
    val = T(il, std::forward<Args>(args)...);
    hasValue = true;
    return val;
}

template <typename T, typename E>
constexpr void expected<T, E>::swap( expected<T, E>& other ) {
    if (hasValue && other.hasValue) {
        std::swap(val, other.val);
    }
    else if (!hasValue && !other.hasValue) {
        std::swap(unex, other.unex);
    }
    else if (hasValue) {
        E tmp(std::move(other.unex));
        other.unex.~E();
        
        try {
            ::new (&other.val) T(std::move(this->val));

        } catch (...) {
            ::new (&other.unex) E(std::move(tmp));
            throw;
        }
        this->val.~T();
        ::new (&this->unex) E(std::move(tmp));

        std::swap(hasValue, other.hasValue);
    }
    else {
        E tmp(std::move(unex));
        unex.~E();
        
        try {
            ::new (&this->val) T(std::move(other.val));

        } catch (...) {
            ::new (&this->unex) E(std::move(tmp));
            throw;
        }
        other.val.~T();
        ::new (&other.unex) E(std::move(tmp));

        std::swap(hasValue, other.hasValue);
    }
}

template <typename T, typename E>
template< class F >
constexpr auto expected<T, E>::and_then( F&& f ) & {
    using R = std::invoke_result_t<F, T&>;
    if (!hasValue)
        return R(unexpect, unex);
    return std::invoke(std::forward<F>(f), val);
}

template <typename T, typename E>
template< class F >
constexpr auto expected<T, E>::and_then( F&& f ) const& {
    using R = std::invoke_result_t<F, const T&>;
    if (!hasValue)
        return R(unexpect, unex);
    return std::invoke(std::forward<F>(f), val);

}

template <typename T, typename E>
template< class F >
constexpr auto expected<T, E>::and_then( F&& f ) && {
    using R = std::invoke_result_t<F, T&&>;
    if (!hasValue)
        return R(unexpect, std::move(unex));
    return std::invoke(std::forward<F>(f), std::move(val));
}

template <typename T, typename E>
template< class F >
constexpr auto expected<T, E>::and_then( F&& f ) const&& {
    using R = std::invoke_result_t<F, const T&&>;
    if (!hasValue)
        return R(unexpect, std::move(unex));
    return std::invoke(std::forward<F>(f), std::move(val));
}

template <typename T, typename E>
template< class F >
constexpr auto expected<T, E>::transform( F&& f ) & {
    using R = expected<std::invoke_result_t<F, T&>, E>;
    if (!hasValue)
        return R(unexpect, unex);
    return R(std::invoke(std::forward<F>(f), val));
}

template <typename T, typename E>
template< class F >
constexpr auto expected<T, E>::transform( F&& f ) const& {
    using R = expected<std::invoke_result_t<F, const T&>, E>;
    if (!hasValue)
        return R(unexpect, unex);
    return R(std::invoke(std::forward<F>(f), val));

}

template <typename T, typename E>
template< class F >
constexpr auto expected<T, E>::transform( F&& f ) && {
    using R = expected<std::invoke_result_t<F, T&&>, E>;
    if (!hasValue)
        return R(unexpect, std::move(unex));
    return R(std::invoke(std::forward<F>(f), std::move(val)));
}

template <typename T, typename E>
template< class F >
constexpr auto expected<T, E>::transform( F&& f ) const&& {
    using R = expected<std::invoke_result_t<F, const T&&>, E>;
    if (!hasValue)
        return R(unexpect, std::move(unex));
    return R(std::invoke(std::forward<F>(f), std::move(val)));
}

template <typename T, typename E>
template< class F >
constexpr auto expected<T, E>::or_else( F&& f ) & {
    using R = std::invoke_result_t<F, E&>;
    if (hasValue)
        return R(val);
    return std::invoke(std::forward<F>(f), unex);
}

template <typename T, typename E>
template< class F >
constexpr auto expected<T, E>::or_else( F&& f ) const& {
    using R = std::invoke_result_t<F, const E&>;
    if (hasValue)
        return R(val);
    return std::invoke(std::forward<F>(f), unex);

}

template <typename T, typename E>
template< class F >
constexpr auto expected<T, E>::or_else( F&& f ) && {
    using R = std::invoke_result_t<F, E&&>;
    if (hasValue)
        return R(std::move(val));
    return std::invoke(std::forward<F>(f), std::move(unex));
}

template <typename T, typename E>
template< class F >
constexpr auto expected<T, E>::or_else( F&& f ) const&& {
    using R = std::invoke_result_t<F, const E&&>;
    if (hasValue)
        return R(std::move(val));
    return std::invoke(std::forward<F>(f), std::move(unex));
}

template <typename T, typename E>
template< class F >
constexpr auto expected<T, E>::transform_error( F&& f ) & {
    using R = expected<T, std::invoke_result_t<F, E&>>;
    if (hasValue)
        return R(val);
    return R(unexpect, std::invoke(std::forward<F>(f), unex));
}

template <typename T, typename E>
template< class F >
constexpr auto expected<T, E>::transform_error( F&& f ) const& {
    using R = expected<T, std::invoke_result_t<F, const E&>>;
    if (hasValue)
        return R(val);
    return R(unexpect, std::invoke(std::forward<F>(f), unex));

}

template <typename T, typename E>
template< class F >
constexpr auto expected<T, E>::transform_error( F&& f ) && {
    using R = expected<T, std::invoke_result_t<F, E&&>>;
    if (hasValue)
        return R(std::move(val));
    return R(unexpect, std::invoke(std::forward<F>(f), std::move(unex)));
}

template <typename T, typename E>
template< class F >
constexpr auto expected<T, E>::transform_error( F&& f ) const&& {
    using R = expected<T, std::invoke_result_t<F, const E&&>>;
    if (hasValue)
        return R(std::move(val));
    return R(unexpect, std::invoke(std::forward<F>(f), std::move(unex)));
}


} //namespace

#endif //EXPECTED_HPP
