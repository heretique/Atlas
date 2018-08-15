#pragma once

#include "Assets/Types.h"
#include "Component.h"
#include "Core/Types.h"

#include <bgfx/bgfx.h>

namespace atlas
{
class MeshComponent : public Component
{
public:
    static ComponentPtr factoryFunc()
    {
        return std::make_unique<MeshComponent>(StringHash(ComponentNames::Mesh));
    }

    MeshComponent(ComponentType type);

    void setMesh(AssetPtr mesh);
    void clearMesh();

private:
    AssetPtr _mesh;
};

}  // namespace Atlas
