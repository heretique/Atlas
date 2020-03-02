#pragma once

#include <entt/fwd.hpp>
#include <Hq/Math/MathTypes.h>

namespace atlas
{

class PickingSystem
{
public:
    static entt::entity pick(entt::registry& registry, const hq::math::Ray3& ray);
};

} // atlas namespace
