#ifndef ANY_HPP
#define ANY_HPP

#include <typeinfo>
#include <type_traits>
#include <utility>

namespace mystd {

template <typename T>
concept soo_type = sizeof(T) <= sizeof(void*) && std::is_nothrow_move_constructible_v<T>;

class bad_any_cast : public std::bad_cast {

};


class any
{
private:
    void* ptr = nullptr;
    enum class any_op { get, clone, move, destroy };
    using manager_fn = void* (*)(any_op, any*, any*);

    bool empty = true;
    const std::type_info* stored_type = &typeid(void);
    manager_fn manager = nullptr;


    template <typename T>
    static void* manage(any_op op, any* self, any* other) {
        if constexpr (soo_type<T>) {
            T* self_obj = reinterpret_cast<T*>(&self->ptr);
            switch (op) {
                case any_op::get:
                    return self_obj;
                case any_op::clone:
                    new (&other->ptr) T(*self_obj);
                    return nullptr;
                case any_op::move:
                    new (&other->ptr) T(std::move(*self_obj));
                    self_obj->~T();
                    return nullptr;
                case any_op::destroy:
                    self_obj->~T();
                    return nullptr;
            }
        } else {
            switch (op) {
                case any_op::get:
                    return self->ptr;
                case any_op::clone:
                    other->ptr = new T(*static_cast<T*>(self->ptr));
                    return nullptr;
                case any_op::move:
                    other->ptr = self->ptr;
                    self->ptr = nullptr;
                    return nullptr;
                case any_op::destroy:
                    delete static_cast<T*>(self->ptr);
                    return nullptr;
            }
        }
        return nullptr;
    }

    template <typename T, typename... Args>
    void construct(Args&&... args) {
        if constexpr (soo_type<T>) {
            new (&ptr) T(std::forward<Args>(args)...);
        } else {
            ptr = new T(std::forward<Args>(args)...);
        }
        manager = &manage<T>;
        stored_type = &typeid(T);
        empty = false;
    }

public:
    any() = default;

    template <typename T, typename... Args>
    explicit any(std::in_place_type_t<T>, Args&&... args) {
        construct<std::decay_t<T>>(std::forward<Args>(args)...);
    }

    template <typename T, typename U, typename... Args>
    explicit any(std::in_place_type_t<T>, std::initializer_list<U> il, Args&&... args) {
        construct<std::decay_t<T>>(il, std::forward<Args>(args)...);
    }

    any(const any& other);
    any& operator=(const any& other);
    any(any&& other) noexcept;
    any& operator=(any&& other) noexcept;

    template <typename T>
    any(const T& t) {
        construct<std::decay_t<T>>(t);
    }

    ~any();

    template <typename T>
    friend T any_cast(const any& a);

    template <typename T>
    friend T any_cast(any& a);

    template <typename T>
    friend T any_cast(any&& a);

    template <typename T>
    friend const std::remove_cvref_t<T>* any_cast(const any* a) noexcept;

    template <typename T>
    friend std::remove_cvref_t<T>* any_cast(any* a) noexcept;

    template <class T, class... Args>
    T& emplace(Args&&... args);

    template <class T, class U, class... Args>
    T& emplace(std::initializer_list<U> il, Args&&... args);

    void reset();
    void swap(any& other) noexcept;

    bool has_value() const;
    const std::type_info& type() const;
};

template <typename T>
T any_cast(const any& a) {
    if (a.empty || a.stored_type != &typeid(T))
        throw bad_any_cast{};
    return *static_cast<std::remove_cvref_t<T>*>(a.manager(any::any_op::get, const_cast<any*>(&a), nullptr));
}

template <typename T>
T any_cast(any& a) {
    if (a.empty || a.stored_type != &typeid(T))
        throw bad_any_cast{};
    return *static_cast<std::remove_cvref_t<T>*>(a.manager(any::any_op::get, &a, nullptr));
}

template <typename T>
T any_cast(any&& a) {
    if (a.empty || a.stored_type != &typeid(T))
        throw bad_any_cast{};
    return std::move(*static_cast<std::remove_cvref_t<T>*>(a.manager(any::any_op::get, &a, nullptr)));
}

template <typename T>
const std::remove_cvref_t<T>* any_cast(const any* a) noexcept {
    if (!a || a->empty || a->stored_type != &typeid(T))
        return nullptr;
    return static_cast<std::remove_cvref_t<T>*>(a->manager(any::any_op::get, const_cast<any*>(a), nullptr));
}

template <typename T>
std::remove_cvref_t<T>* any_cast(any* a) noexcept {
    if (!a || a->empty || a->stored_type != &typeid(T))
        return nullptr;
    return static_cast<std::remove_cvref_t<T>*>(a->manager(any::any_op::get, a, nullptr));
}

template <class T, class... Args>
T& any::emplace(Args&&... args) {
    any{std::in_place_type_t<T>{}, std::forward<Args>(args)...}.swap(*this);
    return any_cast<T&>(*this);
}

template <class T, class U, class... Args>
T& any::emplace(std::initializer_list<U> il, Args&&... args) {
    any{std::in_place_type_t<T>{}, il, std::forward<Args>(args)...}.swap(*this);
    return any_cast<T&>(*this);
}

template <class T, class... Args>
any make_any(Args&&... args) {
    return any{std::in_place_type<T>, std::forward<Args>(args)...};
}

template <class T, class U, class... Args>
any make_any(std::initializer_list<U> il, Args&&... args) {
    return any{std::in_place_type<T>, il, std::forward<Args>(args)...};
}

void swap(any& lhs, any& rhs) noexcept;

} //namespace mystd

#endif //ANY_HPP
