#pragma once

#include "Assets/Asset.h"
#include "Materials/Texture.h"
#include "Hq/BasicTypes.h"
#include <vector>
#include <bgfx/c99/bgfx.h>

namespace atlas
{
struct MaterialParam
{
    u32                type;
    std::vector<float> value;

    bgfx_uniform_type_t toBgfxUniformType() const;

    template <class Serializer>
    void Serialize(Serializer& serializer)
    {
        SERIALIZE(type);
        SERIALIZE(value);
    }
};

struct MaterialInfo
{
    using Params   = std::unordered_map<std::string, MaterialParam>;
    using Textures = std::unordered_map<std::string, std::string>;

    std::string name;
    std::string vertexShader;
    std::string fragmentShader;
    Params      params;
    Textures    textures;

    template <class Serializer>
    void Serialize(Serializer& serializer)
    {
        SERIALIZE(name);
        SERIALIZE(vertexShader);
        SERIALIZE(fragmentShader);
        SERIALIZE(params);
        SERIALIZE(textures);
    }
};

struct MaterialUniform
{
    bgfx_uniform_handle_t uniform;
    bgfx_uniform_type_t   type;
    std::vector<float>    value;
};

struct MaterialSampler
{
    bgfx_uniform_handle_t         sampler;
    std::shared_ptr<TextureAsset> textureHandle;
};

class MaterialAsset : public Asset
{
public:
    static AssetPtr factoryFunc(const std::string& filename, u32 flags);

    MaterialAsset(const std::string& filename, u32 flags);
    ~MaterialAsset() override;

    void setMaterialInfo(const MaterialInfo& info);
    void write(const std::string& filename);

    /// "Binds" the material, setting all uniform values and texture samplers to bgfx pipeline
    /// readying for submision to render thread
    void bind() const;
    /// Return program handle to be used with bgfx::submit
    bgfx_program_handle_t program() const;

    // Asset interface
protected:
    virtual bool loadImpl(std::istream& data) override;
    virtual bool isGPUResource() override;
    virtual bool uploadGPUImpl() override;

private:
    using MaterialUniforms = std::unordered_map<std::string, MaterialUniform>;
    using MaterialSamplers = std::unordered_map<std::string, MaterialSampler>;

    u32                   _materialId;
    MaterialInfo          _materialInfo;
    AssetPtr              _vsh;
    AssetPtr              _fsh;
    MaterialUniforms      _uniforms;
    MaterialSamplers      _samplers;
    bgfx_program_handle_t _program;
};

}  // atlas namespace
