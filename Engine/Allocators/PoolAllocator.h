#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////
///////////////// Tiago Costa, 2013
/////////////////////////////////////////////////////////////////////////////////////////////

#include "Allocators/Allocator.h"

class PoolAllocator : public Allocator
{
public:
    PoolAllocator(size_t objectSize, u8 objectAlignment, size_t size, void* mem);
    ~PoolAllocator();

    void* allocate(size_t size, u8 alignment) override;

    void deallocate(void* p) override;

private:
    PoolAllocator(const PoolAllocator&);  // Prevent copies because it might cause errors
    PoolAllocator& operator=(const PoolAllocator&);

    size_t _objectSize;
    u8     _objectAlignment;

    void** _free_list;
};

namespace allocator
{
inline PoolAllocator* newPoolAllocator(size_t objectSize, u8 objectAlignment, size_t size, Allocator& allocator)
{
    void* p = allocator.allocate(size + sizeof(PoolAllocator), __alignof(PoolAllocator));
    return new (p) PoolAllocator(objectSize, objectAlignment, size, pointer_math::add(p, sizeof(PoolAllocator)));
}

inline void deletePoolAllocator(PoolAllocator& poolAllocator, Allocator& allocator)
{
    poolAllocator.~PoolAllocator();

    allocator.deallocate(&poolAllocator);
}
}  // allocator namespace
