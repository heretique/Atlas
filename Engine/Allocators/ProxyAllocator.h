#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////
///////////////// Tiago Costa, 2014
/////////////////////////////////////////////////////////////////////////////////////////////

#include "Allocators/Allocator.h"

class ProxyAllocator : public Allocator
{
public:
    ProxyAllocator(Allocator& allocator);
    ~ProxyAllocator();

    void* allocate(size_t size, u8 alignment) override;

    void deallocate(void* p) override;

private:
    ProxyAllocator(const ProxyAllocator&);  // Prevent copies because it might cause errors
    ProxyAllocator& operator=(const ProxyAllocator&);

    Allocator& _allocator;
};

namespace allocator
{
inline ProxyAllocator* newProxyAllocator(Allocator& allocator)
{
    void* p = allocator.allocate(sizeof(ProxyAllocator), __alignof(ProxyAllocator));
    return new (p) ProxyAllocator(allocator);
}

inline void deleteProxyAllocator(ProxyAllocator& proxyAllocator, Allocator& allocator)
{
    proxyAllocator.~ProxyAllocator();

    allocator.deallocate(&proxyAllocator);
}
}  // allocator namespace
