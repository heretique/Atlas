#pragma once

#include "Core/Types.h"
#include <vector>

namespace atlas
{
template <typename T, typename H>
class DynFreeList
{
public:
    explicit DynFreeList(size_t capacity)
        : _capacity(capacity)
    {
        init();
    }

    void clear()
    {
        _freeList.clear();
        _storage.clear();
        init();
    }

    bool isValid(const H& handle) const
    {
        return ((handle.index() < _storage.size()) && (handle.generation() != 0) &&
                (_freeList[handle.index()].generation() == handle.generation()));
    }

    H alloc()
    {
        // Don't assert anymore, let higher level code handle allocation failure
        if (_freeHead >= _storage.size())
        {
            _storage.emplace_back();
            _freeList.emplace_back();
            H handle = _freeList.back();
            handle.setIndex(_storage.size());
            handle.setGeneration(1);
        }

        uint32_t index = _freeHead;
        _freeHead      = _freeList[index].index();
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
        return const_cast<T*>(static_cast<const DynFreeList*>(this)->get(handle));
    }

    const T* get(const H& handle) const
    {
        return isValid(handle) ? &(_storage[handle.index()]) : 0x0;
    }

    T& getRef(const H& handle)
    {
        return const_cast<T&>(static_cast<const DynFreeList*>(this)->getRef(handle));
    }

    const T& getRef(const H& handle) const
    {
        assert(isValid(handle));
        return _storage[handle.index()];
    }

private:
    void init()
    {
        _freeList.reserve(_capacity);
        _storage.reserve(_capacity);
    }

private:
    size_t         _capacity{0};
    uint32_t       _freeHead{0};
    std::vector<H> _freeList;
    std::vector<T> _storage;
};

template <typename T, typename H>
class DynPackedFreeList
{
    struct PackedStorage
    {
        // to remove an object with this solution we use the standard trick of swapping it
        // with the last item in the array. Then we update the index so that it points to
        // the new location of the swapped object.
        std::vector<size_t> indices;
        std::vector<T>      array;
    };

public:
    DynPackedFreeList(size_t capacity)
        : _capacity(capacity)
    {
        init();
    }

    void clear()
    {
        _freeList.clear();
        _storage.indices.clear();
        _storage.array.clear();
        init();
    }

    bool isValid(const H& handle) const
    {
        return ((handle.index() < _storage.array.size()) && (handle.generation() != 0) &&
                (_freeList[handle.index()].generation() == handle.generation()));
    }

    H alloc()
    {
        // Don't assert anymore, let higher level code handle allocation failure
        if (_freeHead >= _storage.array.size())
        {
            _storage.indices.emplace_back();
            _storage.array.emplace_back();
            _freeList.emplace_back();
            H handle = _freeList.back();
            handle.setIndex(_storage.indices.size());
            handle.setGeneration(1);
        }

        const uint32_t freeIndex = _freeHead;
        _freeHead                = _freeList[freeIndex].index();
        // storage location is first element beyond last
        _freeList[freeIndex].setIndex(_storage.array.size());
        const uint32_t storageLocation = _freeList[freeIndex].index();
        // point back to freelist element
        _storage.indices[storageLocation] = freeIndex;

        H handle(freeIndex, _freeList[freeIndex].generation());
        return handle;
    }

    void remove(const H& handle)
    {
        if (!isValid(handle) || _storage.array.size() == 0)
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

        _storage.array.erase(_storage.array.back());

        // set the release freelist index point to the next free one
        _freeList[handle.index()].setIndex(_freeHead);
        // and set the new free head here
        _freeHead = handle.index();
    }

    T* get(const H& handle)
    {
        return const_cast<T*>(static_cast<const DynPackedFreeList*>(this)->get(handle));
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
        return const_cast<T&>(static_cast<const DynPackedFreeList*>(this)->getRef(handle));
    }

    const T& getRef(const H& handle) const
    {
        assert(isValid(handle));
        return _storage.array[_freeList[handle.index()].index()];
    }

    size_t getPackedIndex(const H& handle) const
    {
        if (isValid(handle))
            return _freeList[handle.index()].index();

        return kInvalidPackedIndex;
    }

    H getHandleFromPackedIndex(size_t index) const
    {
        assert(index < _storage.count);
        H handle(_storage.indices[index], _freeList[_storage.indices[index]].generation());
        return handle;
    }

    const std::vector<T>& packedStorage() const
    {
        return _storage.array;
    }

private:
    void init()
    {
        _freeList.reserve(_capacity);
        _storage.indices.reserve(_capacity);
        _storage.array.reserve(_capacity);
    }

private:
    size_t         _capacity{0};
    size_t         _freeHead{0};
    std::vector<H> _freeList;
    PackedStorage  _storage;
};
}
