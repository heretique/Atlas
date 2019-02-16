#include "Shader.h"

#include "Core/Engine.h"
#include "spdlog/spdlog.h"
#include <istream>
#include <SDL_filesystem.h>

namespace atlas
{
AssetPtr ShaderAsset::factoryFunc(const std::string& filename, u32 flags)
{
    return std::make_shared<ShaderAsset>(filename, flags);
}

ShaderAsset::ShaderAsset(const std::string& filename, u32 flags)
    : Asset(AssetTypes::Shader, filename, flags)
{
}

ShaderAsset::~ShaderAsset()
{
    bgfx::destroy(_shader);
}

bgfx::ShaderHandle ShaderAsset::bgfxHandle() const
{
    return _shader;
}

char shaderTypeToChar(ShaderTypes shaderType)
{
    switch (shaderType)
    {
        case ShaderTypes::Compute:
            return 'c';
        case ShaderTypes::Vertex:
            return 'v';
        case ShaderTypes::Fragment:
            return 'f';
        default:
            return ' ';
    }
}

bool ShaderAsset::loadImpl(std::istream& data)
{
    _shader = BGFX_INVALID_HANDLE;

    if (ShaderTypes::None == static_cast<ShaderTypes>(_flags))
        return false;

    _shaderBuffer           = std::string(std::istreambuf_iterator<char>(data.rdbuf()), {});
    const bgfx::Memory* mem = bgfx::makeRef(_shaderBuffer.c_str(), _shaderBuffer.size());
    _shader                 = bgfx::createShader(mem);

    return bgfx::isValid(_shader);
}

bool ShaderAsset::isGPUResource()
{
    return true;
}

bool ShaderAsset::uploadGPUImpl()
{
    return true;
}

}  // atlas namespace
