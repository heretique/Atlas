#pragma once

#include "Resources/Asset.h"
#include "wrenpp/Wren++.h"

namespace atlas
{
class ScriptAsset : public Asset
{
public:
    static AssetPtr factoryFunc(const std::string& name, u32 flags)
    {
        return std::make_shared<ScriptAsset>(name, flags);
    }

    ScriptAsset(const std::string& name, u32 flags);
    virtual ~ScriptAsset() override;

    virtual bool load(const std::istream& data) override;
    virtual AssetPtr clone() const override;

private:
};

}  // atlas
