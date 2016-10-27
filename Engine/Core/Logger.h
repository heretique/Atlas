#ifndef LOGGER
#define LOGGER

#include <cstddef>
#include <cstdarg>

namespace atlas {

struct SourceInfo {

};

class Logger
{
  public:
  virtual ~Logger(void) {}

  virtual void Log(size_t channel, size_t type, size_t verbosity, const SourceInfo& SourceInfo, const char* format, va_list args) = 0;
};


class ILogger {
public:
    virtual ~ILogger() {}
    virtual void writeInfo(const char *msg, ...) = 0;
    virtual void writeWarning(const char *msg, ...) = 0;
    virtual void writeError(const char *msg, ...) = 0;
};

} // atlas

#endif // LOGGER

