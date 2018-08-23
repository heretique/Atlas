#include "MaterialComponent.h"

namespace atlas
{
void MaterialComponent::setMaterial(AssetPtr material)
{
    _material = material;
}

void MaterialComponent::clearMaterial()
{
    _material = nullptr;
}

std::shared_ptr<MaterialAsset> MaterialComponent::material()
{
    return std::static_pointer_cast<MaterialAsset>(_material);
}

}  // namespace atlas
