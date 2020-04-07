#pragma once

#include <cstdint>
#include <entt/fwd.hpp>

namespace atlas
{

class ISystem
{
public:
    virtual ~ISystem() {}
    virtual void runSystem(entt::registry& registry, float dt) = 0;
};

} // atlas namespace
