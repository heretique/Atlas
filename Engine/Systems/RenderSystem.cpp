#include "rendersystem.h"
#include "Math/Transform.h"
#include "Components/meshcomponent.h"

namespace atlas {

using namespace entityx;

RenderSystem::RenderSystem()
{

}

void RenderSystem::update(EntityManager &entities, EventManager &events, TimeDelta dt)
{
    entities.each<math::Transform, MeshComponent>([this,dt] (Entity entity, math::Transform &transform, MeshComponent &meshComp) {
        updateEntity(entity, transform, meshComp, dt);
    });
}


void RenderSystem::updateEntity(Entity entity, math::Transform &transform, MeshComponent &meshComp, TimeDelta dt)
{

}

} // namespace Atlas


