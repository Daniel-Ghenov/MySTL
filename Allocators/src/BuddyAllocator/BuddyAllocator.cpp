#include "BuddyAllocator.h"

#include <cassert>
#include <utility>
#include <bit>
#include <new>
#include <algorithm>

const size_t BuddyAllocator::MIN_SIZE = std::max(size_t{32}, std::bit_ceil(sizeof(BuddyAllocator::Node)));

BuddyAllocator::BuddyAllocator(size_t capacity) {
    this->capacity = getNextPowerOfTwo(capacity);
    this->reset();
}

BuddyAllocator::~BuddyAllocator() {
    free();
}

BuddyAllocator::BuddyAllocator(BuddyAllocator&& other) {
    moveFrom(std::move(other));
}

BuddyAllocator& BuddyAllocator::operator=(BuddyAllocator&& other) {
    if (this != &other) {
        free();
        moveFrom(std::move(other));
    }
    return *this;
}

void* BuddyAllocator::allocate(size_t size) {
    size_t sizeRounded = getNextPowerOfTwo(size);
    int bucket = getBucketIndex(sizeRounded);
    while (bucket >= 0) {
        if (!freeLists[bucket]) {
            bucket--;
            continue;
        }

        Node* free = freeLists[bucket];
        while (sizeRounded < free->size) {
            // split
            freeAddresses[bucket][getBlockIndex(free)] = false;
            if (free->nextFree)
                free->nextFree->prevFree = nullptr;
            freeLists[bucket] = free->nextFree;
            size_t splitSize = free->size / 2;
            void* rightAddress = static_cast<std::byte*>(free->memory) + splitSize;
            Node* rightChild = new (rightAddress) Node(rightAddress, nullptr, nullptr, splitSize);
            Node* leftChild = new (free->memory) Node(free->memory, rightChild, nullptr, splitSize);
            ++bucket;
            freeLists[bucket] = leftChild;
            freeAddresses[bucket][getBlockIndex(rightChild)] = true;
            free = leftChild;
        }

        freeAddresses[bucket][getBlockIndex(free)] = false;
        if (free->nextFree)
            free->nextFree->prevFree = nullptr;
        freeLists[bucket] = free->nextFree;
        return free->memory;
    }
    throw std::bad_alloc();
}

bool BuddyAllocator::deallocate(void* mem, size_t size) noexcept {
    if (mem < memory || mem >= static_cast<std::byte*>(memory) + capacity)
        return false;

    if (isFree(mem, size))
        return false;

    size_t sizeRounded = getNextPowerOfTwo(size);
    int bucket = getBucketIndex(sizeRounded);
    Node toFree = {mem, nullptr, nullptr, sizeRounded};

    void* buddyAddress = getBuddyAddress(toFree.memory, toFree.size);
    Node* buddy = static_cast<Node*>(buddyAddress);
    while (isFree(buddyAddress, toFree.size)) {
        // merge
        freeAddresses[bucket][getBlockIndex(buddyAddress, toFree.size)] = false;

        if (buddy->prevFree)
            buddy->prevFree->nextFree = buddy->nextFree;
        else
            freeLists[bucket] = buddy->nextFree;

        if (buddy->nextFree)
            buddy->nextFree->prevFree = buddy->prevFree;

        sizeRounded *= 2;
        --bucket;
        void* newMem = std::min(toFree.memory, buddyAddress);
        toFree = {newMem, nullptr, nullptr, sizeRounded};
        buddyAddress = getBuddyAddress(newMem, sizeRounded);
        buddy = static_cast<Node*>(buddyAddress);
    }

    freeAddresses[bucket][getBlockIndex(toFree.memory, toFree.size)] = true;
    Node* free = new (toFree.memory) Node(toFree.memory, freeLists[bucket], nullptr, sizeRounded);
    if (freeLists[bucket])
        freeLists[bucket]->prevFree = free;
    freeLists[bucket] = free;

    return true;
}

void BuddyAllocator::reset() {
    memory = operator new(capacity);
    size_t blockSizesCount = std::countr_zero(capacity / MIN_SIZE) + 1;
    freeLists = std::vector<Node*>(blockSizesCount, nullptr);
    freeLists[0] = new (memory) Node({memory, nullptr, nullptr, capacity});
    freeAddresses = std::vector<std::vector<bool>>(blockSizesCount + 1);
    freeAddresses[0] = {true};
    for (size_t i{1uz}; i < blockSizesCount + 1; ++i) {
        size_t blocksCount = size_t{1} << i;
        freeAddresses[i] = std::vector(blocksCount, false);
    }
}

void BuddyAllocator::free() {
    operator delete(memory);
}

void BuddyAllocator::moveFrom(BuddyAllocator&& other) {
    this->memory = other.memory;
    this->capacity = other.capacity;
    other.memory = nullptr;
    this->freeLists = std::move(other.freeLists);
    this->freeAddresses = std::move(other.freeAddresses);
}

size_t BuddyAllocator::getNextPowerOfTwo(size_t size) {
    if (size <= 1) return 1;
    return std::max(std::bit_ceil(size), MIN_SIZE);
}

size_t BuddyAllocator::getBlockIndex(Node* node) const {
    return getBlockIndex(node->memory, node->size);
}

int BuddyAllocator::getBucketIndex(size_t size) const {
    return freeLists.size() - 1 - std::countr_zero(size / MIN_SIZE);
}

size_t BuddyAllocator::getBlockIndex(void* mem, size_t size) const {
    size_t offset = static_cast<std::byte*>(mem) - static_cast<std::byte*>(memory);
    size_t bucketSize = size;
    return offset / bucketSize;
}

bool BuddyAllocator::isFree(void* mem, size_t size) const {
    size_t bucketSize = getNextPowerOfTwo(size);

    int bucketIndex = getBucketIndex(bucketSize);

    return freeAddresses[bucketIndex][getBlockIndex(mem, bucketSize)];
}

void* BuddyAllocator::getBuddyAddress(void* mem, size_t size) const {
    size_t offset = static_cast<std::byte*>(mem) - static_cast<std::byte*>(memory);
    size_t buddyOffset = offset ^ size;
    return static_cast<std::byte*>(memory) + buddyOffset;
}

