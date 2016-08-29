#ifndef ALLOCATOR_H
#define ALLOCATOR_H

/////////////////////////////////////////////////////////////////////////////////////////////
///////////////// Tiago Costa, 2013              
/////////////////////////////////////////////////////////////////////////////////////////////

#include "Base.h"

#include <new>

namespace pointer_math
{
	void*       alignForward(void* address, u8 alignment);
	const void* alignForward(const void* address, u8 alignment);

	void*       alignBackward(void* address, u8 alignment);
	const void* alignBackward(const void* address, u8 alignment);

	u8          alignForwardAdjustment(const void* address, u8 alignment);
	u8          alignForwardAdjustmentWithHeader(const void* address, u8 alignment, u8 headerSize);

	u8          alignBackwardAdjustment(const void* address, u8 alignment);

	void*       add(void* p, size_t x);
	const void* add(const void* p, size_t x);

	void*       subtract(void* p, size_t x);
	const void* subtract(const void* p, size_t x);
}

class Allocator
{
public:
	Allocator(size_t size, void* start)
	{
		_start          = start;
		_size           = size;

		_used_memory     = 0;
		_num_allocations = 0;
	}

	virtual ~Allocator()
	{
        ASSERT(_num_allocations == 0 && _used_memory == 0);

		_start = nullptr;
		_size   = 0;
	}

	virtual void* allocate(size_t size, u8 alignment = 4) = 0;

	virtual void deallocate(void* p) = 0;

	void* getStart() const
	{
		return _start;
	}

	size_t getSize() const
	{
		return _size;
	}

	size_t getUsedMemory() const
	{
		return _used_memory;
	}

	size_t getNumAllocations() const
	{
		return _num_allocations;
	}

protected:
	void*         _start;
	size_t        _size;

	size_t        _used_memory;
	size_t        _num_allocations;
};

namespace allocator
{
	template <class T> T* allocateNew(Allocator& allocator)
	{
		return new (allocator.allocate(sizeof(T), __alignof(T))) T;
	}

	template <class T> T* allocateNew(Allocator& allocator, const T& t)
	{
		return new (allocator.allocate(sizeof(T), __alignof(T))) T(t);
	}

	template<class T> void deallocateDelete(Allocator& allocator, T& object)
	{
		object.~T();
		allocator.deallocate(&object);
	}

	template<class T> T* allocateArray(Allocator& allocator, size_t length)
	{
        ASSERT(length != 0);

		u8 headerSize = sizeof(size_t)/sizeof(T);

		if(sizeof(size_t)%sizeof(T) > 0)
			headerSize += 1;

		//Allocate extra space to store array length in the bytes before the array
		T* p = ( (T*) allocator.allocate(sizeof(T)*(length + headerSize), __alignof(T)) ) + headerSize;

		*( ((size_t*)p) - 1 ) = length;

		for(size_t i = 0; i < length; i++)
			new (&p[i]) T;

		return p;
	}

	template<class T> void deallocateArray(Allocator& allocator, T* array)
	{
        ASSERT(array != nullptr);

		size_t length = *( ((size_t*)array) - 1 );

		for(size_t i = 0; i < length; i++)
			array[i].~T();

		//Calculate how much extra memory was allocated to store the length before the array
		u8 headerSize = sizeof(size_t)/sizeof(T);

		if(sizeof(size_t)%sizeof(T) > 0)
			headerSize += 1;

		allocator.deallocate(array - headerSize);
	}
};

//Inline functions definitions

namespace pointer_math
{
	inline void* alignForward(void* address, u8 alignment)
	{
		return (void*)( ( reinterpret_cast<uptr>(address) + static_cast<uptr>(alignment-1) ) & static_cast<uptr>(~(alignment-1)) );
	}

	inline const void* alignForward(const void* address, u8 alignment)
	{
		return (void*)( ( reinterpret_cast<uptr>(address) + static_cast<uptr>(alignment-1) ) & static_cast<uptr>(~(alignment-1)) );
	}

	inline void* alignBackward(void* address, u8 alignment)
	{
		return (void*)( reinterpret_cast<uptr>(address) & static_cast<uptr>(~(alignment-1)) );
	}

	inline const void* alignBackward(const void* address, u8 alignment)
	{
		return (void*)( reinterpret_cast<uptr>(address) & static_cast<uptr>(~(alignment-1)) );
	}

	inline u8 alignForwardAdjustment(const void* address, u8 alignment)
	{
		u8 adjustment =  alignment - ( reinterpret_cast<uptr>(address) & static_cast<uptr>(alignment-1) );
	
		if(adjustment == alignment)
			return 0; //already aligned
	
		return adjustment;
	}

	inline u8 alignForwardAdjustmentWithHeader(const void* address, u8 alignment, u8 headerSize)
	{
		u8 adjustment =  alignForwardAdjustment(address, alignment);

		u8 neededSpace = headerSize;

		if(adjustment < neededSpace)
		{
			neededSpace -= adjustment;

			//Increase adjustment to fit header
			adjustment += alignment * (neededSpace / alignment);

			if(neededSpace % alignment > 0)
				adjustment += alignment;
		}
	
		return adjustment;
	}

	inline u8 alignBackwardAdjustment(const void* address, u8 alignment)
	{
		u8 adjustment =  reinterpret_cast<uptr>(address) & static_cast<uptr>(alignment-1);
	
		if(adjustment == alignment)
			return 0; //already aligned
	
		return adjustment;
	}

	inline void* add(void* p, size_t x)
	{
		return (void*)( reinterpret_cast<uptr>(p) + x);
	}

	inline const void* add(const void* p, size_t x)
	{
		return (const void*)( reinterpret_cast<uptr>(p) + x);
	}

	inline void* subtract(void* p, size_t x)
	{
		return (void*)( reinterpret_cast<uptr>(p) - x);
	}

	inline const void* subtract(const void* p, size_t x)
	{
		return (const void*)( reinterpret_cast<uptr>(p) - x);
	}
};

#endif
