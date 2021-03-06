#include "Materials/Shader.h"
#include "Utils/BgfxUtils.h"
#include "Core/Logger.h"
#include <istream>
#include <sstream>
#include <filesystem>
#include "bx/process.h"
#include "bx/file.h"

namespace atlas
{
static std::string kDefaultVarying = "varying.def.sc";
#ifdef __unix__
static std::string kShaderCompiler = "shaderc";
#elif defined(WIN32) || defined(WIN64)
static std::string kShaderCompiler = "shaderc.exe";
#endif
static std::string kDefaultShaderIncludePath = "assets/shaders";

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
    bgfx_destroy_shader(_shader);
}

bgfx_shader_handle_t ShaderAsset::bgfxHandle() const
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
    namespace fs = std::filesystem;
    _shader      = BGFX_INVALID_HANDLE;

    if (ShaderTypes::None == static_cast<ShaderTypes>(_flags))
        return false;

    fs::path filePath(_filename);
    assert(filePath.has_parent_path());
    assert(filePath.has_extension());

    if (".sc" == filePath.extension())
    {
        fs::path compilerPath   = fs::current_path().append(kShaderCompiler);
        fs::path shaderInclPath = fs::current_path().append(kDefaultShaderIncludePath);
        fs::path shaderPath     = fs::absolute(filePath);
        fs::path varyingPath    = shaderPath.parent_path();
        varyingPath.append(kDefaultVarying);

        std::stringstream cmdArgs;

        cmdArgs << "-f " << shaderPath.string() << " ";
        cmdArgs << "-i " << shaderPath.parent_path().string() << " ";
        cmdArgs << "-i " << shaderInclPath.string() << " ";
        cmdArgs << "-c ";
        cmdArgs << "--type " << shaderTypeToChar(static_cast<ShaderTypes>(_flags)) << " ";

        std::string       cmdArgsString(cmdArgs.str());
        bx::Error         err;
        bx::StringView    cmdArgsView(cmdArgsString.data(), cmdArgsString.length());
        bx::ProcessReader shaderProcess;
        if (!bx::open(&shaderProcess, compilerPath.string().c_str(), cmdArgsView, &err))
        {
            Logger::log().warn("Shader compiler process failed to open with err: {}", err.getMessage().getPtr());
            return false;
        }

        char         buffer[1024];
        int32_t      bytesRead = 0;
        MemoryWriter compiledShaderWriter;
        while ((bytesRead = bx::read(&shaderProcess, buffer, sizeof(buffer), &err)) > 0)
        {
            bx::write(&compiledShaderWriter, buffer, bytesRead, &err);
        }

        bx::close(&shaderProcess);
        auto ecode = shaderProcess.getExitCode();
        if (0 != ecode)
        {
            Logger::log().warn("Shader compiler process failed to open with err: {}", shaderProcess.getExitCode());
            return false;
        }

        _shaderBuffer            = std::string(compiledShaderWriter.buffer.begin(), compiledShaderWriter.buffer.end());
        const bgfx_memory_t* mem = bgfx_make_ref(_shaderBuffer.c_str(), _shaderBuffer.size());
        _shader                  = bgfx_create_shader(mem);
    }
    else if (".bin" == filePath.extension())
    {
        // pre-compiled shader
        _shaderBuffer            = std::string(std::istreambuf_iterator<char>(data.rdbuf()), {});
        const bgfx_memory_t* mem = bgfx_make_ref(_shaderBuffer.c_str(), _shaderBuffer.size());
        _shader                  = bgfx_create_shader(mem);
    }

    return BGFX_HANDLE_IS_VALID(_shader);
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
