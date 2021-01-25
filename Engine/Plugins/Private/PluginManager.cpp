#include "Plugins/PluginManager.h"

namespace atlas
{
PluginManager::PluginManager() { }

PluginManager& PluginManager::instance()
{
    static PluginManager _instance;
    return _instance;
}

}  // atlas
