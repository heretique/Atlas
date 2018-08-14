#include "Engine.h"

#include "Assets/Geometry.h"
#include "Assets/Material.h"
#include "Assets/Script.h"
#include "Assets/Shader.h"
#include "Assets/Texture.h"
#include "Components/MaterialComponent.h"
#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"
#include "Core/SimpleMeshVertex.h"
#include "Managers/AssetManager.h"
#include "Managers/JobManager.h"
#include "Managers/PluginManager.h"
#include "Managers/SceneManager.h"
#include "Nodes/Node.h"
#include "Scripting/WrenBindings.h"

#include <bgfx/bgfx.h>
#include <bx/allocator.h>
#include <spdlog/spdlog.h>
#include <wrenpp/Wren++.h>

namespace atlas
{
spdlog::logger* Engine::_logger        = nullptr;
PluginManager*  Engine::_pluginManager = nullptr;
AssetManager*   Engine::_assetManager  = nullptr;
SceneManager*   Engine::_sceneManager  = nullptr;
JobManager*     Engine::_jobManager    = nullptr;
wrenpp::VM*     Engine::_vm            = nullptr;

bgfx::VertexDecl SimpleMeshVertex::vertDecl;

std::string& Engine::wrenModule()
{
    static std::string wrenModule;
    return wrenModule;
}

bx::AllocatorI* Engine::bxAllocator()
{
    static bx::DefaultAllocator allocator;
    return &allocator;
}

const bgfx::Stats* Engine::bgfxStats()
{
    return bgfx::getStats();
}

bool Engine::init()
{
    _logger = spdlog::stdout_color_mt("console").get();

    if (_jobManager == nullptr)
        _jobManager = new JobManager();
    if (_pluginManager == nullptr)
        _pluginManager = new PluginManager();
    if (_assetManager == nullptr)
        _assetManager = new AssetManager();
    if (_sceneManager == nullptr)
        _sceneManager = new SceneManager();
    if (_vm == nullptr)
        _vm = new wrenpp::VM();

    initVertDecl();
    registerAssetTypes();
    registerComponentTypes();
    jobs().init();
    initVM();

    return true;
}

void Engine::initVertDecl()
{
    SimpleMeshVertex::init();
}

void Engine::initVM()
{
    // initialize wren vm
    wrenpp::VM::loadModuleFn = [](const char* mod) -> char* {
        std::string path(mod);
        path += ".wren";
        auto scriptAsset = Engine::assets().getAsset<ScriptAsset>(path);
        if (scriptAsset != nullptr)
        {
            auto  source = scriptAsset->script();
            char* buffer = static_cast<char*>(malloc(source.size() + 1));
            assert(buffer != nullptr);
            buffer[source.size()] = '\0';
            memcpy(buffer, source.c_str(), source.size());
            return buffer;
        }

        return nullptr;
    };

    wrenpp::VM::writeFn = [](const char* text) -> void {
        // this hack exists because Wren always prints an extra newline as a separate print statement.
        if (text[0] != '\n')
        {
            Engine::log().info(text);
        }
    };

    // bind wren modules
    wren::bindImgui();
    wren::bindMath();
    wren::bindUtils();
    wren::bindAssetTypes();
    wren::bindAssetManager();
    wren::bindTextureTypes();
    wren::bindShaderTypes();
    wren::bindNode();
    wren::bindTransformComponent();
    wren::bindEngine();
    wren::bindSceneManager();

    //    MaterialAsset test("test", 0);
    //    MaterialInfo  info;
    //    info.name           = "unlit_textured";
    //    MaterialParam param = {"s_diffuse", 0};
    //    info.params.emplace_back(param);
    //    info.textures.insert(std::make_pair("s_diffuse", "caruta.png"));
    //    test.setMaterialInfo(info);
    //    test.write();
}

void Engine::registerAssetTypes()
{
    assets().registerAssetType(AssetTypes::Geometry, AssetNames::Geometry, GeometryAsset::factoryFunc);
    assets().registerAssetType(AssetTypes::Code, AssetNames::Code, ScriptAsset::factoryFunc);
    assets().registerAssetType(AssetTypes::Texture, AssetNames::Texture, TextureAsset::factoryFunc);
    assets().registerAssetType(AssetTypes::Shader, AssetNames::Shader, ShaderAsset::factoryFunc);
    assets().registerAssetType(AssetTypes::Material, AssetNames::Material, MaterialAsset::factoryFunc);
}

void Engine::registerComponentTypes()
{
    scene().registerComponentType(ComponentTypes::Transform, ComponentNames::Transform,
                                  TransformComponent::factoryFunc);
    scene().registerComponentType(ComponentTypes::Mesh, ComponentNames::Mesh, MeshComponent::factoryFunc);
    scene().registerComponentType(ComponentTypes::Material, ComponentNames::Material, MaterialComponent::factoryFunc);
}

void Engine::release()
{
    jobs().release();

    delete _vm;
    _vm = nullptr;

    delete _sceneManager;
    _sceneManager = nullptr;

    delete _assetManager;
    _assetManager = nullptr;

    delete _pluginManager;
    _pluginManager = nullptr;

    delete _jobManager;
    _jobManager = nullptr;
}

namespace wren
{
    void bindEngine()
    {
        Engine::vm()
            .beginModule("main")
            .beginClass("Engine")
            .bindFunction<decltype(&Engine::assets), &Engine::assets>(true, "assets()")
            .bindFunction<decltype(&Engine::scene), &Engine::scene>(true, "scene()")
            .endClass()
            .endModule();

        Engine::wrenModule() +=
            "class Engine {\n"
            "    foreign static assets()\n"
            "    foreign static scene()\n"
            "}\n";
    }
}

}  // namespace Atlas
