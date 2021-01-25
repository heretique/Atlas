#pragma once

#include "Core/Component.h"
#include "Geometry/Geometry.h"
#include "Assets/Types.h"

namespace atlas
{
class MeshComponent : public Component
{
    REFLECTABLE(Component)
public:
    std::shared_ptr<GeometryAsset> geometry;

    MeshComponent() { }
    MeshComponent(std::shared_ptr<GeometryAsset> geometry)
        : geometry(geometry)
    {
    }
};

}  // namespace Atlas
