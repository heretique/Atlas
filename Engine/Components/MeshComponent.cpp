#include "MeshComponent.h"
#include "Core/SerializationArchives.h"

namespace atlas
{
MeshComponent::MeshComponent()
    : Component(StringHash("Mesh"))
    , _meshHash()
    , _mesh()
{
}

MeshComponent::MeshComponent(AssetPtr mesh, StringHash hash)
    : Component(StringHash("Mesh"))
    , _meshHash(hash)
    , _mesh(mesh)
{
}

}  // namespace atlas
CEREAL_REGISTER_TYPE(atlas::MeshComponent)
