#include "MaterialComponent.h"
#include "Core/SerializationArchives.h"

namespace atlas
{
MaterialComponent::MaterialComponent(ComponentType type)
    : Component(type)
{
}

void MaterialComponent::setMaterial(AssetPtr material)
{
    _material = material;
}

void MaterialComponent::clearMaterial()
{
    _material = nullptr;
}

}  // namespace atlas
CEREAL_REGISTER_TYPE(atlas::MaterialComponent)
