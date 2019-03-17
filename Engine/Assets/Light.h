#pragma once

#include "Asset.h"

namespace atlas
{
enum class LightTypes
{
    None = 0,
    Directional,
    Omni,
    Spot,
    Area
};

class LightAsset : public Asset
{
};

}  // namespace atlas
