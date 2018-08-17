#pragma once

#include "Core/Types.h"

#include <cassert>
#include <cstddef>
#include <cstring>

namespace atlas
{
template <typename T, bool IsPOD = true>
struct FreelistConstructor
{
    static void construct(T*)
    {
    }
    static void destruct(T*)
    {
    }
};
// Non-POD
template <typename T>
struct FreelistConstructor<T, false>
{
    static void construct(T* obj)
    {
        new (obj) T();
    }
    static void destruct(T* obj)
    {
        obj->~T();
    }
};

/// @brief Freelist+array based storage using safe handles to identify objects
/// @note This is intended for POD objects, removing objects from this freelist or clearing the list does not call
/// destructors
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
/// @tparam IsPOD bool specifying if the stored type is POD
template <typename T, typename H, size_t StorageSize, bool IsPOD>
struct FreeList
{
    static const size_t kStorageSize = StorageSize;

    FreeList()
        : _freeHead(0)
        , _storage()
    {
        for (uint32_t i = 0; i < StorageSize; ++i)
        {
            _freeList[i].setIndex(i + 1);
            _freeList[i].setGeneration(1);
        }
    }

    void clear()
    {
        for (uint32_t i = 0; i < kStorageSize; ++i)
        {
            FreelistConstructor<T, IsPOD>::destruct(&_storage[i]);
            FreelistConstructor<T, IsPOD>::construct(&_storage[i]);
        }
    }

    bool isValid(const H& handle) const
    {
        assert(handle.index() < StorageSize);
        return (handle.generation() != 0) && (_freeList[handle.index()].generation() == handle.generation());
    }

    H alloc()
    {
        // Don't assert anymore, let higher level code handle allocation failure
        if (_freeHead >= StorageSize)
            return H::invalid;

        uint32_t index = _freeHead;
        _freeHead      = _freeList[index].index();
        H handle(index, _freeList[index].generation());
        return handle;
    }

    void remove(const H& handle)
    {
        if (!isValid(handle))
            return;

        FreelistConstructor<T, IsPOD>::destruct(&_storage[handle.index()]);
        FreelistConstructor<T, IsPOD>::construct(&_storage[handle.index()]);

        _freeList[handle.index()].setGeneration(_freeList[handle.index()].generation() + 1);

        if (_freeList[handle.index()].generation() == 0)
            _freeList[handle.index()].setGeneration(1);

        _freeList[handle.index()].setIndex(_freeHead);
        _freeHead = handle.index();
    }

    T* get(const H& handle)
    {
        return const_cast<T*>(static_cast<const FreeList*>(this)->get(handle));
    }

    const T* get(const H& handle) const
    {
        return isValid(handle) ? &(_storage[handle.index()]) : 0x0;
    }

    T& getRef(const H& handle)
    {
        return const_cast<T&>(static_cast<const FreeList*>(this)->getRef(handle));
    }

    const T& getRef(const H& handle) const
    {
        assert(isValid(handle));
        return _storage[handle.index()];
    }

public:
    uint32_t _freeHead;
    H        _freeList[StorageSize];
    T        _storage[StorageSize];
};

/// Similar with FreeList but it keeps a cache friendly packed array that can be iterated over
template <typename T, typename H, size_t StorageSize, bool IsPOD>
struct PackedFreeList
{
    static const size_t kStorageSize = StorageSize;

    struct PackedStorage
    {
        PackedStorage()
            : count(0)
            , indices()
            , array()
        {
        }

        // count holds the number of packed stored elements
        // to remove an object with this solution we use the standard trick of swapping it
        // with the last item in the array. Then we update the index so that it points to
        // the new location of the swapped object.
        uint32_t count;
        uint32_t indices[StorageSize];
        T        array[StorageSize];
    };

    PackedFreeList()
        : _freeHead(0)
        , _storage()
    {
        // initialize each cell to point to the next for free list
        // when an index is not used to point to an object we use it
        // to point to the next item in the free list
        // first item in the free list is given by _freeHead
        // last by StorageSize - 1 (when they are equal the storage is full
        // and the free list is empty
        for (uint32_t i = 0; i < StorageSize; ++i)
        {
            _freeList[i].setIndex(i + 1);
            _freeList[i].setGeneration(1);
        }
    }

    void clear()
    {
        for (uint32_t i = 0; i < _storage.count; ++i)
        {
            FreelistConstructor<T, IsPOD>::destruct(&_storage.array[i]);
            FreelistConstructor<T, IsPOD>::construct(&_storage.array[i]);
        }
    }

    bool isValid(const H& handle) const
    {
        return ((handle.index() < StorageSize) && (handle.generation() != 0) &&
                (_freeList[handle.index()].generation() == handle.generation()));
    }

    H alloc()
    {
        // Don't assert anymore, let higher level code handle allocation failure
        if (_freeHead >= StorageSize)
            return H::invalid;

        const uint32_t freeIndex = _freeHead;
        _freeHead                = _freeList[freeIndex].index();
        // storage location is first element beyond last
        _freeList[freeIndex].setIndex(_storage.count);
        const uint32_t storageLocation = _freeList[freeIndex].index();
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

        // increment the generation
        _freeList[handle.index()].setGeneration(_freeList[handle.index()].generation() + 1);
        if (_freeList[handle.index()].generation() == 0)
            _freeList[handle.index()].setGeneration(1);

        uint32_t releasedIndex = _freeList[handle.index()].index();
        uint32_t lastIndex     = _storage.count - 1;
        // "swap" the released item with the last one to preserve packing
        std::swap(_storage.array[releasedIndex], _storage.array[lastIndex]);
        // swap indices to freelist handle too
        _storage.indices[releasedIndex] = _storage.indices[lastIndex];
        // make freelist for swapped element point to the new location
        _freeList[_storage.indices[lastIndex]].setIndex(releasedIndex);

        // reconstruct object, so it's clean the next time we use it
        FreelistConstructor<T, IsPOD>::destruct(&_storage.array[lastIndex]);
        FreelistConstructor<T, IsPOD>::construct(&_storage.array[lastIndex]);

        _storage.count--;

        // set the release freelist index point to the next free one
        _freeList[handle.index()].setIndex(_freeHead);
        // and set the new free head here
        _freeHead = handle.index();
    }

    T* get(const H& handle)
    {
        return const_cast<T*>(static_cast<const PackedFreeList*>(this)->get(handle));
    }

    const T* get(const H& handle) const
    {
        // double indirection because of the free list but
        // we gained cache friendly packed object array

        if (isValid(handle))
            return &(_storage.array[_freeList[handle.index()].index()]);

        return NULL;
    }

    T& getRef(const H& handle)
    {
        return const_cast<T&>(static_cast<const PackedFreeList*>(this)->getRef(handle));
    }

    const T& getRef(const H& handle) const
    {
        assert(isValid(handle));
        return _storage.array[_freeList[handle.index()].index()];
    }

    uint32_t getPackedIndex(const H& handle) const
    {
        if (isValid(handle))
            return _freeList[handle.index()].index();

        return kInvalidPackedIndex;
    }

    H getHandleFromPackedIndex(uint32_t index) const
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
    uint32_t      _freeHead;
    H             _freeList[StorageSize];
    PackedStorage _storage;
};

}  // atlas namespace
