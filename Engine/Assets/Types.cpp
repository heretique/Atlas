#include "Types.h"
#include "Core/Engine.h"
#include "wrenpp/Wren++.h"

namespace atlas
{
// TODOCM: refactor this abomination
const std::string& AssetTypes::toName(const AssetType type)
{
    if (type == Undefined)
        return AssetNames::Undefined;
    else if (type == Scene)
        return AssetNames::Scene;
    else if (type == Geometry)
        return AssetNames::Geometry;
    else if (type == Animation)
        return AssetNames::Animation;
    else if (type == Audio)
        return AssetNames::Audio;
    else if (type == Material)
        return AssetNames::Material;
    else if (type == Code)
        return AssetNames::Code;
    else if (type == Shader)
        return AssetNames::Shader;
    else if (type == Texture)
        return AssetNames::Texture;
    else if (type == ParticleEffect)
        return AssetNames::ParticleEffect;
    else if (type == Pipeline)
        return AssetNames::Pipeline;
    else if (type == Template)
        return AssetNames::Template;

    return AssetNames::Undefined;
}

namespace wren
{
    void assetType_Undefined(WrenVM* vm)
    {
        wrenpp::setSlotForeignValue<AssetType>(vm, 0, AssetTypes::Undefined);
    }

    void assetType_Scene(WrenVM* vm)
    {
        wrenpp::setSlotForeignValue<AssetType>(vm, 0, AssetTypes::Scene);
    }

    void assetType_Geometry(WrenVM* vm)
    {
        wrenpp::setSlotForeignValue<AssetType>(vm, 0, AssetTypes::Geometry);
    }

    void assetType_Animation(WrenVM* vm)
    {
        wrenpp::setSlotForeignValue<AssetType>(vm, 0, AssetTypes::Animation);
    }

    void assetType_Audio(WrenVM* vm)
    {
        wrenpp::setSlotForeignValue<AssetType>(vm, 0, AssetTypes::Audio);
    }

    void assetType_Material(WrenVM* vm)
    {
        wrenpp::setSlotForeignValue<AssetType>(vm, 0, AssetTypes::Material);
    }

    void assetType_Code(WrenVM* vm)
    {
        wrenpp::setSlotForeignValue<AssetType>(vm, 0, AssetTypes::Code);
    }

    void assetType_Shader(WrenVM* vm)
    {
        wrenpp::setSlotForeignValue<AssetType>(vm, 0, AssetTypes::Shader);
    }

    void assetType_Texture(WrenVM* vm)
    {
        wrenpp::setSlotForeignValue<AssetType>(vm, 0, AssetTypes::Texture);
    }

    void assetType_ParticleEffect(WrenVM* vm)
    {
        wrenpp::setSlotForeignValue<AssetType>(vm, 0, AssetTypes::ParticleEffect);
    }

    void assetType_Pipeline(WrenVM* vm)
    {
        wrenpp::setSlotForeignValue<AssetType>(vm, 0, AssetTypes::Pipeline);
    }

    void assetType_Template(WrenVM* vm)
    {
        wrenpp::setSlotForeignValue<AssetType>(vm, 0, AssetTypes::Template);
    }

    void bindAssetTypes()
    {
        Engine::vm()
            .beginModule("main")                                                                  //
            .bindClass<AssetPtr>("AssetPtr")                                                      //
            .bindMethod<decltype(&AssetPtr::get), &AssetPtr::get>(false, "get")                   //
            .bindMethod<decltype(&AssetPtr::use_count), &AssetPtr::use_count>(false, "useCount")  //
            .endClass()                                                                           //
            .beginClass("AssetTypes")                                                             //
            .bindCFunction(true, "Undefined", &wren::assetType_Undefined)                         //
            .bindCFunction(true, "Scene", &wren::assetType_Scene)                                 //
            .bindCFunction(true, "Geometry", &wren::assetType_Geometry)                           //
            .bindCFunction(true, "Animation", &wren::assetType_Animation)                         //
            .bindCFunction(true, "Audio", &wren::assetType_Audio)                                 //
            .bindCFunction(true, "Material", &wren::assetType_Material)                           //
            .bindCFunction(true, "Code", &wren::assetType_Code)                                   //
            .bindCFunction(true, "Shader", &wren::assetType_Shader)                               //
            .bindCFunction(true, "Texture", &wren::assetType_Texture)                             //
            .bindCFunction(true, "ParticleEffect", &wren::assetType_ParticleEffect)               //
            .bindCFunction(true, "Pipeline", &wren::assetType_Pipeline)                           //
            .bindCFunction(true, "Template", &wren::assetType_Template)                           //
            .endClass()                                                                           //
            .endModule();

        Engine::wrenModule() +=
            "foreign class AssetPtr {\n"
            "    foreign get\n"
            "    foreign useCount\n"
            "}\n"
            "\n"
            "foreign class AssetTypes {\n"
            "    foreign static Undefined\n"
            "    foreign static Scene\n"
            "    foreign static Geometry\n"
            "    foreign static Animation\n"
            "    foreign static Audio\n"
            "    foreign static Material\n"
            "    foreign static Code\n"
            "    foreign static Shader\n"
            "    foreign static Texture\n"
            "    foreign static ParticleEffect\n"
            "    foreign static Pipeline\n"
            "    foreign static Template\n"
            "}\n";
    }
}

// wren namespace
}  // atlas namespace
