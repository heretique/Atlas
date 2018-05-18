import "scripts/Utils" for StringHash

foreign class AssetPtr {
    foreign get
    foreign useCount
}

foreign class AssetTypes {
    foreign static Undefined
    foreign static Scene
    foreign static Geometry
    foreign static Animation
    foreign static Audio
    foreign static Material
    foreign static Code
    foreign static Shader
    foreign static Texture
    foreign static ParticleEffect
    foreign static Pipeline
    foreign static Template
}

foreign class ShaderTypes {
    foreign static None
    foreign static Vertex
    foreign static Fragment
    foreign static Compute
}

foreign class TextureFlags {
      foreign static None
      foreign static U_Mirror
      foreign static U_Clamp
      foreign static U_Border
      foreign static U_Shift
      foreign static U_Mask
      foreign static V_Mirror
      foreign static V_Clamp
      foreign static V_Border
      foreign static V_Shift
      foreign static V_Mask
      foreign static W_Mirror
      foreign static W_Clamp
      foreign static W_Border
      foreign static W_Shift
      foreign static W_Mask
      foreign static Min_Point
      foreign static Min_Anisotropic
      foreign static Min_Shift
      foreign static Min_Mask
      foreign static Mag_Point
      foreign static Mag_Anisotropic
      foreign static Mag_Shift
      foreign static Mag_Mask
      foreign static Mip_Point
      foreign static Mip_Shift
      foreign static Mip_Mask
      foreign static Msaa_Sample
      foreign static RT
      foreign static RT_Msaa_x2
      foreign static RT_Msaa_x4
      foreign static RT_Msaa_x8
      foreign static RT_Msaa_x16
      foreign static RT_Msaa_Shift
      foreign static RT_Msaa_Mask
      foreign static RT_Write_Only
      foreign static RT_Mask
      foreign static Compare_Less
      foreign static Compare_LEqual
      foreign static Compare_Equal
      foreign static Compare_GEqual
      foreign static Compare_Greater
      foreign static Compare_NotEqual
      foreign static Compare_Never
      foreign static Compare_Always
      foreign static Compare_Shift
      foreign static Compare_Mask
      foreign static Compute_Write
      foreign static SRGB
      foreign static Blit_Dst
      foreign static Read_Back
      foreign static Border_Color_Shift
      foreign static Border_Color_Mask
      foreign static Border_Color(index)
      foreign static Sampler_Bits_Mask
}

foreign class AssetManager {
    foreign addAsset(type, filename, flags)
    foreign removeAsset(ptr)
    foreign removeAssetByHash(hash)
    foreign loadAssets()
    foreign loadAssetsAsync()
}
