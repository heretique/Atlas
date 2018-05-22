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

namespace bx
{
struct AllocatorI;
}

namespace atlas
{
// Forward declarations
class PluginManager;
class AssetManager;
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
    static bx::AllocatorI* bxAllocator();

private:
    static bool init();
    static void initVertDecl();
    static void initVM();
    static void registerAssetTypes();
    static void registerNodeTypes();
    static void release();

private:
    static spdlog::logger* _logger;
    static PluginManager*  _pluginManager;
    static AssetManager*   _assetManager;
    static SceneManager*   _sceneManager;
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
