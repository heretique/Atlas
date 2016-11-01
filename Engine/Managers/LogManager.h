#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "Base.h"
#include "Logger.h"

namespace atlas {

typedef std::function<ILogger* (int)> LogTypeFactoryFunc;
typedef std::function<void (ILogger*)> LogTypeReleaseFunc;

struct LoggerRegEntry
{
    std::string                typeString;
    LogTypeFactoryFunc         factoryFunc;  // Factory to create resource object
    LogTypeReleaseFunc         releaseFunc;  // Called when type is unregistered
};

class LogManager
{
public:
    static void registerLoggerType(const string &name, LogTypeFactoryFunc ff,
                                   LogTypeReleaseFunc rf);
    static void init(string logger, int flags);
    static void writeInfo(const char *msg, ...);
    static void writeWarning(const char *msg, ...);
    static void writeError(const char *msg, ...);
    static void release();
private:
    static map<string, LoggerRegEntry> _registry;
    static map<string, ILogger*> _loggers;
};


#ifdef ATLAS_ENABLE_LOGGING
#define LOGINFO(x, ...) LogManager::writeInfo(x, ##__VA_ARGS__)
#define LOGWARNING(x, ...) LogManager::writeWarning(x, ##__VA_ARGS__)
#define LOGERROR(x, ...) LogManager::writeError(x, ##__VA_ARGS__)
#else
#define LOGINFO(x, ...)
#define LOGWARNING(x, ...)
#define LOGERROR(x, ...)
#endif


} // namespace atlas



#endif // LOGMANAGER_H
