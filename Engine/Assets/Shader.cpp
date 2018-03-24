#include "Shader.h"

#include "Core/Engine.h"
#include "spdlog/spdlog.h"

#include <SDL2/SDL_filesystem.h>
#include <shaderc/shaderc.h>
#include <wrenpp/Wren++.h>

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

bool ShaderAsset::loadImpl(const std::istream& data)
{
    _shader = BGFX_INVALID_HANDLE;

    if (ShaderTypes::None == (ShaderTypes)_flags)
        return false;

    std::string  buffer     = std::string(std::istreambuf_iterator<char>(data.rdbuf()), {});
    const size_t padding    = 4096;
    uint32_t     size       = buffer.size();
    char*        shaderData = new char[size + padding + 1];
    bx::memCopy(shaderData, buffer.c_str(), size);

    if (shaderData[0] == '\xef' && shaderData[1] == '\xbb' && shaderData[2] == '\xbf')
    {
        bx::memMove(shaderData, &shaderData[3], size - 3);
        size -= 3;
    }

    // Compiler generates "error X3000: syntax error: unexpected end of file"
    // if input doesn't have empty line at EOF.
    shaderData[size] = '\n';
    bx::memSet(&shaderData[size + 1], 0, padding);
    buffer.clear();

    shaderc::ShaderCompileOptions compileOptions;

    char shaderType           = shaderTypeToChar((ShaderTypes)_flags);
    compileOptions.shaderType = shaderType;

    std::string varying;
    std::string basePath     = SDL_GetBasePath();
    compileOptions.inputFilePath = basePath + _filename;

    std::string relativePath = _filename.substr(0, _filename.find_last_of('/')) + "/";
    basePath += relativePath;
    compileOptions.includeDirs.push_back(basePath);

    std::string   defaultVarying = basePath + "varying.def.sc";
    std::ifstream ifs(defaultVarying, std::ios::in | std::ios::binary);
    if (ifs)
    {
        varying = std::string(std::istreambuf_iterator<char>(ifs.rdbuf()), {});
    }
    if (!varying.empty())
    {
        compileOptions.dependencies.push_back(varying);
    }
    else
    {
        Engine::log().error(
            "Failed to parse varying def file: {}, No input/output semantics will be generated in the code!\n",
            defaultVarying);
    }

    compileOptions.profile = "130";
    if (!shaderc::compileShader(varying.c_str(), shaderData, size, compileOptions, &_memWriter))
        return false;

    const bgfx::Memory* mem = bgfx::makeRef(_memWriter.buffer.data(), _memWriter.buffer.size());
    _shader = bgfx::createShader(mem);

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

namespace wren
{
    void shaderTypeNone(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, (double)ShaderTypes::None);
    }

    void shaderTypeVertex(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, (double)ShaderTypes::Vertex);
    }

    void shaderTypeFragment(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, (double)ShaderTypes::Fragment);
    }

    void shaderTypeCompute(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, (double)ShaderTypes::Compute);
    }

    void bindShaderTypes()
    {
        Engine::vm()
            .beginModule("scripts/Assets")                        //
            .beginClass("ShaderTypes")                            //
            .bindCFunction(true, "None", &shaderTypeNone)         //
            .bindCFunction(true, "Vertex", &shaderTypeVertex)     //
            .bindCFunction(true, "Fragment", shaderTypeFragment)  //
            .bindCFunction(true, "Compute", &shaderTypeCompute)   //
            .endClass()                                           //
            .endModule();
    }
}

}  // atlas namespace
