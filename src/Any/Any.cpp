#include "Any.hpp"
namespace mystd {

any::any(const any& other) {
    if (!other.empty) {
        other.manager(any_op::clone, const_cast<any*>(&other), this);
        manager = other.manager;
        stored_type = other.stored_type;
        empty = false;
    }
}

any& any::operator=(const any& other) {
    any{other}.swap(*this);
    return *this;
}

any::any(any&& other) noexcept {
    if (!other.empty) {
        other.manager(any_op::move, &other, this);
        manager = other.manager;
        stored_type = other.stored_type;
        empty = false;

        other.empty = true;
        other.manager = nullptr;
        other.stored_type = &typeid(void);
    }
}

any& any::operator=(any&& other) noexcept {
    any{std::move(other)}.swap(*this);
    return *this;
}

any::~any() {
    reset();
}

void any::reset() {
    if (!empty)
        manager(any_op::destroy, this, nullptr);
    empty = true;
    manager = nullptr;
    stored_type = &typeid(void);
}

void any::swap(any& other) noexcept {
    std::swap(ptr, other.ptr);
    std::swap(empty, other.empty);
    std::swap(stored_type, other.stored_type);
    std::swap(manager, other.manager);
}

bool any::has_value() const {
    return !empty;
}

const std::type_info& any::type() const {
    return *stored_type;
}

void swap(any& lhs, any& rhs) noexcept {
    lhs.swap(rhs);
}

}
