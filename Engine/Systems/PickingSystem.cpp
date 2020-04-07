#include "PickingSystem.h"
#include "Components/TransformComponent.h"
#include "Components/Camera.h"
#include "Core/Engine.h"
#include "Managers/InputManager.h"
#include "Managers/ECSManager.h"
#include "Utils/DebugDraw.h"
#include "Hq/PackUtils.h"
#include "Hq/Math/Math.h"

#include <entt/entity/registry.hpp>
#include <Hq/Math/Box3.h>
#include <Hq/Math/Ray3.h>
#include <Hq/Math/Vec3.h>

namespace atlas
{
PickingSystem::PickingSystem()
    : _pickedEntity(entt::null)
{
}

entt::entity PickingSystem::pick(entt::registry& registry, const hq::math::Ray3& ray)
{
    using namespace hq;
    using namespace hq::math;
    entt::entity selectedEntity              = entt::null;
	float        closestIntersectionDistance = kFloatMax;
    auto         view                        = registry.view<TransformComponent>();
    for (auto entity : view)
    {
        const TransformComponent& component = registry.get<TransformComponent>(entity);
        Box3                      transformedBounds;
        transform(component.bounds(), component.world(), transformedBounds);
		float intersectionDistance = kFloatMax;
        if (intersection(ray, transformedBounds, intersectionDistance))
        {
            if (intersectionDistance < closestIntersectionDistance)
            {
                closestIntersectionDistance = intersectionDistance;
                selectedEntity              = entity;
            }
        }
    }

    return selectedEntity;
}

void PickingSystem::runSystem(entt::registry& registry, float dt)
{
    using namespace hq;
    using namespace hq::math;
    Ray3    pickRay;
    Vec2    mousePos        = Engine::input().mousePos();
    Camera& cameraComponent = Engine::ecs().registry().get<Camera>(Engine::ecs().mainCamera());
    u32 width = Engine::viewWidth();
    u32 height = Engine::viewHeight();
    cameraComponent.pickRay(Rect(width, height), mousePos.x, mousePos.y, pickRay);
    _pickedEntity = PickingSystem::pick(Engine::ecs().registry(), pickRay);
    if (_pickedEntity != entt::null)
    {
        TransformComponent& transform = Engine::ecs().registry().get<TransformComponent>(_pickedEntity);
        Engine::debugDraw().begin();
        Engine::debugDraw().drawBox3(transform.bounds(), transform.world(), hq::packUint32(0, 255, 0, 255));
        Engine::debugDraw().drawRay3(pickRay, 10, packUint32(0, 0, 255, 255));
        Engine::debugDraw().end();
    }
}

}  // atlas namespace
