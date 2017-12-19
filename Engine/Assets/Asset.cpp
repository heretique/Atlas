#include "Asset.h"
#include "Core/Engine.h"
#include <spdlog/spdlog.h>

namespace atlas
{
//
// Asset
//
Asset::Asset(AssetType type, const std::string& filename, u32 flags)
    : _type(type)
    , _filename(filename)
    , _hash(filename)
    , _flags(flags)
{
}

Asset::~Asset()
{
}

bool Asset::load(const std::istream& data)
{
    bool loaded = loadImpl(data);
    if (!isGPUResource())
        _loaded = loaded;

    return _loaded;
}

bool Asset::uploadGPU()
{
    _loaded = uploadGPUImpl();
    return _loaded;
}

bool Asset::isGPUResource()
{
    return false;
}

bool Asset::uploadGPUImpl()
{
    return false;
}

}  // atlas namespace
