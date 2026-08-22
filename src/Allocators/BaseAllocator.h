#ifndef MYSTD_ALLOCATORS_BASEALLOCATOR_H
#define MYSTD_ALLOCATORS_BASEALLOCATOR_H

#include <cstddef>

namespace mystd {

class BaseAllocator {
public:
    BaseAllocator() = default;
    virtual ~BaseAllocator() = default;
    [[nodiscard]] virtual void* allocate(size_t size) = 0;
    virtual bool deallocate(void* mem, size_t size) noexcept = 0 ;
    virtual void reset() = 0;
};

} // namespace mystd

#endif // MYSTD_ALLOCATORS_BASEALLOCATOR_H
