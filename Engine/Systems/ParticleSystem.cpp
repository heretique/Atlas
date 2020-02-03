#include "ParticleSystem.h"
#include "Core/Engine.h"
#include "entt/entt.hpp"
#include "Components/ParticleEffectComponent.h"
#include "Components/TransformComponent.h"

namespace atlas
{

ParticleSystem::ParticleSystem()
{

}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::update(float dt)
{
    auto view = Engine::ecs().view<TransformComponent, ParticleEffectComponent>();
    for (auto entity: view)
    {
        const TransformComponent& transform = view.get<TransformComponent>(entity);
        ParticleEffectComponent& particleEffect = view.get<ParticleEffectComponent>(entity);
    }
}

} // atlas namespace
