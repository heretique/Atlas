#include "Ecs/ECSManager.h"
#include "Core/Component.h"
#include "Hq/JsonSerializer.h"
//#include "Camera/Camera.h"
//#include "Hq/Math/Mat4x4.h"
#include "rttr/type.h"

namespace atlas
{

ECSManager::ECSManager()
    : _registry(new entt::registry())
{
}

ECSManager::~ECSManager() { }

ECSManager& ECSManager::instance()
{
    static ECSManager _instance;
    return _instance;
}

entt::registry& ECSManager::registry()
{
    return *_registry;
}

void ECSManager::registerUpdateSystem(std::unique_ptr<ISystem> system)
{
    _updateSystems.emplace_back(std::move(system));
}

void ECSManager::registerVisualSystem(std::unique_ptr<ISystem> system)
{
    _visualSystems.emplace_back(std::move(system));
}

void ECSManager::runUpdateSystems(entt::registry& registry, float dt)
{
    for (auto& system : _updateSystems)
    {
        system->runSystem(registry, dt);
    }
}

void ECSManager::runVisualSystems(entt::registry& registry, float dt)
{
    for (auto& system : _visualSystems)
    {
        system->runSystem(registry, dt);
    }
}

}  // atlas namespace
