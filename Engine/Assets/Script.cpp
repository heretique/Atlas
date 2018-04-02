#include "Script.h"
#include <string>

namespace atlas
{
ScriptAsset::ScriptAsset(const std::string& name, u32 flags)
    : Asset(AssetTypes::Code, name, flags)
{
}

ScriptAsset::~ScriptAsset() {}

bool ScriptAsset::loadImpl(std::istream& data)
{
    _buffer = std::string(std::istreambuf_iterator<char>(data.rdbuf()), {});
    return true;
}

const std::string& ScriptAsset::script() const
{
    return _buffer;
}

}  // atlas
