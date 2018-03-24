#pragma once

#include <vector>
#include <bx/readerwriter.h>

namespace atlas
{
struct MemoryWriter : public bx::WriterI
{
    std::vector<char> buffer;
    // WriterI interface
    virtual int32_t write(const void* data, int32_t size, bx::Error* err) override;
};

}  // atlas namespace
