#pragma once

#include "Assets/Types.h"
#include "Component.h"
#include "Core/Types.h"

#include <bgfx/bgfx.h>
#include <cereal/cereal.hpp>

namespace atlas
{
class MeshComponent : public Component
{
public:
    MeshComponent();
    MeshComponent(AssetHandle handle, StringHash hash);

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(                                       //
            cereal::base_class<Component>(this),  //
            CEREAL_NVP(_meshHash)                 //
            );
    }

private:
    StringHash  _meshHash;
    AssetHandle _mesh;
};

}  // namespace Atlas
