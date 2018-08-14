#include "MeshComponent.h"
#include "Core/SerializationArchives.h"

namespace atlas
{
MeshComponent::MeshComponent(ComponentType type)
    : Component(StringHash("Mesh"))
{
}

}  // namespace atlas
CEREAL_REGISTER_TYPE(atlas::MeshComponent)
