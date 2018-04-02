#pragma once

#include "Asset.h"
#include "Core/Types.h"
#include <vector>
#include <bgfx/bgfx.h>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>

namespace atlas
{
struct MaterialParam
{
    u32                type;
    std::vector<float> value;

    bgfx::UniformType::Enum toBgfxUniformType() const;

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(CEREAL_NVP(type),  //
           CEREAL_NVP(value));
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

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(CEREAL_NVP(name),            //
           CEREAL_NVP(vertexShader),    //
           CEREAL_NVP(fragmentShader),  //
           CEREAL_NVP(params),          //
           CEREAL_NVP(textures));
    }
};

struct MaterialUniform
{
    bgfx::UniformHandle     uniform;
    bgfx::UniformType::Enum type;
    std::vector<float>      value;
};

struct MaterialSampler
{
    bgfx::UniformHandle sampler;
    AssetHandle         textureHandle;
};

class MaterialAsset : public Asset
{
public:
    static AssetPtr factoryFunc(const std::string& filename, u32 flags);

    MaterialAsset(const std::string& filename, u32 flags);
    ~MaterialAsset() override;

    void setMaterialInfo(const MaterialInfo& info);
    void write();

    /// "Binds" the material, setting all uniform values and texture samplers to bgfx pipeline
    /// readying for submision to render thread
    void bind() const;
    /// Return program handle to be used with bgfx::submit
    bgfx::ProgramHandle program() const;

    // Asset interface
protected:
    virtual bool loadImpl(std::istream& data) override;
    virtual bool isGPUResource() override;
    virtual bool uploadGPUImpl() override;

private:
    using MaterialUniforms = std::unordered_map<std::string, MaterialUniform>;
    using MaterialSamplers = std::unordered_map<std::string, MaterialSampler>;

    u32                 _materialId;
    MaterialInfo        _materialInfo;
    AssetHandle         _vsh{AssetHandle::invalid};
    AssetHandle         _fsh{AssetHandle::invalid};
    MaterialUniforms    _uniforms;
    MaterialSamplers    _samplers;
    bgfx::ProgramHandle _program;
};

}  // atlas namespace
