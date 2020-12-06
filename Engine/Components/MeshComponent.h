#pragma once

#include "Common.h"
#include "Assets/Geometry.h"
#include "Assets/Types.h"

namespace atlas
{
class MeshComponent : public Component
{
    RTTR_ENABLE(Component)
public:
    std::shared_ptr<GeometryAsset> geometry;

    MeshComponent() { }
    MeshComponent(std::shared_ptr<GeometryAsset> geometry)
        : geometry(geometry)
    {
    }
};

}  // namespace Atlas
