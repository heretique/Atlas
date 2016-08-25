#include "ECSManager.h"
#include "Systems/RenderSystem.h"

namespace atlas {

void ECSManager::installSystems()
{
    _systems.add<RenderSystem>();


    // call this after registering all systems
    _systems.configure();
}

void ECSManager::update(entityx::TimeDelta dt)
{
    // update systems in priority order
    // TODO: implement flexible priority ordering
    _systems.update<RenderSystem>(dt);
}

} // atlas


