#pragma once

#include "Assets/Asset.h"

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

    // Asset interface
protected:
    bool loadImpl(std::istream &data);
};

}  // namespace atlas
