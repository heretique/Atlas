#pragma once

#include "Component.h"
#include "Assets/Material.h"
#include "Assets/Types.h"

namespace atlas
{
class MaterialAsset;
class MaterialComponent : public Component
{
    DECLARE_SUPER(Component)
    CREATE_TYPEID(MaterialComponent)
public:
    void                           setMaterial(AssetPtr material);
    void                           clearMaterial();
    std::shared_ptr<MaterialAsset> material();

private:
    AssetPtr _material;
};
REGISTER_SERIALIZABLE_TYPE(MaterialComponent)

}  // namespace Atlas
