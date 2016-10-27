#include "LogManager.h"
#include <stdarg.h>
#include <stdio.h>

namespace atlas {

LogManager::LogManager()
{

}

LogManager::~LogManager()
{

}

void LogManager::registerLoggerType(int type, const std::string &typeString, LogTypeInitializationFunc inf,
                                    LogTypeReleaseFunc rf, LogTypeFactoryFunc ff)
{
    LoggerRegEntry entry;
    entry.typeString = typeString;
    entry.initializationFunc = inf;
    entry.releaseFunc = rf;
    entry.factoryFunc = ff;
    _registry[type] = entry;

    // Initialize resource type
    if(inf) inf();
}

void LogManager::writeInfo(const char *msg, ...)
{
    // Temporary

    va_list args;
    va_start( args, msg );
    vfprintf(stdout, msg, args);
    va_end( args );
}

void LogManager::writeWarning(const char *msg, ...)
{
    // Temporary

    va_list args;
    va_start( args, msg );
    vfprintf(stdout, msg, args);
    va_end( args );
}

void LogManager::writeError(const char *msg, ...)
{
    // Temporary

    va_list args;
    va_start( args, msg );
    vfprintf(stderr, msg, args);
    va_end( args );
}

} // atlas


