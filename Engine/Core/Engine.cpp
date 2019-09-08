#include "Engine.h"

#include "Assets/Geometry.h"
#include "Assets/Material.h"
#include "Assets/Script.h"
#include "Assets/Shader.h"
#include "Assets/Texture.h"
#include "Components/Camera.h"
#include "Components/MaterialComponent.h"
#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"
#include "Core/SerializationArchives.h"
#include "Core/SimpleMeshVertex.h"
#include "Managers/AssetManager.h"
#include "Managers/JobManager.h"
#include "Managers/PluginManager.h"
#include "entt/entity/registry.hpp"

#include <bgfx/bgfx.h>
#include <bx/allocator.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace atlas
{
spdlog::logger*        Engine::_logger        = nullptr;
PluginManager*         Engine::_pluginManager = nullptr;
AssetManager*          Engine::_assetManager  = nullptr;
entt::DefaultRegistry* Engine::_ecsManager    = nullptr;
JobManager*            Engine::_jobManager    = nullptr;

bgfx::VertexLayout SimpleMeshVertex::vertLayout;

bx::AllocatorI* Engine::bxAllocator()
{
    static bx::DefaultAllocator allocator;
    return &allocator;
}

const bgfx::Stats* Engine::bgfxStats()
{
    return bgfx::getStats();
}

bool Engine::init()
{
    _logger = spdlog::stdout_color_mt("console").get();

    if (_jobManager == nullptr)
        _jobManager = new JobManager();
    if (_pluginManager == nullptr)
        _pluginManager = new PluginManager();
    if (_assetManager == nullptr)
        _assetManager = new AssetManager();
    if (_ecsManager == nullptr)
        _ecsManager = new entt::DefaultRegistry();

    initVertDecl();
    registerDefaultAssetTypes();
    jobs().init();

    return true;
}

void Engine::initVertDecl()
{
    SimpleMeshVertex::init();
}

void Engine::registerDefaultAssetTypes()
{
    assets().registerAssetType(AssetTypes::Geometry, AssetNames::Geometry, GeometryAsset::factoryFunc);
    assets().registerAssetType(AssetTypes::Code, AssetNames::Code, ScriptAsset::factoryFunc);
    assets().registerAssetType(AssetTypes::Texture, AssetNames::Texture, TextureAsset::factoryFunc);
    assets().registerAssetType(AssetTypes::Shader, AssetNames::Shader, ShaderAsset::factoryFunc);
    assets().registerAssetType(AssetTypes::Material, AssetNames::Material, MaterialAsset::factoryFunc);
}

void Engine::release()
{
    jobs().release();

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
