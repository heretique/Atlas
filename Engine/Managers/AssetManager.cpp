#include "Base.h"
#include "AssetManager.h"
#include "LogManager.h"

namespace atlas {


//
// Asset
//
Asset::Asset( int type, const std::string &name, int flags )
{
    _type = type;
    _name = name;
    _handle = INVALID_ASSET_HANDLE;
    _loaded = false;
    _refCount = 0;
    _flags = flags;
}

Asset::~Asset()
{
    // Remove all references
    // Nothing to do here
}


Asset *Asset::clone()
{
    LOGINFO( "Asset cloning not implemented for type %i", _type );
    return 0x0;
}


void Asset::initDefault()
{
}


void Asset::release()
{
}


bool Asset::load( const istream& data )
{

    LOGWARNING( "Asset '%s' of type %i: No data loaded (file not found?)", _name.c_str(), _type );

    return false;
}


void Asset::unload()
{
    release();
    initDefault();
    _loaded = false;
}

int Asset::getElemCount(int elem)
{
    LOGERROR( "Invalid elem, atlasGetElemCount" );
    return 0;
}

int Asset::getElemParamI( int elem, int elemIdx, int param )
{
    LOGERROR( "Invalid elem, param or component in atlasGetResParamI" );
    return std::numeric_limits<float>::quiet_NaN();
}

void Asset::setElemParamI( int elem, int elemIdx, int param, int value )
{
    LOGERROR( "Invalid elem, param or component in atlasSetResParamI" );
}


float Asset::getElemParamF( int elem, int elemIdx, int param, int compIdx )
{
    LOGERROR( "Invalid elem, param or component in atlasGetResParamF" );
    return std::numeric_limits<float>::quiet_NaN();
}

void Asset::setElemParamF( int elem, int elemIdx, int param, int compIdx, float value )
{
    LOGERROR( "Invalid elem, param or component in atlasSetResParamF" );
}

const char *Asset::getElemParamStr( int elem, int elemIdx, int param )
{
    LOGERROR( "Invalid elem or param in atlasGetResParamStr" );
    return "";
}

void Asset::setElemParamStr( int elem, int elemIdx, int param, const char *value )
{
    LOGERROR( "Invalid elem or param in atlasSetResParamStr" );
}

void *Asset::mapStream( int elem, int elemIdx, int stream, bool read, bool write )
{
    LOGERROR( "Invalid operation in atlasMapResStream" );
    return 0x0;
}

void Asset::unmapStream()
{
    LOGERROR( "Invalid operation by atlasUnmapResStream" );
}


//
// AssetManager
//



AssetManager::AssetManager()
{

}

AssetManager::~AssetManager()
{

}


void AssetManager::registerAssetType(int assetType, const std::string &typeString,
                                     AssetTypeFactoryFunc ff,
                                     AssetTypeReleaseFunc rf)
{
    AssetRegEntry entry;
    entry.typeString = typeString;
    entry.releaseFunc = rf;
    entry.factoryFunc = ff;
    _registry[assetType] = entry;
}

AssetHandle AssetManager::addAsset(Asset *Asset)
{
    return _assets.add(Asset);
}

AssetHandle AssetManager::addAsset(int type, const std::string &name, int flags)
{
    if( name == "" )
    {
        LOGINFO("Invalid name for added Asset of type %i", type);
        return INVALID_ASSET_HANDLE;
    }


    // Create Asset
    Asset *asset = 0x0;
    auto it = _registry.find(type);
    if (it == _registry.end())
    {
        LOGERROR("Asset type not registered: %d", type);
    }

    asset = it->second.factoryFunc(name, flags);
    if( asset == 0x0 ) return INVALID_ASSET_HANDLE;

    LOGINFO("Adding %s Asset '%s'\n", it->second.typeString.c_str(), name.c_str());
    AssetHandle handle = addAsset(asset);
    if (_assets.isValid(handle))
    {
        asset->_handle = handle;
    }
    else
    {
        it->second.releaseFunc(asset);
    }
    return asset->_handle;
}

uint AssetManager::removeAsset(AssetHandle handle)
{
    Asset* asset =  getAsset(handle);
    if (asset != nullptr && asset->_refCount > 0) --asset->_refCount;

    return 0;
}

Asset *AssetManager::getAsset(AssetHandle handle)
{
    return *_assets.get(handle);
}

Asset *AssetManager::getNextAsset(int type, AssetHandle start)
{
    Asset *res = nullptr;
    const AssetPackedArray &packedAssets = _assets.storage();
    u32 startIndex = _assets.getPackedIndex(start);
    if (startIndex == INVALID_PACKED_INDEX)
    {
        for (auto i = startIndex + 1; i < packedAssets.count; ++i)
        {
            res = packedAssets.array[i];
            if (res != nullptr && (type == (int)AssetTypes::Undefined || res->_type == type))
            {
                return res;
            }
        }
    }

    return res;
}

bool AssetManager::loadAssets()
{
    bool success = true;
    Asset *asset = nullptr;
    const AssetPackedArray &packedAssets = _assets.storage();
    for (u32 i = 0; i < packedAssets.count; ++i)
    {
        asset = packedAssets.array[i];
        if (asset != nullptr && !asset->isLoaded())
        {
            string path = _assetDir + asset->getName();
            ifstream ifs(path, std::ios::in | std::ios::binary);
            if (ifs)
            {
                if (!asset->load(ifs))
                {
                    success = false;
                    LOGWARNING("Couldn't load asset: %s\n", asset->getName().c_str());
                }
            }
            else
            {
                success = false;
                LOGWARNING("Couldn't find asset: %s\n", path.c_str());
            }
        }
    }
    return success;
}

void AssetManager::loadAssetsAsync()
{
    // TODO
}

void AssetManager::setAssetsDir(const string& path)
{
    _assetDir = path;
}

void AssetManager::releaseUnusedAssets()
{
    const AssetPackedArray &packedAssets = _assets.storage();
    vector<AssetHandle> assetsForRelease;
    Asset *asset = nullptr;

    for (u32 i = 0; i < packedAssets.count; ++i)
    {
        asset = packedAssets.array[i];
        if (asset != nullptr && asset->_refCount == 0)
        {
            assetsForRelease.push_back(asset->_handle);
        }
    }

    for (AssetHandle handle: assetsForRelease)
    {
        asset = *_assets.get(handle);
        asset->release();
        auto it = _registry.find(asset->getType());
        if (it != _registry.end()) it->second.releaseFunc(asset);
        _assets.remove(handle);
    }

    if (unusedAssets() > 0)
        releaseUnusedAssets(); // previously released assets could leave others unused
}

int AssetManager::unusedAssets()
{
    const AssetPackedArray &packedAssets = _assets.storage();
    Asset *asset = nullptr;
    int unusedAssets = 0;

    for (u32 i = 0; i < packedAssets.count; ++i)
    {
        asset = packedAssets.array[i];
        if (asset != nullptr && asset->_refCount == 0)
        {
            ++unusedAssets;
        }
    }

    return unusedAssets;
}

} // atlas

