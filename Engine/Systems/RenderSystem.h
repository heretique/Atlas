#pragma once

#include <entityx/entityx.h>

namespace atlas
{
class TransformComponent;
class MeshComponent;
class MaterialComponent;

class RenderSystem : public entityx::System<RenderSystem>
{
public:
    RenderSystem();
    ~RenderSystem();

    void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;

private:
    void updateEntity(entityx::Entity entity, const TransformComponent& transform, const MeshComponent& mesh,
                      MaterialComponent& material, entityx::TimeDelta dt);
};

}  // namespace atlas
