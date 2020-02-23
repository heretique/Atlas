#include "ECSManager.h"
#include "entt/entity/registry.hpp"

namespace atlas
{
ECSManager::ECSManager()
    : _registry(new entt::registry())
{
}

ECSManager::~ECSManager() {}

entt::registry& ECSManager::registry()
{
    return *_registry;
}

void ECSManager::serializeEntity(entt::entity entity, hq::Serializer& serializer)
{

    _registry->visit(entity, [&](const auto component) {
        auto it = _componentSerializationMap.find(component);
        assert(it != _componentSerializationMap.end());
        it->second(entity, serializer);
    });
}

}  // atlas namespace
