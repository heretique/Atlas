#include "Picking/PickingSystem.h"
#include "Core/TransformComponent.h"
#include "Camera/Camera.h"
#include "Core/Component.h"
#include "Input/InputManager.h"
#include "Ecs/ECSManager.h"
#include "DebugDraw/DebugDraw.h"
#include "Hq/PackUtils.h"
#include "Hq/Math/Math.h"
#include "Engine/Engine.h"

#include <entt/entity/registry.hpp>
#include <Hq/Math/Box3.h>
#include <Hq/Math/Ray3.h>
#include <Hq/Math/Vec3.h>

namespace atlas
{
PickingSystem::PickingSystem()
    : _pickedEntity(entt::null)
    , _selectedEntity(entt::null)
    , _camera(entt::null)
{
}

void PickingSystem::setCamera(entt::entity camera)
{
    _camera = camera;
}

entt::entity PickingSystem::pick(entt::registry& registry, entt::entity camera, const hq::math::Ray3& ray)
{
    using namespace hq;
    using namespace hq::math;
    if (camera == entt::null)
    {
        return entt::null;
    }

    entt::entity selectedEntity              = entt::null;
    float        closestIntersectionDistance = kFloatMax;
    auto         view                        = registry.view<TransformComponent>();
    for (auto entity : view)
    {
        const TransformComponent& component = registry.get<TransformComponent>(entity);
        Box3                      transformedBounds;
        transform(component.bbox, component.world, transformedBounds);
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

    if (_camera == entt::null)
    {
        return;
    }

    Ray3        pickRay;
    Vec2        mousePos        = InputManager::instance().mousePos();
    ECSManager& ecs             = ECSManager::instance();
    Camera&     cameraComponent = ecs.registry().get<Camera>(_camera);
    u32         width           = Engine::viewWidth();
    u32         height          = Engine::viewHeight();
    cameraComponent.pickRay(Rect(width, height), mousePos.x, mousePos.y, pickRay);
    _pickedEntity = PickingSystem::pick(Engine::ecs().registry(), _camera, pickRay);
    if (_pickedEntity != entt::null)
    {
        TransformComponent& transform = ECSManager::instance().registry().get<TransformComponent>(_pickedEntity);
        //DebugDraw::begin();
        //DebugDraw::drawBox3(transform.bbox, transform.world, hq::packUint32(0, 255, 0, 255));
        //DebugDraw::end();
        if (Engine::input().mouseClick(InputManager::EMouseButtons::Left))
        {
            if (_selectedEntity != entt::null)
            {
                registry.remove_if_exists<Selected>(_selectedEntity);
            }
            _selectedEntity = _pickedEntity;
            registry.emplace<Selected>(_selectedEntity);
        }
    }
}

}  // atlas namespace
