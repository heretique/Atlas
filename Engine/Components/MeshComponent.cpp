#include "MeshComponent.h"
#include "Core/SerializationArchives.h"

namespace atlas
{
MeshComponent::MeshComponent()
    : _meshHash()
    , _mesh()
{
}

MeshComponent::MeshComponent(AssetPtr mesh, StringHash hash)
    : _meshHash(hash)
    , _mesh(mesh)
{
}

}  // namespace atlas
CEREAL_REGISTER_TYPE(atlas::MeshComponent)
