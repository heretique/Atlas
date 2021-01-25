#include "Materials/Material.h"
#include "Materials/Texture.h"
#include "Assets/AssetManager.h"
#include "Materials/Shader.h"
#include "Core/Logger.h"
#include "Hq/JsonSerializer.h"

#include <fstream>

namespace atlas
{
AssetPtr MaterialAsset::factoryFunc(const std::string& filename, u32 flags)
{
    return std::make_shared<MaterialAsset>(filename, flags);
}

MaterialAsset::MaterialAsset(const std::string& filename, u32 flags)
    : Asset(AssetTypes::Material, filename, flags)
{
}

MaterialAsset::~MaterialAsset()
{
    for (const auto& uniformPair : _uniforms)
        bgfx_destroy_uniform(uniformPair.second.uniform);

    bgfx_destroy_program(_program);
}

void MaterialAsset::setMaterialInfo(const MaterialInfo& info)
{
    _materialInfo = info;
}

void MaterialAsset::write(const std::string& filename)
{
    std::ofstream      ofs(filename, std::ios::out | std::ios::binary);
    hq::JsonSerializer serializer(ofs);
    //    serializer(_materialInfo);
}

void MaterialAsset::bind() const
{
    for (const auto& uniformPair : _uniforms)
    {
        const MaterialUniform& materialUniform = uniformPair.second;
        bgfx_set_uniform(materialUniform.uniform, materialUniform.value.data(), 1);
    }

    uint8_t textureUnit = 0;
    for (auto& samplerPair : _samplers)
    {
        const MaterialSampler& materialSampler = samplerPair.second;
        bgfx_set_texture(textureUnit, materialSampler.sampler, materialSampler.textureHandle->bgfxHandle(), UINT32_MAX);
        ++textureUnit;
    }
}

bgfx_program_handle_t MaterialAsset::program() const
{
    return _program;
}

bool MaterialAsset::loadImpl(std::istream& data)
{
    hq::JsonDeserializer deserializer(data);
    deserializer(_materialInfo);

    AssetManager& assetMgr = AssetManager::instance();

    _vsh = assetMgr.addAsset(AssetTypes::Shader, _materialInfo.vertexShader, static_cast<u32>(ShaderTypes::Vertex));
    if (!assetMgr.loadAsset(_vsh))
    {
        Logger::log().error("Couldn't load vertex shader for material: '{}'", _filename);
        return false;
    }
    _fsh = assetMgr.addAsset(AssetTypes::Shader, _materialInfo.fragmentShader, static_cast<u32>(ShaderTypes::Fragment));
    if (!assetMgr.loadAsset(_fsh))
    {
        Logger::log().error("Couldn't load vertex shader for material: '{}'", _filename);
        return false;
    }

    bgfx_shader_handle_t vsh = std::static_pointer_cast<ShaderAsset>(_vsh)->bgfxHandle();
    bgfx_shader_handle_t fsh = std::static_pointer_cast<ShaderAsset>(_fsh)->bgfxHandle();
    _program                 = bgfx_create_program(vsh, fsh, false);
    if (!BGFX_HANDLE_IS_VALID(_program))
    {
        Logger::log().error("Couldn't create program for material: '{}'", _filename);
        return false;
    }

    for (const auto& pair : _materialInfo.textures)
    {
        AssetPtr textureHandle = assetMgr.addAsset(AssetTypes::Texture, pair.second);
        if (!assetMgr.loadAsset(textureHandle))
        {
            Logger::log().error("Couldn't load texture: '{}' for material: '{}'", pair.second, _filename);
            return false;
        }

        bgfx_uniform_handle_t samplerHandle = bgfx_create_uniform(pair.first.c_str(), BGFX_UNIFORM_TYPE_SAMPLER, 1);
        MaterialSampler       sampler       = {samplerHandle, std::dynamic_pointer_cast<TextureAsset>(textureHandle)};
        _samplers.insert(std::make_pair(pair.first, sampler));
    }

    for (const auto& paramPair : _materialInfo.params)
    {
        bgfx_uniform_type_t   uniformType   = paramPair.second.toBgfxUniformType();
        bgfx_uniform_handle_t uniformHandle = bgfx_create_uniform(paramPair.first.c_str(), uniformType, 1);
        if (!BGFX_HANDLE_IS_VALID(uniformHandle))
        {
            Logger::log().error("Couldn't create uniform: '{}' for material '{}'", paramPair.first, _filename);
            return false;
        }
        MaterialUniform uniform = {uniformHandle, uniformType, paramPair.second.value};
        _uniforms.insert(std::make_pair(paramPair.first, uniform));
    }
    return true;
}

bool MaterialAsset::isGPUResource()
{
    return true;
}

bool MaterialAsset::uploadGPUImpl()
{
    return true;
}

bgfx_uniform_type_t MaterialParam::toBgfxUniformType() const
{
    switch (type)
    {
        case 0:
            return BGFX_UNIFORM_TYPE_SAMPLER;
        case 1:
            break;
        case 2:
            return BGFX_UNIFORM_TYPE_VEC4;
        case 3:
            return BGFX_UNIFORM_TYPE_MAT3;
        case 4:
            return BGFX_UNIFORM_TYPE_MAT4;
        default:
            break;
    }
    return BGFX_UNIFORM_TYPE_COUNT;
}

}  // atlas namespace
