#pragma once

#include <entt/fwd.hpp>
#include <Hq/NonCopyable.h>
#include <Hq/Math/MathTypes.h>
#include "Ecs/ISystem.h"

namespace atlas
{
class PickingSystem : public hq::NonCopyable, public ISystem
{
public:
    PickingSystem();
    void                setCamera(entt::entity camera);
    static entt::entity pick(entt::registry& registry, entt::entity camera, const hq::math::Ray3& ray);

    // ISystem interface
public:
    void runSystem(entt::registry& registry, float dt) override;

private:
    entt::entity _pickedEntity;
    entt::entity _selectedEntity;
    entt::entity _camera;
};

}  // atlas namespace
