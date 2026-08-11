#ifndef OPTIONAL_HPP
#define OPTIONAL_HPP

#include <exception>
#include <utility>
namespace mystd {

    struct nullopt_t {} ;

    inline constexpr nullopt_t nullopt{};

    class bad_optional_access : public std::exception {

    };

    template<typename T>
    class optional { 
    private:
        union {
            T _value;
            nullopt_t nullopt;
        };
        bool has_value_flag;

    public:
        
        class iterator {
            private:
                optional<T>* opt_ptr;
            public:

                explicit iterator(optional<T>* opt_ptr): opt_ptr(opt_ptr) {}
                T& operator *(){
                    return opt_ptr->_value;
                }

                iterator& operator++() {
                    opt_ptr = nullptr;
                    return *this;
                }
                
                bool operator!=(const iterator& other) const {
                    return opt_ptr != other.opt_ptr;
                }
                
                bool operator==(const iterator& other) const {
                    return opt_ptr == other.opt_ptr;
                }
        };

        class const_iterator {
            private:
                const optional<T>* opt_ptr;
            public:
                explicit const_iterator(const optional<T>* opt_ptr): opt_ptr(opt_ptr) {}

                const T& operator *() const {
                    return opt_ptr->_value;
                }

                const_iterator& operator++() {
                    opt_ptr = nullptr;
                    return *this;
                }
                
                bool operator!=(const const_iterator& other) const {
                    return opt_ptr != other.opt_ptr;
                }
                
                bool operator==(const const_iterator& other) const {
                    return opt_ptr == other.opt_ptr;
                }
        };
    public:
        using value_type = T;
    
        ~optional() noexcept {
            free();
        }

        constexpr optional() noexcept : has_value_flag(false) {}
        
        constexpr optional(nullopt_t) noexcept : optional() {}

        constexpr optional(const optional& other) : has_value_flag(other.has_value_flag) {
            if (has_value_flag) {
                new(&_value) T(other._value);
            }
        }

        constexpr optional(optional&& other) noexcept : has_value_flag(other.has_value_flag) {
            if (has_value_flag) {
                new(&_value) T(std::move(other._value));
            }
        }
        
        template<typename U>
        constexpr optional(const optional<U>& other) : has_value_flag(other.has_value()) {
            if (has_value_flag) {
                new(&_value) T(other.value());
            }
        }

        template<typename U>
        constexpr optional(optional<U>&& other) noexcept : has_value_flag(other.has_value()) {
            if (has_value_flag) {
                new(&_value) T(std::move(other.value()));
            }
        }

        template<typename... Args>
        constexpr explicit optional(std::in_place_t, Args&&... args) : has_value_flag(true) {
            new(&_value) T(std::forward<Args>(args)...);
        }

        template<typename U, typename... Args>
        constexpr explicit optional(std::in_place_t, std::initializer_list<U> ilist,
                                    Args&&... args) : has_value_flag(true) {
            new(&_value) T(ilist, std::forward<Args>(args)...);
        }
    
        optional& operator=(nullopt_t) noexcept {
            free();
            has_value_flag = false;
            return *this;
        }
        
        constexpr optional& operator=(const optional& other) {
            if (this != &other) {
                free();
                has_value_flag = other.has_value_flag;
                if (has_value_flag) {
                    new(&_value) T(other._value);
                }
            }
            return *this;
        }
        
        constexpr optional& operator=(optional&& other) noexcept {
            if (this != &other) {
                free();
                has_value_flag = other.has_value_flag;
                if (has_value_flag) {
                    new(&_value) T(std::move(other._value));
                }
            }
            return *this;
        }
        
        template<typename U>
        constexpr optional& operator=(const optional<U>& other) {
            free();
            has_value_flag = other.has_value();
            if (has_value_flag) {
                new(&_value) T(other.value());
            }
            return *this;
        }

        template<typename U>
        constexpr optional& operator=(optional<U>&& other) noexcept {
            free();
            has_value_flag = other.has_value();
            if (has_value_flag) {
                new(&_value) T(std::move(other.value()));
            }
            return *this;
        }
        
        iterator begin() {
            if (has_value_flag)
                return iterator{&this};
            return iterator{nullptr};
        }
        
        iterator end() {
            return iterator{nullptr};
        }

        const_iterator begin() const {
            if (has_value_flag)
                return iterator{&this};
            return iterator{nullptr};
        }
        
        const_iterator end() const {
            return iterator{nullptr};
        }

        constexpr const T* operator->() const noexcept {
            return _value;
        
        }
        constexpr T* operator->() noexcept {
            return _value;
        }
        constexpr const T& operator*() const& noexcept {
            return _value;
        }
        constexpr T& operator*() & noexcept {
            return _value;
        }
        constexpr const T&& operator*() const&& noexcept {
            return _value;
        }
        constexpr T&& operator*() && noexcept {
            return _value;
        }

        constexpr explicit operator bool() const {
            return has_value_flag;
        }
        
        constexpr bool has_value() const {
            return has_value_flag;
        }

        constexpr T& value() & {
            if (!has_value_flag) {
                throw bad_optional_access();
            }
            return _value;
        }

        constexpr const T& value() const& {
            if (!has_value_flag) {
                throw bad_optional_access();
            }
            return _value;
        }

        constexpr T&& value() && {
            if (!has_value_flag) {
                throw bad_optional_access();
            }
            return std::move(_value);
        }

        constexpr const T&& value() const&& {
            if (!has_value_flag) {
                throw bad_optional_access();
            }
            return std::move(_value);
        }

        constexpr T value_or(T& default_value) & {
            return has_value_flag ? _value : default_value;
        }

        constexpr T value_or(T&& default_value) && {
            return has_value_flag ? std::move(_value) : std::move(default_value);
        }

        template<typename F>
        constexpr auto and_then(F&& f) & {
            if (has_value_flag)
                return f(_value);
            return optional(nullopt);
        } 
        
        template<typename F>
        constexpr auto and_then(F&& f) const & {
            if (has_value_flag)
                return f(_value);
            return optional(nullopt);
        }

        template<typename F>
        constexpr auto and_then(F&& f) && {
            if (has_value_flag)
                return f(std::move(_value));
            return optional(nullopt);
        }

        template<typename F>
        constexpr auto and_then(F&& f) const && {
            if (has_value_flag)
                return f(std::move(_value));
            return optional(nullopt);
        }

        template<typename F>
        constexpr auto transform(F&& f) & {
            if (has_value_flag)
                return optional(f(_value));
            return optional(nullopt);
        }

        template<typename F>
        constexpr auto transform(F&& f) const & {
            if (has_value_flag)
                return optional(f(_value));
            return optional(nullopt);
        }

        template<typename F>
        constexpr auto transform(F&& f) && {
            if (has_value_flag)
                return optional(f(std::move(_value)));
            return optional(nullopt);
        }

        template<typename F>
        constexpr auto transform(F&& f) const && {
            if (has_value_flag)
                return optional(f(std::move(_value)));
            return optional(nullopt);
        }

        template<typename F>
        constexpr optional or_else(F&& f) && {
            if (has_value_flag)
                return *this;
            return f();
        }
        
        template<typename F>
        constexpr optional or_else(F&& f) const & {
            if (has_value_flag)
                return *this;
            return f();
        }

        void swap(optional& other) noexcept {
            if (has_value_flag && other.has_value_flag) {
                std::swap(_value, other._value);
            } else if (has_value_flag) {
                other.emplace(std::move(_value));
                free();
            } else if (other.has_value_flag) {
                emplace(std::move(other._value));
                other.free();
            }
        }

        void reset() noexcept {
            free();
        }

        template<typename... Args>
        void emplace(Args&&... args) {
            free();
            new(&_value) T(std::forward<Args>(args)...);
            has_value_flag = true;
        }

        
        
    private:
        void free() {
            if (has_value_flag) {
                _value.~T();
                has_value_flag = false;
            }
        }

        friend struct std::hash<optional<T>>;
    };


    template<typename T, typename U>
    void swap(optional<T>& lhs, optional<U>& rhs) noexcept {
        lhs.swap(rhs);
    }

    template<typename T, typename U>
    bool operator==(const optional<T>& lhs, const optional<U>& rhs) {
        if (lhs.has_value() != rhs.has_value()) 
            return false;
        return !lhs.has_value() || lhs.value() == rhs.value();
    }

    template<typename T, typename U>
    bool operator!=(const optional<T>& lhs, const optional<U>& rhs) {
        return !(lhs == rhs);
    }

    template<typename T, typename U>
    bool operator<(const optional<T>& lhs, const optional<U>& rhs) {
        if (!lhs.has_value()) 
            return rhs.has_value();
        
        if (!rhs.has_value()) {
            return false;
        }

        return lhs.value() < rhs.value();
    }

    template<typename T, typename U>
    bool operator>(const optional<T>& lhs, const optional<U>& rhs) {
        return operator<(rhs, lhs);
    }

    template<typename T, typename U>
    bool operator>=(const optional<T>& lhs, const optional<U>& rhs) {
        return !operator<(lhs, rhs);
    }

    
    template<typename T, typename U>
    bool operator<=(const optional<T>& lhs, const optional<U>& rhs) {
        return operator>=(rhs, lhs);
    }

    template< class T >
    constexpr optional<std::decay_t<T>> make_optional( T&& value ) {
        return optional<std::decay_t<T>>(value);
    }
    template< class T, class... Args >
    constexpr optional<T> make_optional( Args&&... args ) {
        return optional<T>(std::in_place, std::forward<Args>(args)...);
    }
    template< class T, class U, class... Args >
    constexpr optional<T> make_optional( std::initializer_list<U> il,
                                              Args&&... args ) {
        return optional<T>(std::in_place, il, std::forward<Args>(args)...);
    }
}

namespace std {

    template<typename T>
    struct hash<mystd::optional<T>> {
        size_t operator()(const mystd::optional<T>& obj) {
            if (obj.has_value_flag)
                return std::hash<T>{}(obj._value);
            return 0;
        }
    };
}


#endif // OPTIONAL_HPP
