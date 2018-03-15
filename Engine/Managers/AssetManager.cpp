#include "Managers/AssetManager.h"
#include "Assets/Asset.h"
#include "Core/Engine.h"
#include <fstream>
#include <spdlog/spdlog.h>
#include <wrenpp/Wren++.h>

namespace atlas
{
//
// AssetManager
//

AssetManager::AssetManager()
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::registerAssetType(AssetType assetType, AssetFactoryFunc f)
{
    if (_registry.find(assetType) != _registry.end())
    {
        Engine::log().warn("Asset type {}, already registered", assetType);
        return;
    }

    _registry.insert(make_pair(assetType, f));
}

AssetHandle AssetManager::addAssetImpl(AssetPtr resource)
{
    AssetHandle handle     = _assets.alloc();
    _assets.getRef(handle) = resource;
    // FIXME: proper check here
    _hashedAssets.insert(std::make_pair(resource->filename(), handle));
    return handle;
}

AssetHandle AssetManager::getHandle(StringHash hash) const
{
    auto it = _hashedAssets.find(hash);
    if (it != _hashedAssets.end())
        return it->second;

    return AssetHandle::invalid;
}

AssetHandle AssetManager::addAsset(AssetType type, const std::string& filename, u32 flags)
{
    if (filename == "")
    {
        Engine::log().error("Invalid name for added Asset of type {}", type);
        return AssetHandle::invalid;
    }

    // Create Asset
    AssetPtr asset = nullptr;
    auto     it    = _registry.find(type);
    if (it == _registry.end())
    {
        Engine::log().error("Asset type not registered: {}", type);
    }

    asset = it->second(filename, flags);
    if (asset == nullptr)
        return AssetHandle::invalid;

    Engine::log().info("Adding asset: '{}' of type: {}", filename.c_str(), type);
    AssetHandle handle = addAssetImpl(asset);
    if (handle != AssetHandle::invalid)
        asset->_handle = handle;

    // FIXME: proper check here
    _hashedAssets.insert(std::make_pair(filename, handle));

    return handle;
}

void AssetManager::removeAsset(AssetHandle handle)
{
    AssetPtr asset = getAsset(handle);
    if (asset != nullptr)
    {
        auto it = _hashedAssets.find(asset->hash());
        if (it != _hashedAssets.end())
            _hashedAssets.erase(it);
        asset = nullptr;  // TODO: check to see if this unloads (call destructor on Asset)
        _assets.remove(handle);
    }
}

void AssetManager::removeAssetByHash(StringHash hash)
{
    AssetHandle handle = getHandle(hash);
    removeAsset(handle);
}

const AssetPtr& AssetManager::getAsset(AssetHandle handle) const
{
    return _assets.getRef(handle);
}

AssetPtr& AssetManager::getAsset(AssetHandle handle)
{
    return _assets.getRef(handle);
}

void AssetManager::loadAssets()
{
    AssetPtr                asset        = nullptr;
    const AssetPackedArray& packedAssets = _assets.storage();
    for (u32 i = 0; i < packedAssets.count; ++i)
    {
        asset = packedAssets.array[i];
        if (asset != nullptr && !asset->isLoaded())
        {
            std::string   path = _assetsDir + asset->filename();
            std::ifstream ifs(path, std::ios::in | std::ios::binary);
            if (ifs)
            {
                if (!asset->load(ifs))
                {
                    Engine::log().warn("Couldn't load asset: {}", asset->filename());
                }
                else
                {
                    if (asset->isGPUResource() && !asset->uploadGPU())
                    {
                        Engine::log().warn("Couldn't upload asset: {}, to GPU", asset->filename());
                    }
                }
            }
            else
            {
                Engine::log().error("Couldn't find asset: {}", path.c_str());
            }
        }
    }
}

void AssetManager::loadAssetsAsync()
{
    // TODO
}

void AssetManager::setAssetsDir(const std::string& path)
{
    _assetsDir = path;
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

void wren::bindAssetManager()
{
    Engine::vm()
        .beginModule("scripts/Assets")
        .bindClass<AssetManager>("AssetManager")
        .bindMethod<decltype(&AssetManager::addAsset), &AssetManager::addAsset>(false, "addAsset(_,_,_)")
        .bindMethod<decltype(&AssetManager::removeAsset), &AssetManager::removeAsset>(false, "removeAsset(_)")
        .bindMethod<decltype(&AssetManager::removeAssetByHash), &AssetManager::removeAssetByHash>(
            false, "removeAssetByHash(_)")
        .bindMethod<decltype(&AssetManager::loadAssets), &AssetManager::loadAssets>(false, "loadAssets()")
        .bindMethod<decltype(&AssetManager::loadAssetsAsync), &AssetManager::loadAssetsAsync>(false,
                                                                                              "loadAssetsAsync()")
        .endClass()
        .endModule();
}

}  // atlas
