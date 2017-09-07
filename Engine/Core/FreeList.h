#pragma once

#include "Core/Types.h"

#include <cassert>
#include <cstddef>
#include <cstring>

namespace atlas
{
static const u32 kInvalidPackedIndex = 0xFFFFFFFF;

/// @brief Freelist+array based storage using safe handles to identify objects
/// @example usage:
///     typedef FreeList<Animation, 1024, 20, 12> AnimationStorage;
///     typedef AnimationStorage::Handle AnimationHandle;
///     AnimationStorage storage;
///     AnimationHandle handle = storage.add(Animation(...));
///     Animation* anim = storage.get(handle);
///     storage.remove(handle); // handle is now invalid
/// @tparam T value to store
/// @tparam H type of the handle of the stored type
/// @tparam StorageSize maximum size of the values to store
template <typename T, typename H, size_t StorageSize>
struct FreeList
{
    static const size_t kStorageSize = StorageSize;

    FreeList()
    {
        memset(_storage, 0x0, StorageSize * sizeof(T));
        for (u32 i = 0; i < StorageSize; ++i)
        {
            _freeList[i].setIndex(i + 1);
            _freeList[i].setGeneration(1);
        }
        _freeHead = 0;
    }

    bool isValid(const H& handle) const
    {
        assert(handle.index() < StorageSize);
        return (handle.generation() != 0) && (_freeList[handle.index()].generation() == handle.generation());
    }

    H alloc()
    {
        // Assert: No more handles available
        assert(_freeHead < StorageSize);
        u32 index = _freeHead;
        _freeHead = _freeList[index].index();
        H handle(index, _freeList[index].generation());
        return handle;
    }

    void remove(const H& handle)
    {
        if (!isValid(handle))
            return;

        _freeList[handle.index()].setGeneration(_freeList[handle.index()].generation() + 1);

        if (_freeList[handle.index()].generation() == 0)
            _freeList[handle.index()].setGeneration(1);

        _freeList[handle.index()].setIndex(_freeHead);
        _freeHead = handle.index();
    }

    T* get(const H& handle)
    {
        return isValid(handle) ? &(_storage[handle.index()]) : 0x0;
    }

    const T* get(const H& handle) const
    {
        return isValid(handle) ? &(_storage[handle.index()]) : 0x0;
    }

public:
    T   _storage[StorageSize];
    H   _freeList[StorageSize];
    u32 _freeHead;
};

/// Similar with FreeList but it keeps a cache friendly packed array that can be iterated over
template <typename T, typename H, size_t StorageSize>
struct PackedFreeList
{
    static const size_t kStorageSize = StorageSize;

    struct PackedStorage
    {
        u32 count;
        u32 indices[StorageSize];
        T   array[StorageSize];
    };

    PackedFreeList()
    {
        // probably not necessary
        memset(_storage.array, 0x0, StorageSize * sizeof(T));

        // initialize each cell to point to the next for free list
        // when an index is not used to point to an object we use it
        // to point to the next item in the free list
        // first item in the free list is given by _freeHead
        // last by StorageSize - 1 (when they are equal the storage is full
        // and the free list is empty
        for (u32 i = 0; i < StorageSize; ++i)
        {
            _freeList[i].setIndex(i + 1);
            _freeList[i].setGeneration(1);
        }
        _freeHead = 0;

        // count holds the number of packed stored elements
        // to remove an object with this solution we use the standard trick of swapping it
        // with the last item in the array. Then we update the index so that it points to
        // the new location of the swapped object.
        _storage.count = 0;
    }

    bool isValid(const H& handle) const
    {
        assert(handle.index() < StorageSize);
        return (handle.generation() != 0) && (_freeList[handle.index()].generation() == handle.generation());
    }

    H alloc()
    {
        // Assert: No more handles available
        assert(_freeHead != StorageSize);
        u32 freeIndex = _freeHead;
        _freeHead     = _freeList[freeIndex].index();
        // storage location is first element beyond last
        _freeList[freeIndex].setIndex(_storage.count);
        u32 storageLocation = _freeList[freeIndex].index();
        // point back to freelist element
        _storage.indices[storageLocation] = freeIndex;
        _storage.count++;

        H handle(freeIndex, _freeList[freeIndex].generation());
        return handle;
    }

    void remove(const H& handle)
    {
        if (!isValid(handle) || _storage.count == 0)
            return;

        _freeList[handle.index()].setGeneration(_freeList[handle.index()].generation() + 1);
        if (_freeList[handle.index()].generation() == 0)
            _freeList[handle.index()].setGeneration(1);

        u32 releasedIndex = _freeList[handle.index()].index();
        u32 lastIndex     = _storage.count - 1;
        // swap the released item with the last one to preserve packing
        _storage.array[releasedIndex]   = _storage.array[lastIndex];
        _storage.indices[releasedIndex] = _storage.indices[lastIndex];
        // make freelist for swapped element point to the new location
        _freeList[_storage.indices[lastIndex]].setIndex(releasedIndex);
        _storage.count--;

        _freeList[handle.index()].setIndex(_freeHead);
        _freeHead = handle.index();
    }

    T* get(const H& handle)
    {
        // double indirection because of the free list but
        // we gained cache friendly packed object array
        if (isValid(handle))
            return &(_storage.array[_freeList[handle.index()].index()]);

        return 0x0;
    }

    const T* get(const H& handle) const
    {
        // double indirection because of the free list but
        // we gained cache friendly packed object array

        if (isValid(handle))
            return &(_storage.array[_freeList[handle.index()].index()]);

        return 0x0;
    }

    u32 getPackedIndex(const H& handle) const
    {
        if (isValid(handle))
            return _freeList[handle.index()].index();

        return kInvalidPackedIndex;
    }

    H getHandleFromPackedIndex(u32 index) const
    {
        assert(index < _storage.count);
        H handle(_storage.indices[index], _freeList[_storage.indices[index]].generation());
        return handle;
    }

    // chache friendly, iterate over it
    const PackedStorage& storage() const
    {
        return _storage;
    }

    PackedStorage& storage()
    {
        return _storage;
    }

private:
    u32           _freeHead;
    H             _freeList[StorageSize];
    PackedStorage _storage;
};

}  // atlas namespace
