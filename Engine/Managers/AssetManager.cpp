#include "Managers/AssetManager.h"

#include "Assets/Asset.h"
#include "Core/Engine.h"
#include "Managers/JobManager.h"

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
        Engine::log().warn("Asset type '{}', already registered", AssetTypes::toName(assetType));
        return;
    }

    _registry.insert(make_pair(assetType, f));
}

AssetPtr AssetManager::getAsset(StringHash hash) const
{
    auto it = _hashedAssets.find(hash);
    if (it != _hashedAssets.end())
        return it->second;

    return nullptr;
}

AssetPtr AssetManager::addAsset(AssetType type, const std::string& filename, u32 flags)
{
    if (filename == "")
    {
        Engine::log().error("Invalid name: '{}' for added Asset of type '{}'", filename, AssetTypes::toName(type));
        return nullptr;
    }

    auto assetIt = _hashedAssets.find(filename);
    if (assetIt != _hashedAssets.end())
        return assetIt->second;

    // Create Asset
    AssetPtr asset = nullptr;
    auto     it    = _registry.find(type);
    if (it == _registry.end())
    {
        Engine::log().error("Asset type not registered: '{}'", AssetTypes::toName(type));
        return nullptr;
    }

    asset = it->second(filename, flags);
    if (asset == nullptr)
        return nullptr;

    Engine::log().info("Adding asset: '{}' of type: '{}'", filename.c_str(), AssetTypes::toName(type));
    _assets.emplace_back(asset);
    _hashedAssets.insert(std::make_pair(filename, asset));

    return asset;
}

void AssetManager::removeAsset(AssetPtr asset)
{
    if (asset != nullptr)
    {
        auto it = _hashedAssets.find(asset->hash());
        if (it != _hashedAssets.end())
            _hashedAssets.erase(it);

        _assets.erase(std::remove(_assets.begin(), _assets.end(), asset), _assets.end());
    }
}

void AssetManager::removeAssetByHash(StringHash hash)
{
    AssetPtr asset = getAsset(hash);
    removeAsset(asset);
}

bool AssetManager::loadAsset(AssetPtr asset)
{
    assert(asset);
    if (asset->isLoaded())
        return true;

    std::string   path = _assetsDir + asset->filename();
    std::ifstream ifs(path, std::ios::in | std::ios::binary);
    if (ifs)
    {
        if (!asset->load(ifs))
        {
            Engine::log().warn(
                "Couldn't load asset: '{}' of type: '{}'", asset->filename(), AssetTypes::toName(asset->type()));
        }
        else
        {
            if (asset->isGPUResource() && !asset->uploadGPU())
            {
                Engine::log().warn("Couldn't upload asset: '{}' of type: '{}', to GPU",
                                   asset->filename(),
                                   AssetTypes::toName(asset->type()));
            }
            else
            {
                Engine::log().info(
                    "Loaded asset: '{}' of type: '{}'", asset->filename(), AssetTypes::toName(asset->type()));
                return true;
            }
        }
    }
    else
    {
        Engine::log().error("Couldn't find asset: '{}'", path.c_str());
    }

    return false;
}

void AssetManager::loadAssets()
{
    _loadingCount = _assets.size();
    _loadedCount  = 0;
    for (AssetPtr asset : _assets)
    {
        loadAsset(asset);
        ++_loadedCount;
        LoadingProgress.fire(_loadedCount / (float)_loadingCount * 100.f);
    }
}

void AssetManager::loadAssetsAsync()
{
    // TODOCM: implement async job loading
    // doesn't work parallel needs secquential async so don't use jobs

    //    AssetPtr                asset        = nullptr;
    //    const AssetPackedArray& packedAssets = _assets.storage();
    //    _loadingCount                        = packedAssets.count;
    //    for (u32 i = 0; i < packedAssets.count; ++i)
    //    {
    //        asset = packedAssets.array[i];
    //        if (asset != nullptr && !asset->isLoaded())
    //        {
    //            std::string path = _assetsDir + asset->filename();
    //            JobFunc     func = [path](void* data, uint) {
    //                Asset*        asset = reinterpret_cast<Asset*>(data);
    //                std::ifstream ifs(path, std::ios::in | std::ios::binary);
    //                if (ifs)
    //                {
    //                    if (!asset->load(ifs))
    //                    {
    //                        Engine::log().warn("Couldn't load asset: '{}'", asset->filename());
    //                    }
    //                    else
    //                    {
    //                        if (asset->isGPUResource() && !asset->uploadGPU())
    //                        {
    //                            Engine::log().warn("Couldn't upload asset: '{}', to GPU", asset->filename());
    //                        }
    //                    }
    //                }
    //                else
    //                {
    //                    Engine::log().error("Couldn't find asset: '{}'", path.c_str());
    //                }
    //            };

    //            JobDoneFunc done = [&]() {
    //                std::lock_guard<std::mutex> ls(_loadingMutex);
    //                ++_loadedCount;
    //                LoadingProgress.fire(_loadedCount / (float)_loadingCount * 100.f);
    //            };
    //            Engine::jobs().addSignalingJob(func, reinterpret_cast<void*>(asset.get()), 1, done);
    //        }
    //        else
    //        {
    //            std::lock_guard<std::mutex> ls(_loadingMutex);
    //            ++_loadedCount;
    //            LoadingProgress.fire(_loadedCount / (float)_loadingCount * 100.f);
    //        }
    //    }
}

void AssetManager::setAssetsDir(const std::string& path)
{
    _assetsDir = path;
}

const std::string& AssetManager::assetsDir() const
{
    return _assetsDir;
}

void AssetManager::releaseUnusedAssets()
{
    _assets.erase(std::remove_if(_assets.begin(), _assets.end(), [](const AssetPtr& a) { return a.use_count() == 1; }),
                  _assets.end());
    if (unusedAssets() > 0)
        releaseUnusedAssets();  // previously released assets could leave others
                                // unused
}

int AssetManager::unusedAssets()
{
    int unusedAssets = 0;

    for (const AssetPtr& asset : _assets)
        if (asset.use_count() > 1)
            ++unusedAssets;

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
