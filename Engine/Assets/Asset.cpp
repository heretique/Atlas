#include "Asset.h"
#include "Core/Engine.h"
#include <spdlog/spdlog.h>

namespace atlas
{
//
// Asset
//
Asset::Asset(int type, const std::string& name, u32 flags)
    : _type(type)
    , _name(name)
    , _flags(flags)
    , _loaded(false)
    , _handle(kInvalidAssetHandle)
{
}

Asset::~Asset()
{
}

AssetPtr Asset::clone() const
{
    Engine::log().warn("Asset cloning not implemented for type {}", _type);
    return nullptr;
}

bool Asset::load(const std::istream& data)
{
    Engine::log().warn("Asset '{}' of type {}: No data loaded (file not found?)", _name.c_str(), _type);

    return false;
}

}  // atlas namespace
