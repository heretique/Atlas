#pragma once

#include "Assets/Asset.h"
#include "Core/NonCopyable.h"

#include <functional>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <signals-cpp/signals.hpp>

namespace atlas
{
class AssetManager : NonCopyable
{
public:
    AssetManager();
    ~AssetManager();

    void registerAssetType(AssetType type, AssetFactoryFunc f);
    AssetPtr addAsset(AssetType type, const std::string& filename, u32 flags = 0);
    void removeAsset(AssetPtr handle);
    void removeAssetByHash(StringHash hash);

    template <typename T>
    typename std::shared_ptr<typename std::enable_if<std::is_base_of<Asset, T>::value, T>::type> getAsset(
        StringHash hash) const;

    bool loadAsset(AssetPtr handle);
    void loadAssets();
    void loadAssetsAsync();
    void setAssetsDir(const std::string& path);
    const std::string& assetsDir() const;
    void               releaseUnusedAssets();
    int                unusedAssets();

public:  // signals
    sigs::signal<void(int)> LoadingProgress;
    sigs::signal<void()>    LoadingDone;

protected:
    AssetPtr getAsset(StringHash hash) const;

private:
    using AssetsByHash = std::unordered_map<StringHash, AssetPtr, StringHash::Hasher>;
    AssetStorage _assets;
    AssetsByHash _hashedAssets;
    std::unordered_map<AssetType, AssetFactoryFunc> _registry;
    std::string _assetsDir{};

    std::mutex _loadingMutex;
    uint       _loadingCount;
    uint       _loadedCount;
};

template <typename T>
typename std::shared_ptr<typename std::enable_if<std::is_base_of<Asset, T>::value, T>::type> AssetManager::getAsset(
    StringHash hash) const
{
    return std::static_pointer_cast<T>(getAsset(hash));
}

namespace wren
{
    void bindAssetManager();
}

}  // atlas
