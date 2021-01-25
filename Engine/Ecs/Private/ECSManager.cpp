#include "Ecs/ECSManager.h"
#include "Core/Component.h"
#include "Hq/JsonSerializer.h"
//#include "Camera/Camera.h"
//#include "Hq/Math/Mat4x4.h"
#include "rttr/type.h"

namespace atlas
{
std::unordered_map<ENTT_ID_TYPE, const char*> ComponentNames::value = {};

ECSManager::ECSManager()
    : _registry(new ECSRegistry())
{
}

ECSManager::~ECSManager() { }

ECSManager& ECSManager::instance()
{
    static ECSManager _instance;
    return _instance;
}

ECSRegistry& ECSManager::registry()
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

void ECSManager::serializeEntity(entt::entity entity)
{
    _registry->visit(entity, [&](const auto typeInfo) {
        Component* comp = _registry->getByType(entity, typeInfo);
        if (comp)
        {
            rttr::type compType     = rttr::type::get(comp);
            rttr::type compTrueType = rttr::type::get_derived_type((void*)comp, compType);
            compType.get_name();
        }
    });
}

atlas::Component* ECSRegistry::getByType(entt::entity entity, entt::type_info typeInfo)
{
    if (typeInfo.seq() > pools.size())
        return nullptr;
    auto& pool = *pools[typeInfo.seq()].pool;
    return static_cast<atlas::Component*>(pool.getPtr(entity));
}

}  // atlas namespace
