#include "Engine/Engine.h"

#include "Geometry/Geometry.h"
#include "Materials/Material.h"
#include "Scripting/Script.h"
#include "Materials/Shader.h"
#include "Materials/Texture.h"
#include "Particles/ParticleEffect.h"
#include "Camera/Camera.h"
#include "Core/Component.h"
#include "Materials/MaterialComponent.h"
#include "Geometry/MeshComponent.h"
#include "Core/TransformComponent.h"
#include "Core/SimpleMeshVertex.h"
#include "Animation/AnimationSystem.h"
#include "Particles/ParticleSystem.h"
#include "Picking/PickingSystem.h"
#include "Rendering/RenderSystem.h"
#include "Plugins/PluginManager.h"
#include "UI/ImGuiSerializer.h"
#include "Hq/JsonSerializer.h"
#include "Hq/BinarySerializer.h"
#include "DebugDraw/DebugDraw.h"
#include "Core/MemoryManager.h"

#include <entt/entt.hpp>

#include <bgfx/c99/bgfx.h>

namespace atlas
{
enki::TaskScheduler* Engine::_jobManager = nullptr;
u32                  Engine::_viewWidth  = 0;
u32                  Engine::_viewHeight = 0;

///////////// Bounds updates //////////////
void UpdateEntityBoundsFromMesh(entt::registry& registry, entt::entity entity)
{
    if (registry.all_of<TransformComponent>(entity))
    {
        TransformComponent& transform = registry.get<TransformComponent>(entity);
        transform.bbox                = registry.get<MeshComponent>(entity).geometry->bounds();
    }
    else
    {
        TransformComponent& transform = registry.emplace<TransformComponent>(entity);
        transform.bbox                = registry.get<MeshComponent>(entity).geometry->bounds();
    }
}

const bgfx_caps_t* Engine::bgfxCaps()
{
    return bgfx_get_caps();
}

const bgfx_stats_t* Engine::bgfxStats()
{
    return bgfx_get_stats();
}

bool Engine::init(u32 viewWidth, u32 viewHeight)
{
    _viewWidth  = viewWidth;
    _viewHeight = viewHeight;

    if (_jobManager == nullptr)
        _jobManager = new enki::TaskScheduler();

    initVertDecl();
    registerDefaultAssetTypes();
    registerComponentDependencies();
    registerSystems();
    jobs().Initialize();
    DebugDraw::initialize();

    return true;
}

void Engine::setViewSize(u32 width, u32 height)
{
    _viewWidth  = width;
    _viewHeight = height;
}

u32 Engine::viewWidth()
{
    return _viewWidth;
}

u32 Engine::viewHeight()
{
    return _viewHeight;
}

void Engine::initVertDecl()
{
    SimpleMeshVertex::init();
}

void Engine::registerComponentDependencies()
{
    ecs().registry().on_construct<MeshComponent>().connect<&UpdateEntityBoundsFromMesh>();
}

void Engine::registerDefaultAssetTypes()
{
    assets().registerAssetType(AssetTypes::Geometry, AssetNames::Geometry, GeometryAsset::factoryFunc);
    assets().registerAssetType(AssetTypes::Code, AssetNames::Code, ScriptAsset::factoryFunc);
    assets().registerAssetType(AssetTypes::Texture, AssetNames::Texture, TextureAsset::factoryFunc);
    assets().registerAssetType(AssetTypes::Shader, AssetNames::Shader, ShaderAsset::factoryFunc);
    assets().registerAssetType(AssetTypes::Material, AssetNames::Material, MaterialAsset::factoryFunc);
    assets().registerAssetType(AssetTypes::ParticleEffect, AssetNames::ParticleEffect,
                               ParticleEffectAsset::factoryFunc);
}

void Engine::registerSystems()
{
    ecs().registerUpdateSystem(std::move(std::make_unique<AnimationSystem>()));
    ecs().registerUpdateSystem(std::move(std::make_unique<ParticleSystem>()));

    ecs().registerVisualSystem(std::move(std::make_unique<RenderSystem>()));
}

void Engine::release()
{
    jobs().WaitforAllAndShutdown();

    delete _jobManager;
    _jobManager = nullptr;

    DebugDraw::release();
    ui().release();
    ecs().registry().clear();
    assets().release();
}
}  // namespace Atlas
