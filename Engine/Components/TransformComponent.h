#pragma once

#include "Common.h"
#include "Hq/Math/MathTypes.h"

namespace atlas
{
class TransformComponent : public Component
{
    RTTR_ENABLE(Component)
public:
    hq::math::Mat4x4 world {hq::math::Mat4x4::Identity};
    hq::math::Mat4x4 local {hq::math::Mat4x4::Identity};
    hq::math::Box3   bbox;
};

}  // atlas namespace
