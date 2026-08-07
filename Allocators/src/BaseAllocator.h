#ifndef BASEALLOCATOR_H
#define BASEALLOCATOR_H

#include <cstddef>

class BaseAllocator {
public:
    BaseAllocator() = default;
    virtual ~BaseAllocator() = default;
    [[nodiscard]] virtual void* allocate(size_t size) = 0;
    virtual bool deallocate(void* mem, size_t size) noexcept = 0 ;
    virtual void reset() = 0;
};



#endif //ASEALLOCATOR_H
