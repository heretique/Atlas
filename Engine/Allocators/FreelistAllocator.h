#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////
///////////////// Tiago Costa, 2013
/////////////////////////////////////////////////////////////////////////////////////////////

#include "Allocators/Allocator.h"

class FreeListAllocator : public Allocator
{
public:
    FreeListAllocator(size_t size, void* start);
    ~FreeListAllocator();

    void* allocate(size_t size, u8 alignment) override;

    void deallocate(void* p) override;

private:
    struct AllocationHeader
    {
        size_t size;
        u8     adjustment;
    };

    struct FreeBlock
    {
        size_t     size;
        FreeBlock* next;
    };

    FreeListAllocator(const FreeListAllocator&);  // Prevent copies because it might cause errors
    FreeListAllocator& operator=(const FreeListAllocator&);

    FreeBlock* _free_blocks;
};

namespace allocator
{
inline FreeListAllocator* newFreeListAllocator(size_t size, Allocator& allocator)
{
    void* p = allocator.allocate(size + sizeof(FreeListAllocator), __alignof(FreeListAllocator));
    return new (p) FreeListAllocator(size, pointer_math::add(p, sizeof(FreeListAllocator)));
}

inline void deleteFreeListAllocator(FreeListAllocator& freeListAllocator, Allocator& allocator)
{
    freeListAllocator.~FreeListAllocator();

    allocator.deallocate(&freeListAllocator);
}
}  // allocator namespace
