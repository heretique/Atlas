#include "ECSManager.h"
#include "Components/Common.h"
#include "Hq/JsonSerializer.h"
#include "Components/Camera.h"
#include "Core/Engine.h"
#include "Hq/Math/Mat4x4.h"
#include "rttr/type.h"

namespace atlas
{
std::unordered_map<ENTT_ID_TYPE, const char*> ComponentNames::value = {};

ECSManager::ECSManager()
    : _registry(new ECSRegistry())
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

ECSManager::~ECSManager() { }

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

entt::entity ECSManager::mainCamera() const
{
    return _mainCamera;
}


void ECSManager::serializeEntity(entt::entity entity)
{
    _registry->visit(entity, [&](const auto typeInfo) { 
        Component* comp = _registry->getByType(entity, typeInfo);
        if (comp)
        {
           rttr::type compType = rttr::type::get(comp);
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
