#pragma once

#include "Component.h"
#include "Core/Types.h"

#include <bgfx/bgfx.h>
#include <cereal/cereal.hpp>

namespace atlas
{
class MeshComponent : public Component
{
public:
    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(                                      //
            cereal::base_class<Component>(this)  //
            // add here when defining this component
            );
    }

private:
};

}  // namespace Atlas
