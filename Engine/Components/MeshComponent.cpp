#include "MeshComponent.h"
#include "Core/SerializationArchives.h"

namespace atlas
{
MeshComponent::MeshComponent(ComponentType type)
    : Component(StringHash("Mesh"))
{
}

void MeshComponent::setMesh(AssetPtr mesh)
{
    _mesh = mesh;
}

void MeshComponent::clearMesh()
{
    _mesh = nullptr;
}

}  // namespace atlas
CEREAL_REGISTER_TYPE(atlas::MeshComponent)
