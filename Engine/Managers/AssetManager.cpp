#include "Managers/AssetManager.h"
#include "Assets/Asset.h"
#include "Managers/LogManager.h"
#include <fstream>

namespace atlas
{
//
// AssetManager
//

AssetManager::AssetManager() {}

AssetManager::~AssetManager() {}

void AssetManager::registerAssetType(int assetType, const std::string& typeString, AssetTypeFactoryFunc ff)
{
    AssetRegEntry entry;
    entry.typeString     = typeString;
    entry.factoryFunc    = ff;
    _registry[assetType] = entry;
}

AssetHandle AssetManager::addAsset(AssetPtr resource)
{
    AssetHandle handle     = _assets.alloc();
    *(_assets.get(handle)) = resource;
    return handle;
}

AssetHandle AssetManager::addAsset(int type, const std::string& name, int flags)
{
    if (name == "")
    {
        LOGINFO("Invalid name for added Asset of type %i", type);
        return kInvalidAssetHandle;
    }

    // Create Asset
    AssetPtr asset = nullptr;
    auto     it    = _registry.find(type);
    if (it == _registry.end())
    {
        LOGERROR("Asset type not registered: %d", type);
    }

    asset = it->second.factoryFunc(name, flags);
    if (asset == nullptr)
        return kInvalidAssetHandle;

    LOGINFO("Adding %s Asset '%s'\n", it->second.typeString.c_str(), name.c_str());
    AssetHandle handle = addAsset(asset);
    if (handle != kInvalidAssetHandle)
        asset->_handle = handle;
    return handle;
}

uint AssetManager::removeAsset(AssetHandle handle)
{
    AssetPtr asset = getAsset(handle);
    if (asset != nullptr)
    {
    }

    return 0;
}

const AssetPtr& AssetManager::getAsset(AssetHandle handle) const
{
    return *_assets.get(handle);
}

AssetPtr& AssetManager::getAsset(AssetHandle handle)
{
    return *_assets.get(handle);
}

AssetPtr AssetManager::getNextAsset(int type, AssetHandle start) const
{
    AssetPtr                res          = nullptr;
    const AssetPackedArray& packedAssets = _assets.storage();
    u32                     startIndex   = _assets.getPackedIndex(start);
    if (startIndex == kInvalidPackedIndex)
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
    bool                    success      = true;
    AssetPtr                asset        = nullptr;
    const AssetPackedArray& packedAssets = _assets.storage();
    for (u32 i = 0; i < packedAssets.count; ++i)
    {
        asset = packedAssets.array[i];
        if (asset != nullptr && !asset->isLoaded())
        {
            std::string   path = _assetDir + asset->name();
            std::ifstream ifs(path, std::ios::in | std::ios::binary);
            if (ifs)
            {
                if (!asset->load(ifs))
                {
                    success = false;
                    LOGWARNING("Couldn't load asset: %s\n", asset->name().c_str());
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

void AssetManager::setAssetsDir(const std::string& path)
{
    _assetDir = path;
}

void AssetManager::releaseUnusedAssets()
{
    const AssetPackedArray&  packedAssets = _assets.storage();
    std::vector<AssetHandle> assetsForRelease;

    for (u32 i = 0; i < packedAssets.count; ++i)
    {
        const AssetPtr& asset = packedAssets.array[i];
        if (asset != nullptr && 1 == asset.use_count())
        {
            assetsForRelease.push_back(asset->_handle);
        }
    }

    for (AssetHandle handle : assetsForRelease)
    {
        *_assets.get(handle) = nullptr;
        _assets.remove(handle);
    }

    if (unusedAssets() > 0)
        releaseUnusedAssets();  // previously released assets could leave others
                                // unused
}

int AssetManager::unusedAssets()
{
    const AssetPackedArray& packedAssets = _assets.storage();
    int                     unusedAssets = 0;

    for (u32 i = 0; i < packedAssets.count; ++i)
    {
        const AssetPtr& asset = packedAssets.array[i];
        if (asset != nullptr && 1 == asset.use_count())
        {
            ++unusedAssets;
        }
    }

    return unusedAssets;
}

}  // atlas
