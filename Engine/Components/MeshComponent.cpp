#include "MeshComponent.h"
#include "Core/SerializationArchives.h"

namespace atlas
{
MeshComponent::MeshComponent()
    : _meshHash()
    , _mesh()
{
}

MeshComponent::MeshComponent(AssetHandle handle, StringHash hash)
    : _meshHash(hash)
    , _mesh(handle)
{
}

}  // namespace atlas
CEREAL_REGISTER_TYPE(atlas::MeshComponent)
