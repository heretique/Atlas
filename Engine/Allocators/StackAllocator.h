#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////
///////////////// Tiago Costa, 2013
/////////////////////////////////////////////////////////////////////////////////////////////

#include "Allocators/Allocator.h"

class StackAllocator : public Allocator
{
public:
    StackAllocator(size_t size, void* start);
    ~StackAllocator();

    void* allocate(size_t size, u8 alignment) override;

    void deallocate(void* p) override;

private:
    StackAllocator(const StackAllocator&);  // Prevent copies because it might cause errors
    StackAllocator& operator=(const StackAllocator&);

    struct AllocationHeader
    {
#ifndef NDEBUG
        void* prev_address;
#endif
        u8 adjustment;
    };

#ifndef NDEBUG
    void* _prev_position;
#endif

    void* _current_pos;
};

namespace allocator
{
inline StackAllocator* newStackAllocator(size_t size, Allocator& allocator)
{
    void* p = allocator.allocate(size + sizeof(StackAllocator), __alignof(StackAllocator));
    return new (p) StackAllocator(size, pointer_math::add(p, sizeof(StackAllocator)));
}

inline void deleteStackAllocator(StackAllocator& stackAllocator, Allocator& allocator)
{
    stackAllocator.~StackAllocator();

    allocator.deallocate(&stackAllocator);
}
}  // allocator namespace
