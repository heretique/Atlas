#ifndef ENGINE_H
#define ENGINE_H

namespace atlas {

// Forward declarations
class PluginManager;
class ResourceManager;
class LogManager;
class ECSManager;
class AudioManager;
class PhysicsManager;
class ProfilingManager;
class JobManager;

class Engine
{
public:
    static bool init();
    static void release();

    static LogManager &log() { return *_logManager; }
    static ProfilingManager &profiler() { return *_profilingManager; }
    static PluginManager &plugMan() { return *_pluginManager; }
    static ResourceManager &resMan() { return *_resourceManager; }
    static ECSManager &ecs() { return *_ecsManager; }
    static JobManager &jobMan() { return *_jobManager; }

private:
    static LogManager       *_logManager;
    static ProfilingManager *_profilingManager;
    static PluginManager    *_pluginManager;
    static ResourceManager  *_resourceManager;
    static ECSManager       *_ecsManager;
    static JobManager       *_jobManager;
};

} // namespace atlas

#endif // ENGINE_H
