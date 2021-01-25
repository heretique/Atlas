#pragma once

#include "Assets/Asset.h"

namespace atlas
{
class ScriptAsset : public Asset
{
public:
    static AssetPtr factoryFunc(const std::string& name, u32 flags)
    {
        return std::make_shared<ScriptAsset>(name, flags);
    }

    ScriptAsset(const std::string& filename, u32 flags);
    ~ScriptAsset() override;

    bool loadImpl(std::istream& data) override;

    const std::string& script() const;

private:
    std::string _buffer{};
};

}  // atlas
