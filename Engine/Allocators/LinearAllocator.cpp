#include "LinearAllocator.h"

LinearAllocator::LinearAllocator(size_t size, void* start) 
	: Allocator(size, start), _current_pos(start)
{
	ASSERT(size > 0);
}

LinearAllocator::~LinearAllocator()
{
	_current_pos   = nullptr;
}

void* LinearAllocator::allocate(size_t size, u8 alignment)
{
	ASSERT(size != 0);

	u8 adjustment =  pointer_math::alignForwardAdjustment(_current_pos, alignment);

	if(_used_memory + adjustment + size > _size)
		return nullptr;

	uptr aligned_address = (uptr)_current_pos + adjustment;

	_current_pos = (void*)(aligned_address + size);

	_used_memory += size + adjustment;
	_num_allocations++;

	return (void*)aligned_address;
}

void LinearAllocator::deallocate(void*)
{
	ASSERT( false && "Use clear() instead" );
}

void LinearAllocator::clear()
{
	_num_allocations     = 0;
	_used_memory         = 0;

	_current_pos   = _start;
}
