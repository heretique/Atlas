#ifndef LINEARALLOCATOR_H
#define LINEARALLOCATOR_H

/////////////////////////////////////////////////////////////////////////////////////////////
///////////////// Tiago Costa, 2013              
/////////////////////////////////////////////////////////////////////////////////////////////

#include "Allocator.h"

class LinearAllocator : public Allocator
{
public:
	LinearAllocator(size_t size, void* start);
	~LinearAllocator();

	void* allocate(size_t size, u8 alignment) override;
		
	void deallocate(void* p) override;

	void clear();

private:
	LinearAllocator(const LinearAllocator&); //Prevent copies because it might cause errors
	LinearAllocator& operator=(const LinearAllocator&);

	void* _current_pos;
};

namespace allocator
{
	inline LinearAllocator* newLinearAllocator(size_t size, Allocator& allocator)
	{
		void* p = allocator.allocate(size+sizeof(LinearAllocator), __alignof(LinearAllocator));
		return new (p) LinearAllocator(size, pointer_math::add(p, sizeof(LinearAllocator)));
	}

	inline void deleteLinearAllocator(LinearAllocator& linearAllocator, Allocator& allocator)
	{
		linearAllocator.~LinearAllocator();

		allocator.deallocate(&linearAllocator);
	}
}

#endif
