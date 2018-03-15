import "scripts/Utils" for StringHash

foreign class AssetHandle {
    foreign static invalid
    foreign generation
    foreign index

    foreign == ( rhs )
    foreign != ( rhs )
    foreign < ( rhs )
}

class AssetTypes {
    static Undefined { StringHash.new("Undefined")}
    static Scene { StringHash.new("Scene")}
    static Geometry { StringHash.new("Geometry")}
    static Animation { StringHash.new("Animation")}
    static Audio { StringHash.new("Audio")}
    static Material { StringHash.new("Material")}
    static Code { StringHash.new("Code")}
    static Shader { StringHash.new("Shader")}
    static Texture { StringHash.new("Texture")}
    static ParticleEffect { StringHash.new("Particles")}
    static Pipeline { StringHash.new("Pipeline")}
    static Template { StringHash.new("Template")}
}

foreign class AssetManager {
    foreign addAsset(type, filename, flags)
    foreign removeAsset(handle)
    foreign removeAssetByHash(hash)
    foreign loadAssets()
    foreign loadAssetsAsync()
}
