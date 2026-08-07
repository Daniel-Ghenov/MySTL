#include <gtest/gtest.h>
#include <random>
#include "BuddyAllocator.h"

class BuddyAllocatorTest : public ::testing::Test {
protected:
    BuddyAllocator* allocator;
    static const size_t ALLOCATOR_CAPACITY = 1024;

    void SetUp() override {
        allocator = new BuddyAllocator(ALLOCATOR_CAPACITY);
    }

    void TearDown() override {
        delete allocator;
    }



    void checkInvariants() const {
        checkFreeList();
        checkFreeListEqualsBitmap();
        checkEverythingMerged();
        checkNoNestedMemory();
    }

    void checkFreeList() const {
        for (size_t i{0uz}; i < allocator->freeLists.size(); ++i) {
            BuddyAllocator::Node* iter = allocator->freeLists[i];
            if (!iter)
                continue;

            ASSERT_EQ(iter->prevFree, nullptr) << "Head of free list has a non-nullptr previous free";

            BuddyAllocator::Node* fastIter = allocator->freeLists[i];
            bool reachedEnd = false;
            while (!reachedEnd) {
                if (!iter->nextFree || !fastIter->nextFree || !fastIter->nextFree->nextFree) {
                    reachedEnd = true;
                    break;
                }

                iter = iter->nextFree;
                fastIter = fastIter->nextFree->nextFree;

                ASSERT_NE(iter, fastIter) << "Found cycle inside of free list";
            }
            iter = allocator->freeLists[i];

            while (iter) {
                if (iter->prevFree)
                    ASSERT_EQ(iter->prevFree->nextFree, iter) << "Found free list inconsistency";
                if (iter->nextFree)
                    ASSERT_EQ(iter->nextFree->prevFree, iter) << "Found free list inconsistency";

                iter = iter->nextFree;
            }
        }
    }

    void checkFreeListEqualsBitmap() const {
        for (size_t i{0uz}; i < allocator->freeLists.size(); ++i) {
            BuddyAllocator::Node* iter = allocator->freeLists[i];
            while (iter) {
                ASSERT_TRUE(allocator->isFree(iter->memory, iter->size)) << "Found free list node without a free address in the bitmap";
                iter = iter->nextFree;
            }
        }

        for (size_t i{0uz}; i < allocator->freeAddresses.size(); ++i) {
            size_t size = allocator->capacity >> i;
            for (size_t j{0uz}; j < allocator->freeAddresses[i].size(); j++) {
                if (!allocator->freeAddresses[i][j])
                    continue;

                void* currMemory = static_cast<std::byte*>(allocator->memory) + j * size;
                BuddyAllocator::Node* node = static_cast<BuddyAllocator::Node*>(currMemory);
                ASSERT_EQ(node->size, size) << "Found free list node with incorrect size";
                ASSERT_TRUE(allocator->isFree(node->memory, node->size)) << "Found free list node without a free address in the bitmap";

                bool foundInList = false;
                BuddyAllocator::Node* iter = allocator->freeLists[i];
                while (iter) {
                    if (iter == node) {
                        foundInList = true;
                        break;
                    }
                    iter = iter->nextFree;
                }
                ASSERT_TRUE(foundInList) << "Bitmap free node not found in free list";
            }
        }
    }

    void checkEverythingMerged() const {
        for (size_t i{0uz}; i < allocator->freeAddresses.size(); ++i) {
            size_t size = allocator->capacity >> i;
            for (size_t j{0uz}; j < allocator->freeAddresses[i].size(); j++) {
                if (!allocator->freeAddresses[i][j])
                    continue;

                void* currMemory = static_cast<std::byte*>(allocator->memory) + j * size;
                void* buddyAddress = allocator->getBuddyAddress(currMemory, size);
                ASSERT_FALSE(allocator->isFree(buddyAddress, size)) << "Found non-merged pair of free blocks";
            }
        }
    }

    void checkNoNestedMemory() const {
        for (size_t i{allocator->freeAddresses.size() - 1}; i > 0; --i) {
            size_t size = allocator->capacity >> i;
            for (size_t j{0uz}; j < allocator->freeAddresses[i].size(); j++) {
                if (!allocator->freeAddresses[i][j])
                    continue;

                void* currMemory = static_cast<std::byte*>(allocator->memory) + j * size;
                size_t currentSize = size;
                int level = i;
                while (level > 0) {
                    void* buddyAddress = allocator->getBuddyAddress(currMemory, currentSize);
                    void* parentAddress = std::min(currMemory, buddyAddress);
                    currentSize *= 2;
                    ASSERT_FALSE(allocator->isFree(parentAddress, currentSize)) << "Found nested memory allocated";
                    currMemory = parentAddress;
                    --level;
                }
            }
        }
    }


    FRIEND_TEST(BuddyAllocatorTest, AllocateAndDeallocate);
    FRIEND_TEST(BuddyAllocatorTest, AllocateAndDeallocateTwice);
    FRIEND_TEST(BuddyAllocatorTest, AllocateOverCapacity);
    FRIEND_TEST(BuddyAllocatorTest, DeallocateInvalidPointer);
    FRIEND_TEST(BuddyAllocatorTest, AllocateWholeArena);
    FRIEND_TEST(BuddyAllocatorTest, RandomisedMemoryOperations);

};

TEST_F(BuddyAllocatorTest, AllocateAndDeallocate) {
    size_t allocSize = 64;
    void* ptr = allocator->allocate(allocSize);
    checkInvariants();

    ASSERT_NE(ptr, nullptr) << "Allocator returned nullptr for a valid allocation";

    allocator->deallocate(ptr, allocSize);
    checkInvariants();
}

TEST_F(BuddyAllocatorTest, AllocateAndDeallocateTwice) {
    size_t allocSize = 64;
    void* ptr1 = allocator->allocate(allocSize);
    checkInvariants();

    void* ptr2 = allocator->allocate(allocSize);
    checkInvariants();

    ASSERT_NE(ptr1, ptr2) << "Allocator returned the same pointer for two different allocations";

    allocator->deallocate(ptr1, allocSize);
    checkInvariants();

    allocator->deallocate(ptr2, allocSize);
    checkInvariants();

    checkEverythingMerged();
}

TEST_F(BuddyAllocatorTest, AllocateOverCapacity) {
    size_t allocSize = ALLOCATOR_CAPACITY * 2;
    EXPECT_THROW(allocator->allocate(allocSize), std::bad_alloc);
    checkInvariants();

    EXPECT_THROW(allocator->allocate(ALLOCATOR_CAPACITY + 1), std::bad_alloc);
    checkInvariants();
}

TEST_F(BuddyAllocatorTest, DeallocateInvalidPointer) {
    size_t allocSize = 64;
    void* ptr = allocator->allocate(allocSize);
    checkInvariants();

    int dummy;
    EXPECT_FALSE(allocator->deallocate(&dummy, sizeof(dummy)));
    checkInvariants();

    EXPECT_TRUE(allocator->deallocate(ptr, allocSize));
    checkInvariants();
}

TEST_F(BuddyAllocatorTest, AllocateWholeArena) {
    size_t allocSize = ALLOCATOR_CAPACITY;
    void* ptr = allocator->allocate(allocSize);
    checkInvariants();

    EXPECT_THROW(allocator->allocate(1), std::bad_alloc);
    checkInvariants();

    allocator->deallocate(ptr, allocSize);
    checkInvariants();
}

TEST_F(BuddyAllocatorTest, RandomisedMemoryOperations) {
    unsigned seed = 0x5EED;
    std::default_random_engine generator(seed);

    struct Allocation {
        void* ptr;
        size_t size;
    };

    std::vector<Allocation> allocations;

    const int numOperations = 50;

    for (int i = 0; i < numOperations; ++i) {
        int operation = std::uniform_int_distribution<>(0, 1)(generator);

        if (operation == 0 || allocations.empty()) {
            size_t allocSize = std::uniform_int_distribution<>(1, ALLOCATOR_CAPACITY / 4)(generator);
            try {
                void* ptr = allocator->allocate(allocSize);
                allocations.push_back({ptr, allocSize});
            } catch (const std::bad_alloc&) {
                checkInvariants();
                continue;
            }

            for(size_t j = 0; j < allocations.size() - 1; ++j) {
                void* start1 = allocations[j].ptr;
                void* end1 = static_cast<std::byte*>(allocations[j].ptr) + allocations[j].size;
                void* start2 = allocations[allocations.size() - 1].ptr;
                void* end2 = static_cast<std::byte*>(allocations[allocations.size() - 1].ptr) + allocations[allocations.size() - 1].size;

                ASSERT_TRUE(end1 <= start2 || end2 <= start1) << "Allocated memory blocks overlap";
            }

        } else {
            int index = std::uniform_int_distribution<>(0, allocations.size() - 1)(generator);
            allocator->deallocate(allocations[index].ptr, allocations[index].size);
            allocations.erase(allocations.begin() + index);
        }
        checkInvariants();
        if (::testing::Test::HasFatalFailure()) return;  
    }
}
