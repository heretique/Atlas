#ifndef ENGINE_H
#define ENGINE_H

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
struct Stats;
}

namespace entt
{
template <typename>
class Registry;
using DefaultRegistry = Registry<std::uint32_t>;
}

namespace hq
{
class JobManager;
}

namespace atlas
{
// Forward declarations
class PluginManager;
class AssetManager;
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
    static entt::DefaultRegistry& ecs()
    {
        return *_ecsManager;
    }
    static hq::JobManager& jobs()
    {
        return *_jobManager;
    }
    static spdlog::logger& log()
    {
        return *_logger;
    }

    static bx::AllocatorI* bxAllocator();

    static const bgfx::Stats* bgfxStats();

private:
    static bool init();
    static void initVertDecl();
    static void registerDefaultAssetTypes();
    static void release();

private:
    static spdlog::logger*        _logger;
    static PluginManager*         _pluginManager;
    static AssetManager*          _assetManager;
    static entt::DefaultRegistry* _ecsManager;
    static hq::JobManager*        _jobManager;

    friend class MainWindow;
};
}  // namespace atlas

#endif  // ENGINE_H
