#include "Engine.h"
#include "LogManager.h"
#include "PluginManager.h"
#include "AssetManager.h"
#include "ProfilingManager.h"
#include "ECSManager.h"
#include "Resources/Geometry.h"
#include "Resources/Script.h"
#include "JobManager.h"

namespace atlas {

ProfilingManager    *Engine::_profilingManager = 0x0;
PluginManager       *Engine::_pluginManager = 0x0;
AssetManager        *Engine::_assetManager = 0x0;
ECSManager          *Engine::_ecsManager = 0x0;
JobManager          *Engine::_jobManager = 0x0;



class StdoutLogger: ILogger {
public:
    static ILogger* FactoryFunc(int flags) { return new StdoutLogger; }
    static void ReleaseFunc(ILogger* logger) { delete logger; }

    virtual ~StdoutLogger() override {}
    virtual void writeInfo(const char *msg, ...) override
    {
        va_list args;
        va_start( args, msg );
        vfprintf(stdout, msg, args);
        va_end( args );
    }

    virtual void writeWarning(const char *msg, ...) override
    {
        va_list args;
        va_start( args, msg );
        vfprintf(stdout, msg, args);
        va_end( args );
    }

    virtual void writeError(const char *msg, ...) override
    {
        va_list args;
        va_start( args, msg );
        vfprintf(stderr, msg, args);
        va_end( args );
    }
};




bool Engine::init()
{
    LogManager::registerLoggerType("stdout", StdoutLogger::FactoryFunc, StdoutLogger::ReleaseFunc);
    LogManager::init("stdout", 0);

    if (_profilingManager == 0x0) _profilingManager = new ProfilingManager();
    if (_pluginManager == 0x0) _pluginManager = new PluginManager();
    if (_assetManager == 0x0) _assetManager = new AssetManager();
    if (_ecsManager == 0x0) _ecsManager = new ECSManager();
    if (_jobManager == 0x0) _jobManager = new JobManager();


    assets().registerAssetType( (int)AssetTypes::Geometry, "Geometry", GeometryAsset::factoryFunc,
                                GeometryAsset::releaseFunc);
    assets().registerAssetType( (int)AssetTypes::Code, "Code", ScriptAsset::factoryFunc,
                                ScriptAsset::releaseFunc);

    jobMan().init();

    return true;
}


void Engine::release()
{
    jobMan().release();
    LogManager::release();
}

} // namespace Atlas


