#pragma once

#include "entt/fwd.hpp"
#include "entt/core/type_info.hpp"
#include "entt/entity/registry.hpp"

#include <memory>
#include <unordered_map>
#include <functional>

namespace atlas
{

class ECSManager
{
public:
    ECSManager();
    ~ECSManager();

    entt::registry& registry();

    template <typename Serializer>
    void serializeEntity(entt::entity entity, Serializer& serializer);

    template<typename Component, typename Serializer>
    void registerComponentSerialization();

private:
    std::unique_ptr<entt::registry> _registry;
    template<typename Serializer>
    static std::unordered_map<ENTT_ID_TYPE, std::function<void(entt::entity, Serializer&)>> _componentSerializationMap;
};

template<typename Serializer>
std::unordered_map<ENTT_ID_TYPE, std::function<void(entt::entity, Serializer&)>> ECSManager::_componentSerializationMap = {};

template<typename Serializer>
void ECSManager::serializeEntity(entt::entity entity, Serializer &serializer)
{

    _registry->visit(entity, [&](const auto component) {
        auto it = _componentSerializationMap<Serializer>.find(component);
        assert(it != _componentSerializationMap.end());
        it->second(entity, serializer);
    });
}


template<typename Component, typename Serializer>
void ECSManager::registerComponentSerialization()
{
    _componentSerializationMap<Serializer>[entt::type_info<Component>::id()] = [&](entt::entity entity, Serializer& serializer) {
        auto& component = _registry->get<Component>(entity);
        serializer(component);
    };
}



} // atlas namespace
