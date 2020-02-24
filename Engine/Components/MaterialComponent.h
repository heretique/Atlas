#pragma once

#include "Assets/Material.h"
#include "Assets/Types.h"

namespace atlas
{
class MaterialAsset;
class MaterialComponent
{
public:
    void setMaterial(AssetPtr material);
    void                           clearMaterial();
    std::shared_ptr<MaterialAsset> material();

private:
    AssetPtr _material;
};

}  // namespace Atlas
