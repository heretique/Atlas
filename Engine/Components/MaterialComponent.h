#pragma once

#include "Common.h"
#include "Assets/Material.h"
#include "Assets/Types.h"

namespace atlas
{
class MaterialAsset;
class MaterialComponent : public Component
{
public:
    void setMaterial(AssetPtr material);
    void                           clearMaterial();
    std::shared_ptr<MaterialAsset> material();

    template <class Serializer>
    void Serialize(Serializer& serializer)
    {
    }

private:
    AssetPtr _material;
};

}  // namespace Atlas
