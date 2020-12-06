#include "Engine.h"

#include "Assets/Geometry.h"
#include "Assets/Material.h"
#include "Assets/Script.h"
#include "Assets/Shader.h"
#include "Assets/Texture.h"
#include "Assets/ParticleEffect.h"
#include "Components/Camera.h"
#include "Components/Common.h"
#include "Components/MaterialComponent.h"
#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"
#include "Core/SimpleMeshVertex.h"
#include "Managers/AssetManager.h"
#include "Managers/InputManager.h"
#include "Managers/ECSManager.h"
#include "Managers/UIManager.h"
#include "Systems/AnimationSystem.h"
#include "Systems/ParticleSystem.h"
#include "Systems/PickingSystem.h"
#include "Systems/RenderSystem.h"
#include "enkiTS/TaskScheduler.h"
#include "Managers/PluginManager.h"
#include "Utils/DebugDraw.h"
#include "Utils/ImGuiSerializer.h"
#include "Hq/JsonSerializer.h"
#include "Hq/BinarySerializer.h"

#include <entt/entt.hpp>

#include <bgfx/bgfx.h>
#include <bx/allocator.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace atlas
{
spdlog::logger*      Engine::_logger        = nullptr;
PluginManager*       Engine::_pluginManager = nullptr;
AssetManager*        Engine::_assetManager  = nullptr;
InputManager*        Engine::_inputManager  = nullptr;
ECSManager*          Engine::_ecsManager    = nullptr;
UIManager*           Engine::_uiManager     = nullptr;
enki::TaskScheduler* Engine::_jobManager    = nullptr;
DebugDraw*           Engine::_debugDraw     = nullptr;
u32                  Engine::_viewWidth     = 0;
u32                  Engine::_viewHeight    = 0;

bgfx::VertexLayout SimpleMeshVertex::vertLayout;

///////////// Bounds updates //////////////
void UpdateEntityBoundsFromMesh(entt::registry& registry, entt::entity entity)
{
    if (registry.has<TransformComponent>(entity))
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

bx::AllocatorI* Engine::bxAllocator()
{
    static bx::DefaultAllocator allocator;
    return &allocator;
}

const bgfx::Caps* Engine::bgfxCaps()
{
    return bgfx::getCaps();
}

const bgfx::Stats* Engine::bgfxStats()
{
    return bgfx::getStats();
}

bool Engine::init(u32 viewWidth, u32 viewHeight)
{
    _viewWidth  = viewWidth;
    _viewHeight = viewHeight;

    _logger = spdlog::stdout_color_mt("console").get();

    if (_jobManager == nullptr)
        _jobManager = new enki::TaskScheduler();
    if (_pluginManager == nullptr)
        _pluginManager = new PluginManager();
    if (_assetManager == nullptr)
        _assetManager = new AssetManager();
    if (_inputManager == nullptr)
        _inputManager = new InputManager();
    if (_ecsManager == nullptr)
        _ecsManager = new ECSManager();
    if (_debugDraw == nullptr)
        _debugDraw = new DebugDraw();
    if (_uiManager == nullptr)
        _uiManager = new UIManager();

    initVertDecl();
    registerDefaultAssetTypes();
    registerComponentDependencies();
    registerSystems();
    jobs().Initialize();
    debugDraw().initialize();

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
    ecs().registerUpdateSystem(std::move(std::make_unique<PickingSystem>()));

    ecs().registerVisualSystem(std::move(std::make_unique<RenderSystem>()));
}

void Engine::release()
{
    jobs().WaitforAllAndShutdown();

    delete _uiManager;
    _uiManager = nullptr;

    delete _ecsManager;
    _ecsManager = nullptr;

    delete _assetManager;
    _assetManager = nullptr;

    delete _pluginManager;
    _pluginManager = nullptr;

    delete _jobManager;
    _jobManager = nullptr;
}
}  // namespace Atlas
