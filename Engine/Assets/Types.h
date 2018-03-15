#pragma once

#include "Core/FreeList.h"
#include "Core/Handle.h"
#include "Core/StringHash.h"
#include <memory>

namespace atlas
{
// do not change bitfields size without changing the API handle bitfield size
// these need to be the same
class Asset;
using AssetPtr         = std::shared_ptr<Asset>;
using AssetHandle      = Handle<20, 12>;
using AssetStorage     = PackedFreeList<AssetPtr, AssetHandle, 4096>;
using AssetPackedArray = AssetStorage::PackedStorage;
using AssetType        = StringHash;

namespace AssetTypes
{
    static AssetType Undefined{"Undefined"};
    static AssetType Scene{"Scene"};
    static AssetType Geometry{"Geometry"};
    static AssetType Animation{"Animation"};
    static AssetType Audio{"Audio"};
    static AssetType Material{"Material"};
    static AssetType Code{"Code"};
    static AssetType Shader{"Shader"};
    static AssetType Texture{"Texture"};
    static AssetType ParticleEffect{"Particles"};
    static AssetType Pipeline{"Pipeline"};
    static AssetType Template{"Template"};
}  // AssetTypes namespace

namespace wren
{
    void bindAssetTypes();
}

}  // atlas namespace
