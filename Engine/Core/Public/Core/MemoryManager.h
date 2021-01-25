#pragma once

namespace bx
{
struct AllocatorI;
}

namespace atlas
{
class MemoryManager
{
public:
    static MemoryManager&  instance();
    static bx::AllocatorI* bxAllocator();
};
}
