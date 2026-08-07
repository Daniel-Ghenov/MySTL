#ifndef ADAPTER_HPP
#define ADAPTER_HPP

#include "BaseAllocator.h"

template <class T>
class Adapter {
public:
    using value_type = T;

    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
    using is_always_equal = std::false_type;

    explicit Adapter(BaseAllocator* a) noexcept : alloc_(a) {}

    template <class U>
    Adapter(const Adapter<U>& o) noexcept : alloc_(o.alloc_) {}

    T* allocate(size_t n) {
        return static_cast<T*>(alloc_->allocate(n * sizeof(T)));
    }
    void deallocate(T* p, size_t n) noexcept {
        alloc_->deallocate(p, n * sizeof(T));
    }

private:
    BaseAllocator* alloc_;
    template <class U> friend class Adapter;

    template <class V, class W>
    friend bool operator==(const Adapter<V>&, const Adapter<W>&) noexcept;

    template <class V, class W>
    friend bool operator!=(const Adapter<V>&, const Adapter<W>&) noexcept;
};

template <class T, class U>
bool operator==(const Adapter<T>& a, const Adapter<U>& b) noexcept {
    return a.alloc_ == b.alloc_;
}
template <class T, class U>
bool operator!=(const Adapter<T>& a, const Adapter<U>& b) noexcept {
    return !(a == b);
}

#endif // ADAPTER_HPP
