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
    MeshComponent();
    MeshComponent(AssetPtr mesh, StringHash hash);

private:
    StringHash _meshHash;
    AssetPtr   _mesh;
};

}  // namespace Atlas
