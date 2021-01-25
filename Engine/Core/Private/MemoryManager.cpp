#include "Core/MemoryManager.h"
#include <bx/allocator.h>

namespace atlas
{

//class AllocatorC99 : public bx::AllocatorI
//{
//public:
//    virtual ~AllocatorC99() { }
//
//    virtual void* realloc(void* _ptr, size_t _size, size_t _align, const char* _file, uint32_t _line) override
//    {
//        return m_interface->vtbl->realloc(m_interface, _ptr, _size, _align, _file, _line);
//    }
//
//    bgfx_allocator_interface_t* m_interface;
//};

MemoryManager& MemoryManager::instance()
{
    static MemoryManager _instance;
    return _instance;
}

bx::AllocatorI* MemoryManager::bxAllocator()
{
    static bx::DefaultAllocator allocator;
    return &allocator;
}

}
