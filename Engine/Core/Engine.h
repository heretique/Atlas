#ifndef ENGINE_H
#define ENGINE_H

namespace atlas {

// Forward declarations
class PluginManager;
class AssetManager;
class ECSManager;
class AudioManager;
class PhysicsManager;
class ProfilingManager;
class JobManager;

class Engine {
public:
  static bool init();
  static void release();

  static ProfilingManager &profiler() { return *_profilingManager; }
  static PluginManager &plugMan() { return *_pluginManager; }
  static AssetManager &assets() { return *_assetManager; }
  static ECSManager &ecs() { return *_ecsManager; }
  static JobManager &jobMan() { return *_jobManager; }

private:
  static ProfilingManager *_profilingManager;
  static PluginManager *_pluginManager;
  static AssetManager *_assetManager;
  static ECSManager *_ecsManager;
  static JobManager *_jobManager;
};

} // namespace atlas

#endif // ENGINE_H
