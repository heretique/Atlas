#pragma once

#include "Assets/Types.h"
#include "Component.h"
#include <cereal/cereal.hpp>

namespace atlas
{
class MaterialComponent : public Component
{
public:
private:
    StringHash _materialHash;
    AssetPtr   _material;
};

}  // namespace Atlas
