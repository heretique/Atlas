#include "Material.h"
#include "Assets/Texture.h"
#include "Core/Engine.h"
#include "Managers/AssetManager.h"
#include "Shader.h"
#include <fstream>
#include <cereal/archives/json.hpp>
#include <spdlog/spdlog.h>

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
        bgfx::destroy(uniformPair.second.uniform);

    bgfx::destroy(_program);
}

void MaterialAsset::setMaterialInfo(const MaterialInfo& info)
{
    _materialInfo = info;
}

void MaterialAsset::write()
{
    std::ofstream             ofs("test.material", std::ios::out | std::ios::binary);
    cereal::JSONOutputArchive archive(ofs);
    archive(_materialInfo);
}

void MaterialAsset::bind() const
{
    for (const auto& uniformPair : _uniforms)
    {
        const MaterialUniform& materialUniform = uniformPair.second;
        bgfx::setUniform(materialUniform.uniform, materialUniform.value.data());
    }

    uint8_t textureUnit = 0;
    for (auto& samplerPair : _samplers)
    {
        const MaterialSampler&        materialSampler = samplerPair.second;
        std::shared_ptr<TextureAsset> texture = Engine::assets().getAsset<TextureAsset>(materialSampler.textureHandle);
        assert(texture);
        bgfx::setTexture(textureUnit, materialSampler.sampler, texture->bgfxHandle());
        ++textureUnit;
    }
}

bgfx::ProgramHandle MaterialAsset::program() const
{
    return _program;
}

bool MaterialAsset::loadImpl(std::istream& data)
{
    cereal::JSONInputArchive archive(data);
    archive(_materialInfo);

    _vsh = Engine::assets().addAsset(AssetTypes::Shader, _materialInfo.vertexShader,
                                     static_cast<u32>(ShaderTypes::Vertex));
    if (!Engine::assets().loadAsset(_vsh))
    {
        Engine::log().error("Couldn't load vertex shader for material: '{}'", _filename);
        return false;
    }
    _fsh = Engine::assets().addAsset(AssetTypes::Shader, _materialInfo.fragmentShader,
                                     static_cast<u32>(ShaderTypes::Fragment));
    if (!Engine::assets().loadAsset(_fsh))
    {
        Engine::log().error("Couldn't load vertex shader for material: '{}'", _filename);
        return false;
    }

    bgfx::ShaderHandle vsh = Engine::assets().getAsset<ShaderAsset>(_vsh)->bgfxHandle();
    bgfx::ShaderHandle fsh = Engine::assets().getAsset<ShaderAsset>(_fsh)->bgfxHandle();
    _program               = bgfx::createProgram(vsh, fsh);
    if (!bgfx::isValid(_program))
    {
        Engine::log().error("Couldn't create program for material: '{}'", _filename);
        return false;
    }

    for (const auto& pair : _materialInfo.textures)
    {
        AssetHandle textureHandle = Engine::assets().addAsset(AssetTypes::Texture, pair.second);
        if (!Engine::assets().loadAsset(textureHandle))
        {
            Engine::log().error("Couldn't load texture: '{}' for material: '{}'", pair.second, _filename);
            return false;
        }

        bgfx::UniformHandle samplerHandle = bgfx::createUniform(pair.first.c_str(), bgfx::UniformType::Int1);
        MaterialSampler     sampler       = {samplerHandle, textureHandle};
        _samplers.insert(std::make_pair(pair.first, sampler));
    }

    for (const auto& paramPair : _materialInfo.params)
    {
        bgfx::UniformType::Enum uniformType   = paramPair.second.toBgfxUniformType();
        bgfx::UniformHandle     uniformHandle = bgfx::createUniform(paramPair.first.c_str(), uniformType);
        if (!bgfx::isValid(uniformHandle))
        {
            Engine::log().error("Couldn't create uniform: '{}' for material '{}'", paramPair.first, _filename);
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

bgfx::UniformType::Enum MaterialParam::toBgfxUniformType() const
{
    switch (type)
    {
        case 0:
            return bgfx::UniformType::Int1;
        case 1:
            break;
        case 2:
            return bgfx::UniformType::Vec4;
        case 3:
            return bgfx::UniformType::Mat3;
        case 4:
            return bgfx::UniformType::Mat4;
        default:
            break;
    }
    return bgfx::UniformType::Count;
}

}  // atlas namespace
