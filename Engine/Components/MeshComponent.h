#pragma once

#include "Assets/Geometry.h"
#include "Assets/Types.h"

namespace atlas
{
class MeshComponent
{
public:
    std::shared_ptr<GeometryAsset> geometry()
    {
        return std::static_pointer_cast<GeometryAsset>(_mesh);
    }

    void setGeomtry(AssetPtr geometryAsset)
    {
        _mesh = geometryAsset;
    }

private:
    AssetPtr _mesh;
};

}  // namespace Atlas
