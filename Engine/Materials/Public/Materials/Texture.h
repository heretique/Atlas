#pragma once

#include "Assets/Asset.h"
#include <bgfx/c99/bgfx.h>

namespace atlas
{
class TextureAsset : public Asset
{
public:
    static AssetPtr factoryFunc(const std::string& filename, u32 flags);

    TextureAsset(const std::string& filename, u32 flags);
    ~TextureAsset() override;

    bgfx_texture_handle_t bgfxHandle() const;

    // Asset interface
protected:
    virtual bool loadImpl(std::istream& data) override;
    virtual bool isGPUResource() override;
    virtual bool uploadGPUImpl() override;

private:
    bgfx_texture_handle_t _texture;
    bgfx_texture_info_t   _textureInfo;
};

}  // atlas namespace
