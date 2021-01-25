#pragma once

#include <string>
#include <Hq/BasicTypes.h>
#include "Assets/AssetManager.h"
#include "Input/InputManager.h"
#include "Ecs/ECSManager.h"
#include "UI/UIManager.h"
#include "enkiTS/TaskScheduler.h"
#include "Plugins/PluginManager.h"

typedef struct bgfx_caps_s  bgfx_caps_t;
typedef struct bgfx_stats_s bgfx_stats_t;

namespace enki
{
class TaskScheduler;
}

namespace atlas
{
class Engine
{
public:
    static PluginManager& plugins()
    {
        return PluginManager::instance();
    }
    static AssetManager& assets()
    {
        return AssetManager::instance();
    }
    static InputManager& input()
    {
        return InputManager::instance();
    }
    static ECSManager& ecs()
    {
        return ECSManager::instance();
    }
    static enki::TaskScheduler& jobs()
    {
        return *_jobManager;
    }

    static UIManager& ui()
    {
        return UIManager::instance();
    }

    static const bgfx_caps_t*  bgfxCaps();
    static const bgfx_stats_t* bgfxStats();

    static bool init(u32 viewWidth, u32 viewHeight);
    static void setViewSize(u32 width, u32 height);
    static u32  viewWidth();
    static u32  viewHeight();

private:
    static void initVertDecl();
    static void registerComponentDependencies();
    static void registerDefaultAssetTypes();
    static void registerSystems();
    static void release();

private:
    static enki::TaskScheduler* _jobManager;
    static u32                  _viewWidth;
    static u32                  _viewHeight;
};
}  // namespace atlas
