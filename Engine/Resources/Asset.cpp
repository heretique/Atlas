#include "Asset.h"
#include <Managers/LogManager.h>

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
    LOGINFO("Asset cloning not implemented for type %i", _type);
    return nullptr;
}

bool Asset::load(const std::istream& data)
{
    LOGWARNING("Asset '%s' of type %i: No data loaded (file not found?)", _name.c_str(), _type);

    return false;
}

}  // atlas namespace
