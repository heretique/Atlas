#pragma once

#include "Asset.h"
#include <bgfx/bgfx.h>

namespace atlas
{
class TextureAsset : public Asset
{
public:
    static AssetPtr factoryFunc(const std::string& filename, u32 flags);

    TextureAsset(const std::string& filename, u32 flags);
    ~TextureAsset() override;

    // Asset interface
protected:
    virtual bool loadImpl(const std::istream& data) override;
    virtual bool isGPUResource() override;
    virtual bool uploadGPUImpl() override;

private:
    bgfx::TextureHandle _texture;
    bgfx::TextureInfo   _textureInfo;
};

namespace wren
{
    void bindTextureTypes();
}

}  // atlas namespace