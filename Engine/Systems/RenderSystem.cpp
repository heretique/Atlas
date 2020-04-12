#include "Systems/RenderSystem.h"
#include "entt/entity/registry.hpp"
#include "Core/Engine.h"
#include "Utils/DebugDraw.h"

#include "Components/Common.h"
#include "Components/MaterialComponent.h"
#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"
#include <Hq/Math/Math.h>
#include <Hq/Math/Vec3.h>
#include <Hq/Math/Quat.h>
#include <Hq/Math/Mat4x4.h>
#include <Hq/PackUtils.h>

namespace atlas
{

RenderSystem::RenderSystem()
{

}

RenderSystem::~RenderSystem()
{

}

void RenderSystem::render()
{

}

void RenderSystem::runSystem(entt::registry &registry, float dt)
{
    using namespace hq;
    using namespace hq::math;

    auto view = registry.view<TransformComponent, MeshComponent, MaterialComponent>();
    for (auto entity : view)
    {
        TransformComponent& transform = view.get<TransformComponent>(entity);
        MeshComponent&      mesh      = view.get<MeshComponent>(entity);
        MaterialComponent&  material  = view.get<MaterialComponent>(entity);

        Vec3 forward(0.f, 0.f, 1.f);
        Vec3 right(1.f, 0.f, 0.f);
        Vec3 up(0.f, 1.f, 0.f);

        Quat rotZ = createFromAxisAngle(forward, 10 * kDegToRad * dt);
        Quat rotY = createFromAxisAngle(up, 10 * kDegToRad * dt);
        Quat rotX = createFromAxisAngle(right, 10 * kDegToRad * dt);

        Mat4x4 rot;
        createRotation(rotZ * rotY * rotX, rot);
        mul(transform.world(), rot);

        bgfx::setTransform(transform.world().data);
        bgfx::setVertexBuffer(0, mesh.geometry()->vbo());
        bgfx::setIndexBuffer(mesh.geometry()->ibo());
        material.material()->bind();
        bgfx::setState(BGFX_STATE_DEFAULT);
        bgfx::submit(0, material.material()->program());
    }
    auto selectedView = registry.view<Selected>();
    for (auto entity: selectedView)
    {
        TransformComponent& transform =view.get<TransformComponent>(entity);
        Engine::debugDraw().begin();
        Engine::debugDraw().drawBox3(transform.bounds(), transform.world(), hq::packUint32(255, 255, 0, 255));
        Engine::debugDraw().end();
    }
}

}  // namespace Atlas
