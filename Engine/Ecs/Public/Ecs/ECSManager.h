#pragma once

#include "entt/fwd.hpp"
#include "entt/core/type_info.hpp"
#include "entt/entity/registry.hpp"
#include "ISystem.h"
#include "Utils/TypeTraits.h"

#include <memory>
#include <unordered_map>
#include <functional>
#include <string>

namespace atlas
{
class Component;

class ECSRegistry : public entt::registry
{
public:
    Component* getByType(entt::entity entity, entt::type_info typeInfo);
};

class ECSManager
{
public:
    ECSManager();
    ~ECSManager();

    static ECSManager& instance();

    ECSRegistry& registry();

    void serializeEntity(entt::entity entity);
    template <typename Serializer>
    void deserializeEntity(entt::entity entity, Serializer& serializer);

    template <typename Component, typename Serializer, typename Deserializer>
    void registerComponentSerialization();

    template <typename Component>
    void registerComponentName(const char* name);

    void registerUpdateSystem(std::unique_ptr<ISystem> system);
    void registerVisualSystem(std::unique_ptr<ISystem> system);
    void runUpdateSystems(entt::registry& registry, float dt);
    void runVisualSystems(entt::registry& registry, float dt);

private:
    std::unique_ptr<ECSRegistry>           _registry;
    std::vector<std::unique_ptr<ISystem> > _updateSystems;
    std::vector<std::unique_ptr<ISystem> > _visualSystems;
};

struct ComponentNames
{
    static std::unordered_map<ENTT_ID_TYPE, const char*> value;
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
    ComponentSerializationMap<Serializer>::value[entt::type_id<Component>().hash()] = [&](entt::entity entity,
                                                                                          Serializer&  serializer) {
        auto& component = _registry->get<Component>(entity);
        serializer(component);
    };

    ComponentDeserializationMap<Deserializer>::value[entt::type_id<Component>().hash()] =
        [&](entt::entity entity, Deserializer& deserializer) {
            Component component;
            deserializer(component);
            _registry->emplace<Component>(entity);
        };
}

template <typename Component>
void ECSManager::registerComponentName(const char* name)
{
    ComponentNames::value[entt::type_info<Component>::id()] = name;
}

}  // atlas namespace
