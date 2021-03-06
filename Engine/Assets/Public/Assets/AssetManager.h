#pragma once

#include "Assets/Asset.h"
#include "Hq/NonCopyable.h"

#include <functional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <signals-cpp/signals.hpp>

namespace atlas
{
class AssetManager : hq::NonCopyable
{
    friend class Engine;

public:

    ~AssetManager();

    static AssetManager& instance();

    void     registerAssetType(AssetType type, std::string assetTypeName, AssetFactoryFunc f);
    AssetPtr addAsset(AssetType type, const std::string& filename, u32 flags = 0);
    void     removeAsset(AssetPtr handle);
    void     removeAssetByHash(hq::StringHash hash);

    template <typename T>
    typename std::shared_ptr<typename std::enable_if<std::is_base_of<Asset, T>::value, T>::type> getAsset(
        hq::StringHash hash) const;

    bool               loadAsset(AssetPtr handle);
    void               loadAssets();
    void               loadAssetsAsync();
    void               setAssetsDir(const std::string& path);
    const std::string& assetsDir() const;
    void               releaseUnusedAssets();
    int                unusedAssets();
    const std::string  assetName(AssetType type);


public:  // signals
    sigs::signal<void(int)> LoadingProgress;
    sigs::signal<void()>    LoadingDone;

protected:
    AssetPtr getAsset(hq::StringHash hash) const;

private:
    AssetManager();
    void release();

    using AssetsByHash = std::unordered_map<hq::StringHash, AssetPtr, hq::StringHash::Hasher>;
    struct AssetRegistryEntry
    {
        std::string      name;
        AssetFactoryFunc factory;
    };
    AssetStorage                                      _assets;
    AssetsByHash                                      _hashedAssets;
    std::unordered_map<AssetType, AssetRegistryEntry> _registry;
    std::string                                       _assetsDir {};

    std::mutex _loadingMutex;
    uint       _loadingCount;
    uint       _loadedCount;
    bool       _released {false};
};

template <typename T>
typename std::shared_ptr<typename std::enable_if<std::is_base_of<Asset, T>::value, T>::type> AssetManager::getAsset(
    hq::StringHash hash) const
{
    return std::dynamic_pointer_cast<T>(getAsset(hash));
}
}  // atlas
