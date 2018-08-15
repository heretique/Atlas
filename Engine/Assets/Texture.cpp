#include "Texture.h"

#include "Core/Engine.h"
#include "wrenpp/Wren++.h"
#include <bgfx/defines.h>
#include <bimg/decode.h>

namespace atlas
{
AssetPtr TextureAsset::factoryFunc(const std::string& filename, u32 flags)
{
    return std::make_shared<TextureAsset>(filename, flags);
}

TextureAsset::TextureAsset(const std::string& filename, u32 flags)
    : Asset(AssetTypes::Texture, filename, flags)
{
}

TextureAsset::~TextureAsset()
{
    bgfx::destroy(_texture);
}

bgfx::TextureHandle TextureAsset::bgfxHandle() const
{
    return _texture;
}

bool TextureAsset::loadImpl(std::istream& data)
{
    _texture = BGFX_INVALID_HANDLE;

    std::vector<unsigned char> buffer;
    buffer.insert(buffer.begin(), std::istreambuf_iterator<char>(data.rdbuf()), {});

    bimg::ImageContainer* imageContainer = bimg::imageParse(Engine::bxAllocator(), buffer.data(), buffer.size());

    if (imageContainer == nullptr)
    {
        return false;
    }

    const bgfx::Memory* mem = bgfx::copy(imageContainer->m_data, imageContainer->m_size);

    if (imageContainer->m_cubeMap)
    {
        _texture = bgfx::createTextureCube(uint16_t(imageContainer->m_width), 1 < imageContainer->m_numMips,
                                           imageContainer->m_numLayers,
                                           bgfx::TextureFormat::Enum(imageContainer->m_format), _flags, mem);
    }
    else if (1 < imageContainer->m_depth)
    {
        _texture = bgfx::createTexture3D(uint16_t(imageContainer->m_width), uint16_t(imageContainer->m_height),
                                         uint16_t(imageContainer->m_depth), 1 < imageContainer->m_numMips,
                                         bgfx::TextureFormat::Enum(imageContainer->m_format), _flags, mem);
    }
    else
    {
        _texture = bgfx::createTexture2D(uint16_t(imageContainer->m_width), uint16_t(imageContainer->m_height),
                                         1 < imageContainer->m_numMips, imageContainer->m_numLayers,
                                         bgfx::TextureFormat::Enum(imageContainer->m_format), _flags, mem);
    }

    bgfx::setName(_texture, _filename.c_str());

    bgfx::calcTextureSize(_textureInfo, uint16_t(imageContainer->m_width), uint16_t(imageContainer->m_height),
                          uint16_t(imageContainer->m_depth), imageContainer->m_cubeMap, 1 < imageContainer->m_numMips,
                          imageContainer->m_numLayers, bgfx::TextureFormat::Enum(imageContainer->m_format));

    bimg::imageFree(imageContainer);
    return true;
}

bool TextureAsset::isGPUResource()
{
    return true;
}

bool TextureAsset::uploadGPUImpl()
{
    return true;
}

namespace wren
{
    void textureFlag_None(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_NONE);
    }

    void textureFlag_U_Mirror(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_U_MIRROR);
    }

    void textureFlag_U_Clamp(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_U_CLAMP);
    }

    void textureFlag_U_Border(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_U_BORDER);
    }

    void textureFlag_U_Shift(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_U_SHIFT);
    }

    void textureFlag_U_Mask(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_U_MASK);
    }

    void textureFlag_V_Mirror(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_V_MIRROR);
    }

    void textureFlag_V_Clamp(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_V_CLAMP);
    }

    void textureFlag_V_Border(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_V_BORDER);
    }

    void textureFlag_V_Shift(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_V_SHIFT);
    }

    void textureFlag_V_Mask(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_V_MASK);
    }

    void textureFlag_W_Mirror(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_W_MIRROR);
    }

    void textureFlag_W_Clamp(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_W_CLAMP);
    }

    void textureFlag_W_Border(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_W_BORDER);
    }

    void textureFlag_W_Shift(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_W_SHIFT);
    }

    void textureFlag_W_Mask(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_W_MASK);
    }

    void textureFlag_Min_Point(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_MIN_POINT);
    }

    void textureFlag_Min_Anisotropic(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_MIN_ANISOTROPIC);
    }

    void textureFlag_Min_Shift(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_MIN_SHIFT);
    }

    void textureFlag_Min_Mask(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_MIN_MASK);
    }

    void textureFlag_Mag_Point(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_MAG_POINT);
    }

    void textureFlag_Mag_Anisotropic(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_MAG_ANISOTROPIC);
    }

    void textureFlag_Mag_Shift(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_MAG_SHIFT);
    }

    void textureFlag_Mag_Mask(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_MAG_MASK);
    }

    void textureFlag_Mip_Point(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_MIP_POINT);
    }

    void textureFlag_Mip_Shift(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_MIP_SHIFT);
    }

    void textureFlag_Mip_Mask(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_MIP_MASK);
    }

    void textureFlag_Msaa_Sample(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_MSAA_SAMPLE);
    }

    void textureFlag_RT(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_RT);
    }

    void textureFlag_RT_Msaa_x2(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_RT_MSAA_X2);
    }

    void textureFlag_RT_Msaa_x4(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_RT_MSAA_X4);
    }

    void textureFlag_RT_Msaa_x8(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_RT_MSAA_X8);
    }

    void textureFlag_RT_Msaa_x16(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_RT_MSAA_X16);
    }

    void textureFlag_RT_Msaa_Shift(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_RT_MSAA_SHIFT);
    }

    void textureFlag_RT_Msaa_Mask(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_RT_MSAA_MASK);
    }

    void textureFlag_RT_Write_Only(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_RT_WRITE_ONLY);
    }

    void textureFlag_RT_Mask(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_RT_MASK);
    }

    void textureFlag_Compare_Less(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_COMPARE_LESS);
    }

    void textureFlag_Compare_LEqual(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_COMPARE_LEQUAL);
    }

    void textureFlag_Compare_Equal(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_COMPARE_EQUAL);
    }

    void textureFlag_Compare_GEqual(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_COMPARE_GEQUAL);
    }

    void textureFlag_Compare_Greater(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_COMPARE_GREATER);
    }

    void textureFlag_Compare_NotEqual(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_COMPARE_NOTEQUAL);
    }

    void textureFlag_Compare_Never(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_COMPARE_NEVER);
    }

    void textureFlag_Compare_Always(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_COMPARE_ALWAYS);
    }

    void textureFlag_Compare_Shift(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_COMPARE_SHIFT);
    }

    void textureFlag_Compare_Mask(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_COMPARE_MASK);
    }

    void textureFlag_Compute_Write(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_COMPUTE_WRITE);
    }

    void textureFlag_SRGB(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_SRGB);
    }

    void textureFlag_Blit_Dst(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_BLIT_DST);
    }

    void textureFlag_Read_Back(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_READ_BACK);
    }

    void textureFlag_Border_Color_Shift(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_BORDER_COLOR_SHIFT);
    }

    void textureFlag_Border_Color_Mask(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_BORDER_COLOR_MASK);
    }

    void textureFlag_Border_Color(WrenVM* vm)
    {
        u32 index = static_cast<u32>(wrenGetSlotDouble(vm, 1));
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_BORDER_COLOR(index));
    }

    void textureFlag_Sampler_Bits_Mask(WrenVM* vm)
    {
        wrenSetSlotDouble(vm, 0, BGFX_TEXTURE_SAMPLER_BITS_MASK);
    }

    void bindTextureTypes()
    {
        Engine::vm()
            .beginModule("main")                                                         //
            .beginClass("TextureFlags")                                                  //
            .bindCFunction(true, "None", &textureFlag_None)                              //
            .bindCFunction(true, "U_Mirror", &textureFlag_U_Mirror)                      //
            .bindCFunction(true, "U_Clamp", &textureFlag_U_Clamp)                        //
            .bindCFunction(true, "U_Border", &textureFlag_U_Border)                      //
            .bindCFunction(true, "U_Shift", &textureFlag_U_Shift)                        //
            .bindCFunction(true, "U_Mask", &textureFlag_U_Mask)                          //
            .bindCFunction(true, "V_Mirror", &textureFlag_V_Mirror)                      //
            .bindCFunction(true, "V_Clamp", &textureFlag_V_Clamp)                        //
            .bindCFunction(true, "V_Border", &textureFlag_V_Border)                      //
            .bindCFunction(true, "V_Shift", &textureFlag_V_Shift)                        //
            .bindCFunction(true, "V_Mask", &textureFlag_V_Mask)                          //
            .bindCFunction(true, "W_Mirror", &textureFlag_W_Mirror)                      //
            .bindCFunction(true, "W_Clamp", &textureFlag_W_Clamp)                        //
            .bindCFunction(true, "W_Border", &textureFlag_W_Border)                      //
            .bindCFunction(true, "W_Shift", &textureFlag_W_Shift)                        //
            .bindCFunction(true, "W_Mask", &textureFlag_W_Mask)                          //
            .bindCFunction(true, "Min_Point", &textureFlag_Min_Point)                    //
            .bindCFunction(true, "Min_Anisotropic", &textureFlag_Min_Anisotropic)        //
            .bindCFunction(true, "Min_Shift", &textureFlag_Min_Shift)                    //
            .bindCFunction(true, "Min_Mask", &textureFlag_Min_Mask)                      //
            .bindCFunction(true, "Mag_Point", &textureFlag_Mag_Point)                    //
            .bindCFunction(true, "Mag_Anisotropic", &textureFlag_Mag_Anisotropic)        //
            .bindCFunction(true, "Mag_Shift", &textureFlag_Mag_Shift)                    //
            .bindCFunction(true, "Mag_Mask", &textureFlag_Mag_Mask)                      //
            .bindCFunction(true, "Mip_Point", &textureFlag_Mip_Point)                    //
            .bindCFunction(true, "Mip_Shift", &textureFlag_Mip_Shift)                    //
            .bindCFunction(true, "Mip_Mask", &textureFlag_Mip_Mask)                      //
            .bindCFunction(true, "Msaa_Sample", &textureFlag_Msaa_Sample)                //
            .bindCFunction(true, "RT", &textureFlag_RT)                                  //
            .bindCFunction(true, "RT_Msaa_x2", &textureFlag_RT_Msaa_x2)                  //
            .bindCFunction(true, "RT_Msaa_x4", &textureFlag_RT_Msaa_x4)                  //
            .bindCFunction(true, "RT_Msaa_x8", &textureFlag_RT_Msaa_x8)                  //
            .bindCFunction(true, "RT_Msaa_x16", &textureFlag_RT_Msaa_x16)                //
            .bindCFunction(true, "RT_Msaa_Shift", &textureFlag_RT_Msaa_Shift)            //
            .bindCFunction(true, "RT_Msaa_Mask", &textureFlag_RT_Msaa_Mask)              //
            .bindCFunction(true, "RT_Write_Only", &textureFlag_RT_Write_Only)            //
            .bindCFunction(true, "RT_Mask", &textureFlag_RT_Mask)                        //
            .bindCFunction(true, "Compare_Less", &textureFlag_Compare_Less)              //
            .bindCFunction(true, "Compare_LEqual", &textureFlag_Compare_LEqual)          //
            .bindCFunction(true, "Compare_Equal", &textureFlag_Compare_Equal)            //
            .bindCFunction(true, "Compare_GEqual", &textureFlag_Compare_GEqual)          //
            .bindCFunction(true, "Compare_Greater", &textureFlag_Compare_Greater)        //
            .bindCFunction(true, "Compare_NotEqual", &textureFlag_Compare_NotEqual)      //
            .bindCFunction(true, "Compare_Never", &textureFlag_Compare_Never)            //
            .bindCFunction(true, "Compare_Always", &textureFlag_Compare_Always)          //
            .bindCFunction(true, "Compare_Shift", &textureFlag_Compare_Shift)            //
            .bindCFunction(true, "Compare_Mask", &textureFlag_Compare_Mask)              //
            .bindCFunction(true, "Compute_Write", &textureFlag_Compute_Write)            //
            .bindCFunction(true, "SRGB", &textureFlag_SRGB)                              //
            .bindCFunction(true, "Blit_Dst", &textureFlag_Blit_Dst)                      //
            .bindCFunction(true, "Read_Back", &textureFlag_Read_Back)                    //
            .bindCFunction(true, "Border_Color_Shift", &textureFlag_Border_Color_Shift)  //
            .bindCFunction(true, "Border_Color_Mask", &textureFlag_Border_Color_Mask)    //
            .bindCFunction(true, "Border_Color(_)", &textureFlag_Border_Color)           //
            .bindCFunction(true, "Sampler_Bits_Mask", &textureFlag_Sampler_Bits_Mask)    //
            .endClass()                                                                  //
            .endModule();

        Engine::wrenModule() +=
            "foreign class TextureFlags {\n"
            "    foreign static None\n"
            "    foreign static U_Mirror\n"
            "    foreign static U_Clamp\n"
            "    foreign static U_Border\n"
            "    foreign static U_Shift\n"
            "    foreign static U_Mask\n"
            "    foreign static V_Mirror\n"
            "    foreign static V_Clamp\n"
            "    foreign static V_Border\n"
            "    foreign static V_Shift\n"
            "    foreign static V_Mask\n"
            "    foreign static W_Mirror\n"
            "    foreign static W_Clamp\n"
            "    foreign static W_Border\n"
            "    foreign static W_Shift\n"
            "    foreign static W_Mask\n"
            "    foreign static Min_Point\n"
            "    foreign static Min_Anisotropic\n"
            "    foreign static Min_Shift\n"
            "    foreign static Min_Mask\n"
            "    foreign static Mag_Point\n"
            "    foreign static Mag_Anisotropic\n"
            "    foreign static Mag_Shift\n"
            "    foreign static Mag_Mask\n"
            "    foreign static Mip_Point\n"
            "    foreign static Mip_Shift\n"
            "    foreign static Mip_Mask\n"
            "    foreign static Msaa_Sample\n"
            "    foreign static RT\n"
            "    foreign static RT_Msaa_x2\n"
            "    foreign static RT_Msaa_x4\n"
            "    foreign static RT_Msaa_x8\n"
            "    foreign static RT_Msaa_x16\n"
            "    foreign static RT_Msaa_Shift\n"
            "    foreign static RT_Msaa_Mask\n"
            "    foreign static RT_Write_Only\n"
            "    foreign static RT_Mask\n"
            "    foreign static Compare_Less\n"
            "    foreign static Compare_LEqual\n"
            "    foreign static Compare_Equal\n"
            "    foreign static Compare_GEqual\n"
            "    foreign static Compare_Greater\n"
            "    foreign static Compare_NotEqual\n"
            "    foreign static Compare_Never\n"
            "    foreign static Compare_Always\n"
            "    foreign static Compare_Shift\n"
            "    foreign static Compare_Mask\n"
            "    foreign static Compute_Write\n"
            "    foreign static SRGB\n"
            "    foreign static Blit_Dst\n"
            "    foreign static Read_Back\n"
            "    foreign static Border_Color_Shift\n"
            "    foreign static Border_Color_Mask\n"
            "    foreign static Border_Color(index)\n"
            "    foreign static Sampler_Bits_Mask\n"
            "}\n";
    }
}

}  // atlas namespace
