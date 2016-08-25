#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <entityx/entityx.h>

namespace math {
 class Transform;
}

namespace atlas {

class MeshComponent;

class RenderSystem : public entityx::System<RenderSystem>
{
public:
    RenderSystem();
    ~RenderSystem();

    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override;

private:
    void updateEntity(entityx::Entity entity, math::Transform &transform, MeshComponent &meshComp, entityx::TimeDelta dt);
};

} // namespace atlas


#endif // RENDERSYSTEM_H
