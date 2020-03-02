#pragma once

#include "Assets/Geometry.h"
#include "Assets/Types.h"

namespace atlas
{
class MeshComponent
{
public:
    MeshComponent() {}
    MeshComponent(AssetPtr mesh) : _mesh(mesh) {}
    std::shared_ptr<GeometryAsset> geometry()
    {
        return std::static_pointer_cast<GeometryAsset>(_mesh);
    }

    void setGeomtry(AssetPtr geometryAsset)
    {
        _mesh = geometryAsset;
    }

    template <class Serializer>
    void Serialize(Serializer& serializer)
    {
    }
private:
    AssetPtr _mesh;
};

}  // namespace Atlas
