#include "Engine.h"
#include "Assets/Geometry.h"
#include "Assets/Script.h"
#include "Managers/AssetManager.h"
#include "Managers/ECSManager.h"
#include "Managers/JobManager.h"
#include "Managers/PluginManager.h"
#include <spdlog/spdlog.h>

namespace atlas
{
spdlog::logger* Engine::_logger        = nullptr;
PluginManager*  Engine::_pluginManager = nullptr;
AssetManager*   Engine::_assetManager  = nullptr;
ECSManager*     Engine::_ecsManager    = nullptr;
JobManager*     Engine::_jobManager    = nullptr;

bool Engine::init()
{
    _logger = spdlog::stdout_color_mt("console").get();

    if (_pluginManager == nullptr)
        _pluginManager = new PluginManager();
    if (_assetManager == nullptr)
        _assetManager = new AssetManager();
    if (_ecsManager == nullptr)
        _ecsManager = new ECSManager();
    if (_jobManager == nullptr)
        _jobManager = new JobManager();

    assets().registerAssetType((int)AssetTypes::Geometry, "Geometry", GeometryAsset::factoryFunc);
    assets().registerAssetType((int)AssetTypes::Code, "Code", ScriptAsset::factoryFunc);

    jobs().init();

    return true;
}

void Engine::release()
{
    jobs().release();

    delete _jobManager;
    _jobManager = nullptr;

    delete _ecsManager;
    _ecsManager = nullptr;

    delete _assetManager;
    _assetManager = nullptr;

    delete _pluginManager;
    _pluginManager = nullptr;
}

}  // namespace Atlas
