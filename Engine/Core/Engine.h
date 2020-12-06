#pragma once

#include <string>
#include <Hq/BasicTypes.h>

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
class MainWindow;
class DebugDraw;
class UIManager;

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
    static DebugDraw& debugDraw()
    {
        return *_debugDraw;
    }
    static UIManager& ui()
    {
        return *_uiManager;
    }


    static bx::AllocatorI*    bxAllocator();
    static const bgfx::Caps*  bgfxCaps();
    static const bgfx::Stats* bgfxStats();

    static bool init(u32 viewWidth, u32 viewHeight);
    static void setViewSize(u32 width, u32 height);
    static u32 viewWidth();
    static u32 viewHeight();

private :
    static void initVertDecl();
    static void registerComponentDependencies();
    static void registerDefaultAssetTypes();
    static void registerSystems();
    static void release();

private:
    static spdlog::logger*      _logger;
    static PluginManager*       _pluginManager;
    static AssetManager*        _assetManager;
    static InputManager*        _inputManager;
    static ECSManager*          _ecsManager;
    static enki::TaskScheduler* _jobManager;
    static UIManager*           _uiManager;
    static DebugDraw*           _debugDraw;
    static u32                  _viewWidth;
    static u32                  _viewHeight;
};
}  // namespace atlas
