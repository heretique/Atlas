#ifndef ENGINE_H
#define ENGINE_H

namespace spdlog
{
class logger;
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

private:
    static bool init();
    static void release();

private:
    static spdlog::logger* _logger;
    static PluginManager*  _pluginManager;
    static AssetManager*   _assetManager;
    static ECSManager*     _ecsManager;
    static JobManager*     _jobManager;

    friend class MainWindow;
};

}  // namespace atlas

#endif  // ENGINE_H
