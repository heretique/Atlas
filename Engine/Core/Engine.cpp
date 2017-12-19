#include "Engine.h"

#include "Assets/Geometry.h"
#include "Assets/Script.h"
#include "Managers/AssetManager.h"
#include "Managers/ECSManager.h"
#include "Managers/JobManager.h"
#include "Managers/PluginManager.h"
#include "Scripting/WrenBindings.h"

#include <spdlog/spdlog.h>
#include <wrenpp/Wren++.h>

namespace atlas
{
spdlog::logger* Engine::_logger        = nullptr;
PluginManager*  Engine::_pluginManager = nullptr;
AssetManager*   Engine::_assetManager  = nullptr;
ECSManager*     Engine::_ecsManager    = nullptr;
JobManager*     Engine::_jobManager    = nullptr;
wrenpp::VM*     Engine::_vm            = nullptr;

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

    // initialize wren vm
    wrenpp::VM::loadModuleFn = [](const char* mod) -> char* {
        std::string path(mod);
        path += ".wren";
        auto scriptAsset = Engine::assets().getAsset<ScriptAsset>(path);
        if (scriptAsset != nullptr)
        {
            auto  source = scriptAsset->script();
            char* buffer = (char*)malloc(source.size() + 1);
            assert(buffer != NULL);
            buffer[source.size()] = '\0';
            memcpy(buffer, source.c_str(), source.size());
            return buffer;
        }

        return NULL;
    };
    wrenpp::VM::writeFn = [](const char* text) -> void {
        // this hack exists because Wren always prints an extra newline as a separate print statement.
        if (text[0] != '\n')
        {
            Engine::log().info(text);
        }
    };

    if (_vm == nullptr)
        _vm = new wrenpp::VM();

    assets().registerAssetType(AssetTypes::Geometry, GeometryAsset::factoryFunc);
    assets().registerAssetType(AssetTypes::Code, ScriptAsset::factoryFunc);

    jobs().init();

    // bind wren modules
    wren::bindImguiModule();

    return true;
}

void Engine::release()
{
    jobs().release();

    delete _jobManager;
    _jobManager = nullptr;

    delete _vm;
    _vm = nullptr;

    delete _ecsManager;
    _ecsManager = nullptr;

    delete _assetManager;
    _assetManager = nullptr;

    delete _pluginManager;
    _pluginManager = nullptr;
}

}  // namespace Atlas
