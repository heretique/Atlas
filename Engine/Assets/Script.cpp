#include "Script.h"
#include <string>

namespace atlas
{
ScriptAsset::ScriptAsset(const std::string& name, u32 flags)
    : Asset((int)AssetTypes::Code, name, flags)
{
}

ScriptAsset::~ScriptAsset()
{
}

bool ScriptAsset::load(const std::istream& data)
{
    return false;
}

AssetPtr ScriptAsset::clone() const
{
    return nullptr;
}

}  // atlas
