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
using AssetPtr     = std::shared_ptr<Asset>;
using AssetStorage = std::vector<AssetPtr>;
using AssetType    = StringHash;

namespace AssetNames
{
    static const std::string Undefined{"Undefined"};
    static const std::string Scene{"Scene"};
    static const std::string Geometry{"Geometry"};
    static const std::string Animation{"Animation"};
    static const std::string Audio{"Audio"};
    static const std::string Material{"Material"};
    static const std::string Code{"Code"};
    static const std::string Shader{"Shader"};
    static const std::string Texture{"Texture"};
    static const std::string ParticleEffect{"Particles"};
    static const std::string Pipeline{"Pipeline"};
    static const std::string Template{"Template"};
}

namespace AssetTypes
{
    static const AssetType Undefined{AssetNames::Undefined};
    static const AssetType Scene{AssetNames::Scene};
    static const AssetType Geometry{AssetNames::Geometry};
    static const AssetType Animation{AssetNames::Animation};
    static const AssetType Audio{AssetNames::Audio};
    static const AssetType Material{AssetNames::Material};
    static const AssetType Code{AssetNames::Code};
    static const AssetType Shader{AssetNames::Shader};
    static const AssetType Texture{AssetNames::Texture};
    static const AssetType ParticleEffect{AssetNames::ParticleEffect};
    static const AssetType Pipeline{AssetNames::Pipeline};
    static const AssetType Template{AssetNames::Template};

    const std::string& toName(AssetType type);
}  // AssetTypes namespace

namespace wren
{
    void bindAssetTypes();
}

}  // atlas namespace
