#pragma once

#include "Asset.h"
#include "Utils/BgfxUtils.h"
#include <bgfx/bgfx.h>

namespace atlas
{
enum class ShaderTypes
{
    None = 0,
    Vertex,
    Fragment,
    Compute
};

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
    // TODO shader is created on render thread, get rid of
    // shader memory afterwards
    MemoryWriter _memWriter;
};

namespace wren
{
    void bindShaderTypes();
}

}  // atlas namespace
