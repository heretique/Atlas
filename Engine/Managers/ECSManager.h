#pragma once

#include "entt/fwd.hpp"
#include "entt/core/type_info.hpp"

#include <memory>
#include <unordered_map>
#include <functional>

namespace hq {
class Serializer;
}

namespace atlas
{

class ECSManager
{
public:
    ECSManager();
    ~ECSManager();

    entt::registry& registry();

    void serializeEntity(entt::entity entity, hq::Serializer& serializer);

    template<typename ComponentT>
    void registerComponentSerialization()
    {
        _componentSerializationMap[entt::type_info<ComponentT>::id()] = [&](entt::entity entity, hq::Serializer& serializer) {
            auto& component = _registry->get<ComponentT>(entity);
            serializer(component);
        };
    }



private:
    std::unique_ptr<entt::registry> _registry;
    std::unordered_map<ENTT_ID_TYPE, std::function<void(entt::entity, hq::Serializer&)>> _componentSerializationMap;
};

} // atlas namespace
