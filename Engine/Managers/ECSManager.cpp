#include "ECSManager.h"
#include "Hq/JsonSerializer.h"

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

}  // atlas namespace
