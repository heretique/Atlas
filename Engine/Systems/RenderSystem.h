#pragma once

#include <entityx/entityx.h>

namespace math
{
class Transform;
}

namespace atlas
{
struct MeshComponent;

class RenderSystem : public entityx::System<RenderSystem>
{
public:
    RenderSystem();
    ~RenderSystem();

    void update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt) override;

private:
    void updateEntity(entityx::Entity entity, math::Transform& transform, MeshComponent& meshComp,
                      entityx::TimeDelta dt);
};

}  // namespace atlas
