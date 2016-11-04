#ifndef ATLASCORE_H
#define ATLASCORE_H

#include "Base.h"


namespace atlas {

#define INVALID_PACKED_INDEX UINT32_MAX


template <typename T, size_t StorageBits, size_t GenerationBits>
struct HandledStorage
{
    constexpr static uint32_t StorageSize = (1 << StorageBits);

    struct Handle {
        u32 index : StorageBits;
        u32 generation : GenerationBits;
    };

    HandledStorage() {
        memset(_storage, 0x0, StorageSize*sizeof(T));
        memset(_generations, 0x0, StorageSize*sizeof(u32));
        for (u32 i = 0; i < StorageSize; ++i) {
            _freeList[i] = i + 1;
        }
        _freeHead = 0;
    }

    bool isValid(Handle handle) {
        if (_generations[handle.index] != handle.generation || handle.generation == 0)
            return false;
        return true;
    }

    Handle add(T &item)
    {
        ASSERT(_freeHead < StorageSize && "Assert: No more handles available");
        u32 index = _freeHead;
        _storage[index] = item;
        Handle handle = {index, _generations[index]};
        _freeHead = _freeList[_freeHead];
        return handle;
    }


    void remove(Handle handle)
    {
        if (!isValid(handle))
            return;

        // swap the released item with the last one to preserve packing
        _generations[handle.index]++;
        _freeList[handle.index] = _freeHead;
        _freeHead = handle.index;
    }

    T* get(Handle handle) {
      return isValid(handle) ? &(_storage[handle.index]) : 0x0;
    }


    T _storage[StorageSize];
    u32 _generations[StorageSize];
    u32 _freeList[StorageSize];
    u32 _freeHead;
};


template <typename T, size_t StorageBits, size_t GenerationBits>
struct HandledPackedStorage
{
    constexpr static uint32_t StorageSize = (1 << StorageBits);

    struct Handle {
        u32 index : StorageBits;
        u32 generation : GenerationBits;
    };

    struct Index {
        u32 index : StorageBits;
        u32 generation : GenerationBits;
    };

    struct PackedStorage {
        u32 count;
        T   array[StorageSize];
    };

    HandledPackedStorage()
    {
//        //probably not necessary
//        memset(&(_storage.array), 0x0, StorageSize*sizeof(PackedStorage::array));

        // initialize each cell to point to the next for free list
        // when an index is not used to point to an object we use it
        // to point to the next item in the free list
        // first item in the free list is given by _freeHead
        // last by StorageSize - 1 (when they are equal the storage is full
        // and the free list is empty
        for (u32 i = 0; i < StorageSize; ++i) {
            _indices[i].index = i + 1;
            _indices[i].generation = 1;
        }
        _freeHead = 0;

        // count holds the number of packed stored elements
        // to remove an object with this solution we use the standard trick of swapping it
        // with the last item in the array. Then we update the index so that it points to
        // the new location of the swapped object.
        _storage.count = 0;
    }

    bool isValid(Handle handle)
    {
        if (_indices[handle.index].generation != handle.generation || handle.generation == 0)
            return false;
        return true;
    }

    Handle add(T &item)
    {
        ASSERT(_freeHead != StorageSize && "Assert: No more handles available");
        u32 index = _freeHead;
        _freeHead = _indices[index].index;
        _indices[index].index = _storage.count;
        _storage.array[_storage.count] = item;
        _storage.count++;
        Handle handle = {index, _indices[index].generation};
        return handle;
    }

    void remove(Handle handle)
    {
        if (!isValid(handle))
            return;

        // swap the released item with the last one to preserve packing
        _storage.array[_indices[handle.index].index] = _storage.array[_storage.count - 1];
        _storage.count--;

        _indices[handle.index].generation++;
        _indices[handle.index].index = _freeHead;
        _freeHead = handle.index;
    }

    T* get(Handle handle)
    {
        // double indirection because of the free list but
        // we gained cache friendly packed object array
        if (isValid(handle)) return &(_storage.array[_indices[handle.index].index]);

        return 0x0;
    }

    u32 getPackedIndex(Handle handle)
    {
        if (isValid(handle)) return _indices[handle.index].index;

        return INVALID_PACKED_INDEX;
    }

    // chache friendly, iterate over it
    const PackedStorage& storage() const
    {
        return _storage;
    }

private:
    u32     _freeHead;
    Index   _indices[StorageSize];
    PackedStorage _storage;
};


} // atlas namespace



#endif // ATLASCORE_H
