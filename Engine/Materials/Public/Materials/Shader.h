#pragma once

#include "Assets/Asset.h"
#include "Utils/BgfxUtils.h"
#include <bgfx/c99/bgfx.h>

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

    bgfx_shader_handle_t bgfxHandle() const;

    // Asset interface
protected:
    virtual bool loadImpl(std::istream& data) override;
    virtual bool isGPUResource() override;
    virtual bool uploadGPUImpl() override;

private:
    bgfx_shader_handle_t _shader;
    // TODO shader is created on render thread, get rid of
    // shader memory afterwards
    std::string _shaderBuffer;
};

}  // atlas namespace
