#include "Particles/ParticleSystem.h"
#include "Ecs/ECSManager.h"
#include "entt/entt.hpp"
#include "Particles/ParticleEffectComponent.h"
#include "Core/TransformComponent.h"

namespace atlas
{
ParticleSystem::ParticleSystem() { }

ParticleSystem::~ParticleSystem() { }

void ParticleSystem::update(float dt)
{
    auto view = ECSManager::instance().registry().view<TransformComponent, ParticleEffectComponent>();
    for (auto entity : view)
    {
        const TransformComponent& transform      = view.get<TransformComponent>(entity);
        ParticleEffectComponent&  particleEffect = view.get<ParticleEffectComponent>(entity);
    }
}

void ParticleSystem::runSystem(entt::registry& registry, float dt) { }

void ParticleSystem::ExecuteRange(enki::TaskSetPartition range_, uint32_t threadnum_) { }

}  // atlas namespace
