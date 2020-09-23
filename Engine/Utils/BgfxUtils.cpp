#include "BgfxUtils.h"

#include <iterator>

namespace atlas
{
int32_t atlas::MemoryWriter::write(const void* data, int32_t size, bx::Error* err)
{
    BX_UNUSED(err);
    std::copy(static_cast<const char*>(data),         //
              static_cast<const char*>(data) + size,  //
              std::back_inserter(buffer));
    return size;
}

}  // atlas namespace
