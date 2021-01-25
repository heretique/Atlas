#pragma once

#include "Core/Component.h"
#include "Reflection/Reflection.h"
#include "Materials/Material.h"
#include "Assets/Types.h"

namespace atlas
{
class MaterialAsset;
class MaterialComponent : public Component
{
    REFLECTABLE(Component)
public:
    std::shared_ptr<MaterialAsset> material;
};

}  // namespace Atlas
