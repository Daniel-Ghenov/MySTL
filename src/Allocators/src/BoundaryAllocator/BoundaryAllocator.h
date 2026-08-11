#ifndef MYSTD_ALLOCATORS_BOUNDARYALLOCATOR_BOUNDARYALLOCATOR_H
#define MYSTD_ALLOCATORS_BOUNDARYALLOCATOR_BOUNDARYALLOCATOR_H

#include "BaseAllocator.h"
#include <cstddef>

namespace mystd {

class BoundaryAllocator : public BaseAllocator {
private:
    static const size_t MIN_SIZE = 32; // bytes
    void* memory = nullptr;
    size_t capacity = -1;

    struct MemoryBlock {
        MemoryBlock* nextFree = nullptr;
        MemoryBlock* prevFree = nullptr;
        MemoryBlock* prev = nullptr;
        MemoryBlock* next = nullptr;
        size_t size;
        bool isFree = true;
    };

    MemoryBlock* firstFree;

public:
    explicit BoundaryAllocator(size_t capacity);
    ~BoundaryAllocator() override;

    BoundaryAllocator(const BoundaryAllocator& other) = delete;
    BoundaryAllocator& operator=(const BoundaryAllocator& other) = delete;

    BoundaryAllocator(BoundaryAllocator&& other);
    BoundaryAllocator& operator=(BoundaryAllocator&& other);

    [[nodiscard]] void* allocate(size_t size) override;
    bool deallocate(void* mem, size_t size) noexcept override;
    void reset() override;

private:
    void free();
    void moveFrom(BoundaryAllocator&& other);
    size_t getNextPowerOfTwo(size_t size);

    friend class BoundaryAllocatorTest;
};

} // namespace mystd

#endif // MYSTD_ALLOCATORS_BOUNDARYALLOCATOR_BOUNDARYALLOCATOR_H
