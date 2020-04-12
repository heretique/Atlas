#pragma once

#include "entt/fwd.hpp"
#include "entt/core/type_info.hpp"
#include "entt/entity/registry.hpp"
#include "Systems/ISystem.h"

#include <memory>
#include <unordered_map>
#include <functional>
#include <string>

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
    template <typename Serializer>
    void deserializeEntity(entt::entity entity, Serializer& serializer);

    template <typename Component, typename Serializer, typename Deserializer>
    void registerComponentSerialization();

    void registerUpdateSystem(std::shared_ptr<ISystem> system);
    void registerVisualSystem(std::shared_ptr<ISystem> system);
    void runUpdateSystems(entt::registry& registry, float dt);
    void runVisualSystems(entt::registry& registry, float dt);

    entt::entity mainCamera() const;

private:
    std::unique_ptr<entt::registry>        _registry;
    std::vector<std::shared_ptr<ISystem> > _updateSystems;
    std::vector<std::shared_ptr<ISystem> > _visualSystems;

    entt::entity _mainCamera;
};

template <typename Serializer>
struct ComponentSerializationMap
{
    static std::unordered_map<ENTT_ID_TYPE, std::function<void(entt::entity, Serializer&)> > value;
};

template <typename Serializer>
struct ComponentDeserializationMap
{
    static std::unordered_map<ENTT_ID_TYPE, std::function<void(entt::entity, Serializer&)> > value;
};

template <typename Serializer>
std::unordered_map<ENTT_ID_TYPE, std::function<void(entt::entity, Serializer&)> >
    ComponentSerializationMap<Serializer>::value = {};

template <typename Serializer>
std::unordered_map<ENTT_ID_TYPE, std::function<void(entt::entity, Serializer&)> >
    ComponentDeserializationMap<Serializer>::value = {};


template <typename Serializer>
void ECSManager::serializeEntity(entt::entity entity, Serializer& serializer)
{
    _registry->visit(entity, [&](const auto component) {
        auto it = ComponentSerializationMap<Serializer>::value.find(component);
//        assert(it != ComponentSerializationMap<Serializer>::value.end() && "Component not registered for serializarion");
        if (it != ComponentSerializationMap<Serializer>::value.end())
            it->second(entity, serializer);
    });
}

template <typename Serializer>
void ECSManager::deserializeEntity(entt::entity entity, Serializer& serializer)
{
//    _registry->visit(entity, [&](const auto component) {
//        auto it = ComponentSerializationMap<Serializer>::value.find(component);
//        assert(it != ComponentSerializationMap<Serializer>::value.end());
//        it->second(entity, serializer);
//    });
}

template <typename Component, typename Serializer, typename Deserializer>
void ECSManager::registerComponentSerialization()
{
    ComponentSerializationMap<Serializer>::value[entt::type_info<Component>::id()] = [&](entt::entity entity,
                                                                                   Serializer&  serializer) {
        auto& component = _registry->get<Component>(entity);
        serializer(component);
    };

    ComponentDeserializationMap<Deserializer>::value[entt::type_info<Component>::id()] = [&](entt::entity entity,
                                                                                   Deserializer&  deserializer) {
        Component component;
        deserializer(component);
        _registry->emplace<Component>(entity);
    };
}

}  // atlas namespace
