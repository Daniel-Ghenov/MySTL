#ifndef BUDDYALLOCATOR_H
#define BUDDYALLOCATOR_H

#include "../BaseAllocator.h"
#include <cstddef>
#include <vector>

class BuddyAllocator : public BaseAllocator {
private:
    void* memory = nullptr;
    size_t capacity = -1;

    struct Node {
        void* memory = nullptr;
        Node* nextFree = nullptr;
        Node* prevFree = nullptr;
        size_t size;
    };

    std::vector<Node*> freeLists;
    std::vector<std::vector<bool>> freeAddresses;

    static const size_t MIN_SIZE; // bytes

public:
    explicit BuddyAllocator(size_t capacity);
    ~BuddyAllocator() override;

    BuddyAllocator(const BuddyAllocator& other) = delete;
    BuddyAllocator& operator=(const BuddyAllocator& other) = delete;

    BuddyAllocator(BuddyAllocator&& other);
    BuddyAllocator& operator=(BuddyAllocator&& other);

    [[nodiscard]] void* allocate(size_t size) override;
    bool deallocate(void* mem, size_t size) noexcept override;
    void reset() override;

private:
    void free();
    void moveFrom(BuddyAllocator&& other);

    static size_t getNextPowerOfTwo(size_t size);
    size_t getBlockIndex(Node* node) const;
    int getBucketIndex(size_t size) const;
    size_t getBlockIndex(void* mem, size_t size) const;
    bool isFree(void* mem, size_t size) const;
    void* getBuddyAddress(void* mem, size_t size) const;

    friend class BuddyAllocatorTest;
};

#endif // BUDDYALLOCATOR_H
