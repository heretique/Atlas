#include "Texture.h"
#include "Core/Engine.h"

#include <istream>

#include <bgfx/defines.h>
#include <bimg/decode.h>

namespace atlas
{
AssetPtr TextureAsset::factoryFunc(const std::string& filename, u32 flags)
{
    return std::make_shared<TextureAsset>(filename, flags);
}

TextureAsset::TextureAsset(const std::string& filename, u32 flags)
    : Asset(AssetTypes::Texture, filename, flags)
{
}

TextureAsset::~TextureAsset()
{
    bgfx::destroy(_texture);
}

bgfx::TextureHandle TextureAsset::bgfxHandle() const
{
    return _texture;
}

bool TextureAsset::loadImpl(std::istream& data)
{
    _texture = BGFX_INVALID_HANDLE;

    std::vector<unsigned char> buffer;
    buffer.insert(buffer.begin(), std::istreambuf_iterator<char>(data.rdbuf()), {});

    bimg::ImageContainer* imageContainer = bimg::imageParse(Engine::bxAllocator(), buffer.data(), buffer.size());

    if (imageContainer == nullptr)
    {
        return false;
    }

    const bgfx::Memory* mem = bgfx::copy(imageContainer->m_data, imageContainer->m_size);

    if (imageContainer->m_cubeMap)
    {
        _texture = bgfx::createTextureCube(uint16_t(imageContainer->m_width), 1 < imageContainer->m_numMips,
                                           imageContainer->m_numLayers,
                                           bgfx::TextureFormat::Enum(imageContainer->m_format), _flags, mem);
    }
    else if (1 < imageContainer->m_depth)
    {
        _texture = bgfx::createTexture3D(uint16_t(imageContainer->m_width), uint16_t(imageContainer->m_height),
                                         uint16_t(imageContainer->m_depth), 1 < imageContainer->m_numMips,
                                         bgfx::TextureFormat::Enum(imageContainer->m_format), _flags, mem);
    }
    else
    {
        _texture = bgfx::createTexture2D(uint16_t(imageContainer->m_width), uint16_t(imageContainer->m_height),
                                         1 < imageContainer->m_numMips, imageContainer->m_numLayers,
                                         bgfx::TextureFormat::Enum(imageContainer->m_format), _flags, mem);
    }

    bgfx::setName(_texture, _filename.c_str());

    bgfx::calcTextureSize(_textureInfo, uint16_t(imageContainer->m_width), uint16_t(imageContainer->m_height),
                          uint16_t(imageContainer->m_depth), imageContainer->m_cubeMap, 1 < imageContainer->m_numMips,
                          imageContainer->m_numLayers, bgfx::TextureFormat::Enum(imageContainer->m_format));

    bimg::imageFree(imageContainer);
    return true;
}

bool TextureAsset::isGPUResource()
{
    return true;
}

bool TextureAsset::uploadGPUImpl()
{
    return true;
}

}  // atlas namespace
