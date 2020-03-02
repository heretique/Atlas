#include "PickingSystem.h"
#include "Components/TransformComponent.h"

#include <entt/entity/registry.hpp>
#include <Hq/Math/Box3.h>
#include <Hq/Math/Ray3.h>
#include <Hq/Math/Vec3.h>

namespace atlas
{

entt::entity PickingSystem::pick(entt::registry &registry, const hq::math::Ray3 &ray)
{
    using namespace hq;
    using namespace hq::math;
    entt::entity selectedEntity = entt::null;
    float closestIntersectionDistance = FLT_MAX;
    auto view = registry.view<TransformComponent>();
    for (auto entity: view)
    {
        const TransformComponent& component = registry.get<TransformComponent>(entity);
        Box3 transformedBounds;
        transform(component.bounds(), component.world(), transformedBounds);
        float intersectionDistance = FLT_MAX;
        if (intersection(ray, transformedBounds, intersectionDistance))
        {
            if (intersectionDistance < closestIntersectionDistance)
            {
                closestIntersectionDistance = intersectionDistance;
                selectedEntity = entity;
            }
        }
    }

    return selectedEntity;
}

} // atlas namespace
