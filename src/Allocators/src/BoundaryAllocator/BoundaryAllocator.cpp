#include "BoundaryAllocator.h"

#include <cassert>
#include <new>
#include <utility>
#include <bit>

namespace mystd {

BoundaryAllocator::BoundaryAllocator(size_t capacity) : capacity(capacity) {
    memory = ::operator new(capacity);
    reset();
}

BoundaryAllocator::~BoundaryAllocator() {
    free();
}

BoundaryAllocator::BoundaryAllocator(BoundaryAllocator&& other) {
    moveFrom(std::move(other));
}

BoundaryAllocator& BoundaryAllocator::operator=(BoundaryAllocator&& other) {
    if (this != &other) {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

void* BoundaryAllocator::allocate(size_t size) {
    MemoryBlock* iter = firstFree;
    while (iter) {
        if (iter->size < size) {
            iter = iter->nextFree;
            continue;
        }

        if (iter->size < size + MIN_SIZE + sizeof(MemoryBlock)) {
            // no split
            iter->isFree = false;
            if (iter->prevFree == nullptr)
                firstFree = iter->nextFree;
            else
                iter->prevFree->nextFree = iter->nextFree;

            if (iter->nextFree)
                iter->nextFree->prevFree = iter->prevFree;

            iter->prevFree = iter->nextFree = nullptr;
            return iter + 1;
        }

        // split

        MemoryBlock* next = new (reinterpret_cast<std::byte*>(iter + 1) + size) MemoryBlock();
        next->prev = iter;
        next->next = iter->next;

        if (iter->next)
            iter->next->prev = next;

        next->size = iter->size - size - sizeof(MemoryBlock);
        next->prevFree = iter->prevFree;
        next->nextFree = iter->nextFree;
        iter->next = next;
        iter->size = size;
        iter->isFree = false;

        if (iter->prevFree == nullptr)
            firstFree = next;
        else
            iter->prevFree->nextFree = next;

        if (iter->nextFree)
            iter->nextFree->prevFree = next;

        iter->prevFree = iter->nextFree = nullptr;
        return iter + 1;
    }
    throw std::bad_alloc();
}

bool BoundaryAllocator::deallocate(void* mem, size_t size) noexcept {
    if (mem < memory || mem >= static_cast<std::byte*>(memory) + capacity)
        return false;

    MemoryBlock* block = static_cast<MemoryBlock*>(mem) - 1;
    if (block->isFree || block->size < size)
        return false;
    block->isFree = true;
    MemoryBlock* next = block->next;

    bool merged = false;

    if (next && next->isFree) {
        block->next = block->next->next;
        if (block->next)
            block->next->prev = block;
        block->size += sizeof(MemoryBlock) + next->size;
        block->prevFree = next->prevFree;
        block->nextFree = next->nextFree;
        if (block->prevFree)
            block->prevFree->nextFree = block;
        else
            firstFree = block;

        if (block->nextFree)
            block->nextFree->prevFree = block;

        merged = true;
    }

    MemoryBlock* prev = block->prev;
    if (prev && prev->isFree) {
        if (block->prevFree)
            block->prevFree->nextFree = block->nextFree;

        if (block->nextFree)
            block->nextFree->prevFree = block->prevFree;

        prev->next = block->next;
        if (block->next)
            block->next->prev = prev;
        prev->size += sizeof(MemoryBlock) + block->size;
        merged = true;
    }


    if (!merged)
    {
        if (firstFree)
            firstFree->prevFree = block;
        block->nextFree = firstFree;
        firstFree = block;
    }

    return true;
}

void BoundaryAllocator::reset() {
    firstFree = new (memory) MemoryBlock();
    firstFree->size = capacity - sizeof(MemoryBlock);
}

void BoundaryAllocator::free() {
    operator delete(memory);
}

void BoundaryAllocator::moveFrom(BoundaryAllocator&& other) {
    this->memory = other.memory;
    this->capacity = other.capacity;
    other.memory = nullptr;
}

size_t BoundaryAllocator::getNextPowerOfTwo(size_t size) {
    if (size <= 1) return 1;
    return std::bit_ceil(size);
}

} // namespace mystd
