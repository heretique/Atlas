#include "Rendering/RenderSystem.h"
#include "entt/entity/registry.hpp"
#include "DebugDraw/DebugDraw.h"

#include "Core/Component.h"
#include "Materials/MaterialComponent.h"
#include "Geometry/MeshComponent.h"
#include "Core/TransformComponent.h"
#include <Hq/Math/Math.h>
#include <Hq/Math/Vec3.h>
#include <Hq/Math/Quat.h>
#include <Hq/Math/Mat4x4.h>
#include <Hq/PackUtils.h>

namespace atlas
{
RenderSystem::RenderSystem() { }

RenderSystem::~RenderSystem() { }

void RenderSystem::render() { }

void RenderSystem::runSystem(entt::registry& registry, float dt)
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
        mul(transform.world, rot);

        bgfx_set_transform(transform.world.data, 1);
        bgfx_set_vertex_buffer(0, mesh.geometry->vbo(), 0, UINT32_MAX);
        bgfx_set_index_buffer(mesh.geometry->ibo(), 0, UINT32_MAX);
        material.material->bind();
        bgfx_set_state(BGFX_STATE_DEFAULT, 0);
        bgfx_submit(0, material.material->program(), 0, BGFX_DISCARD_ALL);
    }
    auto selectedView = registry.view<Selected>();
    for (auto entity : selectedView)
    {
        TransformComponent& transform = view.get<TransformComponent>(entity);
        DebugDraw::begin();
        DebugDraw::drawBox3(transform.bbox, transform.world, hq::packUint32(255, 255, 0, 255));
        DebugDraw::end();
    }
}

}  // namespace Atlas
