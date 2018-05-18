#pragma once

#include "Assets/Types.h"
#include "Component.h"
#include <cereal/cereal.hpp>

namespace atlas
{
class MaterialComponent : public Component
{
public:
    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(                                       //
            cereal::base_class<Component>(this),  //
            CEREAL_NVP(_materialHash)             //
            );
    }

private:
    StringHash _materialHash;
    AssetPtr   _material;
};

}  // namespace Atlas
