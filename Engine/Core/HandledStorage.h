#pragma once

namespace atlas {
static const uint32_t INVALID_PACKED_INDEX = UINT32_MAX;

/// Freelist+array based storage using safe handles to identify objects
/// Example usage:
///     typedef HandledStorage<Animation, 1024, 20, 12> AnimationStorage;
///     typedef AnimationStorage::Handle AnimationHandle;
///     AnimationStorage storage;
///     AnimationHandle handle = storage.add(Animation(...));
///     Animation* anim = storage.get(handle);
///     storage.remove(handle); // handle is now invalid
template <typename T, size_t StorageSize, size_t StorageBits,
          size_t GenerationBits>
struct HandledStorage {
  //        constexpr static uint32_t StorageSize = (1 << StorageBits);

  struct Handle {
    uint32_t index : StorageBits;
    uint32_t generation : GenerationBits;
  };

  HandledStorage() {
    memset(_storage, 0x0, StorageSize * sizeof(T));
    for (uint32_t i = 0; i < StorageSize; ++i) {
      _freeList[i].index = i + 1;
      _freeList[i].generation = 1;
    }
    _freeHead = 0;
  }

  bool isValid(const Handle &handle) const {
    return (handle.generation != 0) &&
           (_freeList[handle.index].generation == handle.generation);
  }

  Handle alloc() {
    // Assert: No more handles available
    assert(_freeHead < StorageSize);
    uint32_t index = _freeHead;
    _freeHead = _freeList[index].index;
    Handle handle = {index, _freeList[index].generation};
    return handle;
  }

  void remove(const Handle &handle) {
    if (!isValid(handle))
      return;

    _freeList[handle.index].generation++;

    if (_freeList[handle.index].generation == 0)
      _freeList[handle.index].generation = 1;

    _freeList[handle.index].index = _freeHead;
    _freeHead = handle.index;
  }

  T *get(const Handle &handle) {
    return isValid(handle) ? &(_storage[_freeList[handle.index].index]) : 0x0;
  }

  const T *get(const Handle &handle) const { return get(handle); }

private:
  T _storage[StorageSize];
  Handle _freeList[StorageSize];
  uint32_t _freeHead;
};

/// Similar with HandledStorage but it keeps a cache friendly packed array that
/// can be iterated over
template <typename T, size_t StorageSize, size_t StorageBits,
          size_t GenerationBits>
struct HandledPackedStorage {
  //        constexpr static uint32_t StorageSize = (1 << StorageBits);

  struct Handle {
    uint32_t index : StorageBits;
    uint32_t generation : GenerationBits;
  };

  struct PackedStorage {
    uint32_t count;
    uint32_t indices[StorageSize];
    T array[StorageSize];
  };

  HandledPackedStorage() {
    //        //probably not necessary
    //        memset(&(_storage.array), 0x0,
    //        StorageSize*sizeof(PackedStorage::array));

    // initialize each cell to point to the next for free list
    // when an index is not used to point to an object we use it
    // to point to the next item in the free list
    // first item in the free list is given by _freeHead
    // last by StorageSize - 1 (when they are equal the storage is full
    // and the free list is empty
    for (uint32_t i = 0; i < StorageSize; ++i) {
      _freeList[i].index = i + 1;
      _freeList[i].generation = 1;
    }
    _freeHead = 0;

    // count holds the number of packed stored elements
    // to remove an object with this solution we use the standard trick of
    // swapping it
    // with the last item in the array. Then we update the index so that it
    // points to
    // the new location of the swapped object.
    _storage.count = 0;
  }

  bool isValid(const Handle &handle) const {
    return (handle.generation != 0) &&
           (_freeList[handle.index].generation == handle.generation);
  }

  Handle alloc() {
    // Assert: No more handles available
    assert(_freeHead != StorageSize);
    uint32_t freeIndex = _freeHead;
    _freeHead = _freeList[freeIndex].index;
    // storage location is first element beyond last
    _freeList[freeIndex].index = _storage.count;
    uint32_t storageLocation = _freeList[freeIndex].index;
    // point back to freelist element
    _storage.indices[storageLocation] = freeIndex;
    _storage.count++;

    Handle handle = {freeIndex, _freeList[freeIndex].generation};
    return handle;
  }

  void remove(const Handle &handle) {
    if (!isValid(handle) || _storage.count < 1)
      return;

    _freeList[handle.index].generation++;
    if (_freeList[handle.index].generation == 0)
      _freeList[handle.index].generation = 1;

    uint32_t releasedIndex = _freeList[handle.index].index;
    uint32_t lastIndex = _storage.count - 1;
    // swap the released item with the last one to preserve packing
    _storage.array[releasedIndex] = _storage.array[lastIndex];
    _storage.indices[releasedIndex] = _storage.indices[lastIndex];
    // make freelist for swapped element point to the new location
    _freeList[_storage.indices[lastIndex]].index = releasedIndex;
    _storage.count--;

    _freeList[handle.index].index = _freeHead;
    _freeHead = handle.index;
  }

  T *get(const Handle &handle) {
    // double indirection because of the free list but
    // we gained cache friendly packed object array
    if (isValid(handle))
      return &(_storage.array[_freeList[handle.index].index]);

    return 0x0;
  }

  const T *get(const Handle &handle) const {
    // double indirection because of the free list but
    // we gained cache friendly packed object array
    //            if (isValid(handle))
    //                return &(m_storage.array[m_freeList[handle.index].index]);

    return get(handle);
  }

  uint32_t getPackedIndex(const Handle &handle) const {
    if (isValid(handle))
      return _freeList[handle.index].index;

    return INVALID_PACKED_INDEX;
  }

  Handle getHandleFromPackedIndex(uint32_t index) const {
    assert(index < _storage.count);
    Handle handle = {_storage.indices[index],
                     _freeList[_storage.indices[index]].generation};
    return handle;
  }

  // chache friendly, iterate over it
  const PackedStorage &storage() const { return _storage; }

  PackedStorage &storage() { return _storage; }

private:
  uint32_t _freeHead;
  Handle _freeList[StorageSize];
  PackedStorage _storage;
};

} // atlas namespace
