#include "Core/Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace atlas
{

spdlog::logger& Logger::log()
{
    static spdlog::logger* _instance = spdlog::stdout_color_mt("console").get();
    return *_instance;
}

} // atlas namespace
