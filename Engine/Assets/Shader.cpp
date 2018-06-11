#include "Shader.h"

#include "Core/Engine.h"
#include "spdlog/spdlog.h"

#include <SDL2/SDL_filesystem.h>
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
            .beginModule("main")                                  //
            .beginClass("ShaderTypes")                            //
            .bindCFunction(true, "None", &shaderTypeNone)         //
            .bindCFunction(true, "Vertex", &shaderTypeVertex)     //
            .bindCFunction(true, "Fragment", shaderTypeFragment)  //
            .bindCFunction(true, "Compute", &shaderTypeCompute)   //
            .endClass()                                           //
            .endModule();

        Engine::wrenModule() +=
            "foreign class ShaderTypes {\n"
            "    foreign static None\n"
            "    foreign static Vertex\n"
            "    foreign static Fragment\n"
            "    foreign static Compute\n"
            "}\n";
    }
}

}  // atlas namespace
