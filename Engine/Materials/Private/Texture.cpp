#include "Materials/Texture.h"
#include "Core/MemoryManager.h"

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
    bgfx_destroy_texture(_texture);
}

bgfx_texture_handle_t TextureAsset::bgfxHandle() const
{
    return _texture;
}

bool TextureAsset::loadImpl(std::istream& data)
{
    _texture = BGFX_INVALID_HANDLE;

    std::vector<unsigned char> buffer;
    buffer.insert(buffer.begin(), std::istreambuf_iterator<char>(data.rdbuf()), {});

    bimg::ImageContainer* imageContainer = bimg::imageParse(MemoryManager::bxAllocator(), buffer.data(), buffer.size());

    if (imageContainer == nullptr)
    {
        return false;
    }

    const bgfx_memory_t* mem = bgfx_copy(imageContainer->m_data, imageContainer->m_size);

    if (imageContainer->m_cubeMap)
    {
        _texture = bgfx_create_texture_cube(uint16_t(imageContainer->m_width), 1 < imageContainer->m_numMips,
                                            imageContainer->m_numLayers,
                                            bgfx_texture_format_t(imageContainer->m_format), _flags, mem);
    }
    else if (1 < imageContainer->m_depth)
    {
        _texture = bgfx_create_texture_3d(uint16_t(imageContainer->m_width), uint16_t(imageContainer->m_height),
                                          uint16_t(imageContainer->m_depth), 1 < imageContainer->m_numMips,
                                          bgfx_texture_format_t(imageContainer->m_format), _flags, mem);
    }
    else
    {
        _texture = bgfx_create_texture_2d(uint16_t(imageContainer->m_width), uint16_t(imageContainer->m_height),
                                          1 < imageContainer->m_numMips, imageContainer->m_numLayers,
                                          bgfx_texture_format_t(imageContainer->m_format), _flags, mem);
    }

    bgfx_set_texture_name(_texture, _filename.c_str(), INT32_MAX);

    bgfx_calc_texture_size(&_textureInfo, uint16_t(imageContainer->m_width), uint16_t(imageContainer->m_height),
                           uint16_t(imageContainer->m_depth), imageContainer->m_cubeMap, 1 < imageContainer->m_numMips,
                           imageContainer->m_numLayers, bgfx_texture_format_t(imageContainer->m_format));

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
