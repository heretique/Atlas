#pragma once

#include "Common.h"
#include "Assets/Material.h"
#include "Assets/Types.h"

namespace atlas
{
class MaterialAsset;
class MaterialComponent : public Component
{
    RTTR_ENABLE(Component)
public:
    std::shared_ptr<MaterialAsset> material;
};

}  // namespace Atlas
