#include "MaterialComponent.h"
#include "Core/SerializationArchives.h"

namespace atlas
{
MaterialComponent::MaterialComponent(ComponentType type)
    : Component(type)
{
}

}  // namespace atlas
CEREAL_REGISTER_TYPE(atlas::MaterialComponent)
