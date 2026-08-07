#include <gtest/gtest.h>
#include <random>
#include <unordered_set>
#include "BoundaryAllocator.h"

class BoundaryAllocatorTest : public ::testing::Test {
private:
    BoundaryAllocator* allocator;
    static const size_t ALLOCATOR_CAPACITY = 1024;
    static const size_t MEMORY_BLOCK_SIZE = sizeof(BoundaryAllocator::MemoryBlock);


protected:
    void SetUp() override {
        allocator = new BoundaryAllocator(ALLOCATOR_CAPACITY);
    }

    void TearDown() override {
        delete allocator;
    }


    void checkInvariants() const {
        checkLinkedList();
        checkFreeList();
        checkEverythingMerged();
        checkFreeListContainsAllFreeBlocks();
    }

    void checkLinkedList() const {
        BoundaryAllocator::MemoryBlock* iter = static_cast<BoundaryAllocator::MemoryBlock*>(allocator->memory);
        if (!iter)
            return;

        ASSERT_EQ(iter->prev, nullptr) << "Head of block list has a non-nullptr previous free";

        BoundaryAllocator::MemoryBlock* fastIter = iter;

        while (true) {
            if (!iter->next || !fastIter->next || !fastIter->next->next) {
                break;
            }

            iter = iter->next;
            fastIter = fastIter->next->next;

            ASSERT_NE(iter, fastIter) << "Found cycle inside of block list";
        }

        iter = static_cast<BoundaryAllocator::MemoryBlock*>(allocator->memory);

        while (iter) {
            if (iter->prev)
                ASSERT_EQ(iter->prev->next, iter) << "Found block list inconsistency";
            if (!iter->next)
                break;

            ASSERT_EQ(iter->next->prev, iter) << "Found block list inconsistency";
            void* iterNextMem = iter->next;
            void* nextMem = reinterpret_cast<std::byte*>(iter) + iter->size + sizeof(BoundaryAllocator::MemoryBlock);
            ASSERT_EQ(nextMem, iterNextMem) << "Found inconsistency in block chain addresses";
            iter = iter->next;
        }
    }

    void checkFreeList() const {
        BoundaryAllocator::MemoryBlock* iter = allocator->firstFree;
        if (!iter)
            return;

        ASSERT_EQ(iter->prevFree, nullptr) << "Head of free list has a non-nullptr previous free";

        BoundaryAllocator::MemoryBlock* fastIter = iter;
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

        iter = allocator->firstFree;

        while (iter) {
            ASSERT_EQ(iter->isFree, true) << "Found non-free block inside of the free list";

            if (iter->prevFree)
                ASSERT_EQ(iter->prevFree->nextFree, iter) << "Found free list inconsistency";
            if (iter->nextFree)
                ASSERT_EQ(iter->nextFree->prevFree, iter) << "Found free list inconsistency";
            iter = iter->nextFree;
        }
    }

    void checkEverythingMerged() const {
        BoundaryAllocator::MemoryBlock* iter = allocator->firstFree;
        while (iter) {
            if (iter->next)
                ASSERT_FALSE(iter->next->isFree) << "Found non-merged blocks";
            if (iter->prev)
                ASSERT_FALSE(iter->prev->isFree) << "Found non-merged blocks";
            iter = iter->nextFree;
        }
    }

    void checkFreeListContainsAllFreeBlocks() const {
        std::unordered_set<BoundaryAllocator::MemoryBlock*> freeListFree;

        BoundaryAllocator::MemoryBlock* freeIter = allocator->firstFree;
        while (freeIter) {
            freeListFree.insert(freeIter);
            freeIter = freeIter->nextFree;
        }

        BoundaryAllocator::MemoryBlock* iter = static_cast<BoundaryAllocator::MemoryBlock*>(allocator->memory);
        while (iter) {
            if (iter->isFree) {
                ASSERT_TRUE(freeListFree.contains(iter)) << "Found free block not inside the free list";
            }
            iter = iter->next;
        }
    }


    FRIEND_TEST(BoundaryAllocatorTest, AllocateAndDeallocate);
    FRIEND_TEST(BoundaryAllocatorTest, AllocateAndDeallocateTwice);
    FRIEND_TEST(BoundaryAllocatorTest, AllocateOverCapacity);
    FRIEND_TEST(BoundaryAllocatorTest, DeallocateInvalidPointer);
    FRIEND_TEST(BoundaryAllocatorTest, AllocateWholeArena);
    FRIEND_TEST(BoundaryAllocatorTest, AllocateInOnlyFreeBlock);
    FRIEND_TEST(BoundaryAllocatorTest, AllocateWithoutSplit);
    FRIEND_TEST(BoundaryAllocatorTest, AllocateInSecondFreeBlock);
    FRIEND_TEST(BoundaryAllocatorTest, RandomisedMemoryOperations);
};

TEST_F(BoundaryAllocatorTest, AllocateAndDeallocate) {
    size_t allocSize = 128;
    void* ptr = allocator->allocate(allocSize);
    ASSERT_NE(ptr, nullptr);
    checkInvariants();

    allocator->deallocate(ptr, allocSize);
    checkInvariants();
}

TEST_F(BoundaryAllocatorTest, AllocateAndDeallocateTwice) {
    size_t allocSize = 128;
    void* ptr1 = allocator->allocate(allocSize);
    ASSERT_NE(ptr1, nullptr);
    checkInvariants();
    

    size_t allocSize2 = 256;
    void* ptr2 = allocator->allocate(allocSize2);
    ASSERT_NE(ptr2, nullptr);
    checkInvariants();

    allocator->deallocate(ptr1, allocSize);
    checkInvariants();
    allocator->deallocate(ptr2, allocSize2);
    checkInvariants();
}

TEST_F(BoundaryAllocatorTest, AllocateOverCapacity) {
    size_t allocSize = ALLOCATOR_CAPACITY * 2;
    EXPECT_THROW(allocator->allocate(allocSize), std::bad_alloc);
    checkInvariants();

    EXPECT_THROW(allocator->allocate(ALLOCATOR_CAPACITY), std::bad_alloc);
    checkInvariants();

    EXPECT_THROW(allocator->allocate(ALLOCATOR_CAPACITY - MEMORY_BLOCK_SIZE + 1), std::bad_alloc);
    checkInvariants();
}

TEST_F(BoundaryAllocatorTest, DeallocateInvalidPointer) {
    size_t allocSize = 128;
    void* ptr = allocator->allocate(allocSize);
    checkInvariants();

    EXPECT_FALSE(allocator->deallocate(ptr, allocSize * 2));
    checkInvariants();

    int dummy;
    EXPECT_FALSE(allocator->deallocate(&dummy, sizeof(dummy)));
    checkInvariants();

    EXPECT_TRUE(allocator->deallocate(ptr, allocSize));
    checkInvariants();
}

TEST_F(BoundaryAllocatorTest, AllocateWholeArena) {
    size_t allocSize = ALLOCATOR_CAPACITY / 2 - MEMORY_BLOCK_SIZE;

    void* ptr1 = allocator->allocate(allocSize);
    void* ptr2 = allocator->allocate(allocSize);

    checkInvariants();

    EXPECT_THROW(allocator->allocate(1), std::bad_alloc);
    checkInvariants();

    allocator->deallocate(ptr1, allocSize);
    allocator->deallocate(ptr2, allocSize);
    checkInvariants();

    void* arenaPtr = allocator->allocate(ALLOCATOR_CAPACITY - MEMORY_BLOCK_SIZE);
    checkInvariants();
    allocator->deallocate(arenaPtr, ALLOCATOR_CAPACITY - MEMORY_BLOCK_SIZE);
    checkInvariants();
}

TEST_F(BoundaryAllocatorTest, AllocateInOnlyFreeBlock) {
    size_t allocSize = ALLOCATOR_CAPACITY / 3 - MEMORY_BLOCK_SIZE;
    
    void* ptr1 = allocator->allocate(allocSize);
    void* ptr2 = allocator->allocate(allocSize);
    void* ptr3 = allocator->allocate(allocSize);
    
    void* ptr2Copy = ptr2;
    allocator->deallocate(ptr2, allocSize);
    checkInvariants();

    void* ptr4 = allocator->allocate(allocSize);
    ASSERT_EQ(ptr4, ptr2Copy) << "Allocator did not allocate in the only free block";
    checkInvariants();
    
}

TEST_F(BoundaryAllocatorTest, AllocateWithoutSplit) {
    size_t allocSize1 = 128;
    size_t allocSize2 = 64;

    void* ptr1 = allocator->allocate(allocSize1);
    checkInvariants();

    void* ptr2 = allocator->allocate(allocSize2);
    checkInvariants();

    allocator->deallocate(ptr1, allocSize1);
    checkInvariants();

    void* ptr3 = allocator->allocate(allocSize1);
    ASSERT_EQ(ptr3, ptr1) << "Allocator did not allocate in the free block without splitting";
    checkInvariants();
}

TEST_F(BoundaryAllocatorTest, AllocateInSecondFreeBlock) {
    size_t allocSize = 64;
    size_t allocSize2 = 128;

    void* ptr1 = allocator->allocate(allocSize);
    void* ptr2 = allocator->allocate(allocSize);
    void* ptr3 = allocator->allocate(allocSize2);
    void* ptr4 = allocator->allocate(allocSize);
    checkInvariants();

    allocator->deallocate(ptr1, allocSize);
    checkInvariants();

    allocator->deallocate(ptr3, allocSize);
    checkInvariants();

    void* ptr5 = allocator->allocate(allocSize2);
    ASSERT_EQ(ptr5, ptr3) << "Allocator did not allocate in the second free block";
    checkInvariants();
}


TEST_F(BoundaryAllocatorTest, RandomisedMemoryOperations) {
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
            //check that allocations are disjoint
            
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

    
