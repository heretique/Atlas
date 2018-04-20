#include "Systems/RenderSystem.h"

#include "Components/MaterialComponent.h"
#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"

namespace atlas
{
using namespace entityx;

RenderSystem::RenderSystem()
{
}

RenderSystem::~RenderSystem()
{
}

void RenderSystem::update(EntityManager& entities, EventManager& events, TimeDelta dt)
{
    entities.each<TransformComponent, MeshComponent, MaterialComponent>(
        [this, dt](Entity entity, TransformComponent& transform, MeshComponent& mesh, MaterialComponent& material) {
            updateEntity(entity, transform, mesh, material, dt);
        });
}

void RenderSystem::updateEntity(entityx::Entity entity, const TransformComponent& transform, const MeshComponent& mesh,
                                MaterialComponent& material, entityx::TimeDelta dt)
{
}

}  // namespace Atlas
