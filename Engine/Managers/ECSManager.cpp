#include "ECSManager.h"
#include "Hq/JsonSerializer.h"
#include "Components/Camera.h"
#include "Core/Engine.h"
#include "Hq/Math/Mat4x4.h"

namespace atlas
{
ECSManager::ECSManager()
    : _registry(new entt::registry())
{
    using namespace hq::math;
    _mainCamera             = _registry->create();
    Camera& cameraComponent = _registry->emplace<Camera>(_mainCamera);
    cameraComponent.setPerspective(60.f, float(Engine::viewWidth()) / Engine().viewHeight(), .1f, 1000.f);
    Mat4x4 cameraView {Mat4x4::Identity};
    createLookAt(Vec3(5, 5, 10), Vec3::Zero, Vec3(0.f, 1.f, 0.f), cameraView);
    invert(cameraView);
    cameraComponent.setTransform(cameraView);
}

ECSManager::~ECSManager() {}

entt::registry& ECSManager::registry()
{
    return *_registry;
}

void ECSManager::registerUpdateSystem(std::shared_ptr<ISystem> system)
{
    _updateSystems.emplace_back(system);
}

void ECSManager::registerVisualSystem(std::shared_ptr<ISystem> system)
{
    _visualSystems.emplace_back(system);
}

void ECSManager::runUpdateSystems(entt::registry& registry, float dt)
{
    for (auto system : _updateSystems)
    {
        system->runSystem(registry, dt);
    }
}

void ECSManager::runVisualSystems(entt::registry& registry, float dt)
{
    for (auto system : _visualSystems)
    {
        system->runSystem(registry, dt);
    }
}

entt::entity ECSManager::mainCamera() const
{
    return _mainCamera;
}

}  // atlas namespace
