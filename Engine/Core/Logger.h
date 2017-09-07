#pragma once

namespace atlas
{
class ILogger
{
public:
    virtual ~ILogger()
    {
    }
    virtual void writeInfo(const char* msg, ...)    = 0;
    virtual void writeWarning(const char* msg, ...) = 0;
    virtual void writeError(const char* msg, ...)   = 0;
};

}  // atlas
