#pragma once

#include <string>

namespace spdlog
{
class logger;
}
namespace bx
{
struct AllocatorI;
}

namespace bgfx
{
struct Caps;
struct Stats;
}

namespace enki
{
class TaskScheduler;
}

namespace atlas
{
// Forward declarations
class PluginManager;
class AssetManager;
class ECSManager;
class InputManager;
class AudioManager;
class PhysicsManager;
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
    static InputManager& input()
    {
        return *_inputManager;
    }
    static ECSManager& ecs()
    {
        return *_ecsManager;
    }
    static enki::TaskScheduler& jobs()
    {
        return *_jobManager;
    }
    static spdlog::logger& log()
    {
        return *_logger;
    }

    static bx::AllocatorI* bxAllocator();
    static const bgfx::Caps* bgfxCaps();
    static const bgfx::Stats* bgfxStats();

private:
    static bool init();
    static void initVertDecl();
    static void registerDefaultAssetTypes();
    static void release();

private:
    static spdlog::logger*      _logger;
    static PluginManager*       _pluginManager;
    static AssetManager*        _assetManager;
    static InputManager*        _inputManager;
    static ECSManager*          _ecsManager;
    static enki::TaskScheduler* _jobManager;

    friend class MainWindow;
};
}  // namespace atlas
