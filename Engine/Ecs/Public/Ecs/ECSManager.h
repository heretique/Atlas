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

class ECSManager
{
public:
    ECSManager();
    ~ECSManager();

    static ECSManager& instance();

    entt::registry& registry();

    void registerUpdateSystem(std::unique_ptr<ISystem> system);
    void registerVisualSystem(std::unique_ptr<ISystem> system);
    void runUpdateSystems(entt::registry& registry, float dt);
    void runVisualSystems(entt::registry& registry, float dt);

private:
    std::unique_ptr<entt::registry>        _registry;
    std::vector<std::unique_ptr<ISystem> > _updateSystems;
    std::vector<std::unique_ptr<ISystem> > _visualSystems;
};

}  // atlas namespace
