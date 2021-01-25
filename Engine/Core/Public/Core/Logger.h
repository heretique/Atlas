#pragma once

#include <spdlog/spdlog.h>

namespace atlas
{

class Logger
{
public:
    static spdlog::logger& log();
};

} // atlas namespace
