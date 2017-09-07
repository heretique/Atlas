#include "Managers/LogManager.h"
#include <stdarg.h>

namespace atlas
{
std::map<std::string, LoggerRegEntry> LogManager::_registry = std::map<std::string, LoggerRegEntry>();
std::map<std::string, ILogger*>       LogManager::_loggers  = std::map<std::string, ILogger*>();

void LogManager::registerLoggerType(const std::string& name, LogTypeFactoryFunc ff, LogTypeReleaseFunc rf)
{
    LoggerRegEntry entry;
    entry.typeString  = name;
    entry.releaseFunc = rf;
    entry.factoryFunc = ff;
    _registry[name]   = entry;
}

void LogManager::init(std::string logger, int flags)
{
    auto it = _registry.find(logger);
    if (it != _registry.end())
        _loggers[logger] = (*it).second.factoryFunc(flags);
}

void LogManager::release()
{
    for (auto& any : _loggers)
    {
        auto it = _registry.find(any.first);
        if (it != _registry.end())
            (*it).second.releaseFunc(any.second);
    }
    _loggers.clear();
}

void LogManager::writeInfo(const char* msg, ...)
{
    for (auto& any : _loggers)
    {
        va_list args;
        va_start(args, msg);
        any.second->writeInfo(msg, args);
        va_end(args);
    }
}

void LogManager::writeWarning(const char* msg, ...)
{
    for (auto& any : _loggers)
    {
        va_list args;
        va_start(args, msg);
        any.second->writeWarning(msg, args);
        va_end(args);
    }
}

void LogManager::writeError(const char* msg, ...)
{
    for (auto& any : _loggers)
    {
        va_list args;
        va_start(args, msg);
        any.second->writeError(msg, args);
        va_end(args);
    }
}

}  // atlas
