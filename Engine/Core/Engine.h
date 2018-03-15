#ifndef ENGINE_H
#define ENGINE_H

namespace spdlog
{
class logger;
}

namespace wrenpp
{
class VM;
}

namespace atlas
{
// Forward declarations
class PluginManager;
class AssetManager;
class ECSManager;
class AudioManager;
class PhysicsManager;
class JobManager;
class SceneManager;
class MainWindow;

class Engine
{
public:
    static PluginManager& plugins()
    {
        return *_pluginManager;
    }
    static AssetManager& assets()
    {
        return *_assetManager;
    }
    static SceneManager& scene()
    {
        return *_sceneManager;
    }
    static ECSManager& ecs()
    {
        return *_ecsManager;
    }
    static JobManager& jobs()
    {
        return *_jobManager;
    }
    static spdlog::logger& log()
    {
        return *_logger;
    }
    static wrenpp::VM& vm()
    {
        return *_vm;
    }

private:
    static bool init();
    static void initVertDecl();
    static void initVM();
    static void registerAssetTypes();
    static void release();

private:
    static spdlog::logger* _logger;
    static PluginManager*  _pluginManager;
    static AssetManager*   _assetManager;
    static SceneManager*   _sceneManager;
    static ECSManager*     _ecsManager;
    static JobManager*     _jobManager;
    static wrenpp::VM*     _vm;

    friend class MainWindow;
};

namespace wren
{
    void bindEngine();
}

}  // namespace atlas

#endif  // ENGINE_H
