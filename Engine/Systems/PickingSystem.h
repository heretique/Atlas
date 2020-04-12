#pragma once

#include <entt/fwd.hpp>
#include <Hq/NonCopyable.h>
#include <Hq/Math/MathTypes.h>
#include "ISystem.h"

namespace atlas
{

class PickingSystem : public hq::NonCopyable, public ISystem
{
public:
    PickingSystem();
    static entt::entity pick(entt::registry& registry, const hq::math::Ray3& ray);

    // ISystem interface
public:
    void runSystem(entt::registry &registry, float dt) override;

private:
    entt::entity _pickedEntity;
    entt::entity _selectedEntity;
};

} // atlas namespace
