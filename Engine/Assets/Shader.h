#pragma once

#include "Asset.h"
#include <bgfx/bgfx.h>

namespace atlas
{
class ShaderAsset : public Asset
{
public:
    static AssetPtr factoryFunc(const std::string& filename, u32 flags);

    ShaderAsset(const std::string& filename, u32 flags);
    ~ShaderAsset() override;

    // Asset interface
protected:
    virtual bool loadImpl(const std::istream& data) override;
    virtual bool isGPUResource() override;
    virtual bool uploadGPUImpl() override;

private:
    bgfx::ShaderHandle _shader;
};
}  // atlas namespace
