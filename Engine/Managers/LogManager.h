#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "Logger.h"

#include <functional>
#include <map>

namespace atlas {

typedef std::function<void ()> LogTypeInitializationFunc;
typedef std::function<void ()> LogTypeReleaseFunc;
typedef std::function<ILogger* (const std::string &name, int flags)> LogTypeFactoryFunc;

struct LoggerRegEntry
{
    std::string                typeString;
    LogTypeInitializationFunc  initializationFunc;  // Called when type is registered
    LogTypeReleaseFunc         releaseFunc;  // Called when type is unregistered
    LogTypeFactoryFunc         factoryFunc;  // Factory to create resource object
};

class LogManager : public ILogger
{
public:
    LogManager();
    ~LogManager();

    void registerLoggerType(int type, const std::string &typeString, LogTypeInitializationFunc inf,
                         LogTypeReleaseFunc rf, LogTypeFactoryFunc ff);

    void writeInfo(const char *msg, ...) override;
    void writeWarning(const char *msg, ...)override;
    void writeError(const char *msg, ...) override;

private:
    std::map<int, LoggerRegEntry> _registry;
};

} // namespace atlas



#endif // LOGMANAGER_H
