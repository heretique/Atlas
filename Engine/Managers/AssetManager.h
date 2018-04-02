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

    void        registerAssetType(AssetType type, AssetFactoryFunc f);
    AssetHandle addAsset(AssetType type, const std::string& filename, u32 flags = 0);
    void        removeAsset(AssetHandle handle);
    void        removeAssetByHash(StringHash hash);

    const AssetPtr& getAsset(AssetHandle handle) const;
    AssetPtr&       getAsset(AssetHandle handle);

    template <typename T>
    typename std::shared_ptr<typename std::enable_if<std::is_base_of<Asset, T>::value, T>::type> getAsset(
        AssetHandle handle) const;

    template <typename T>
    typename std::shared_ptr<typename std::enable_if<std::is_base_of<Asset, T>::value, T>::type> getAsset(
        StringHash hash) const;

    bool               loadAsset(AssetHandle handle);
    void               loadAssets();
    void               loadAssetsAsync();
    void               setAssetsDir(const std::string& path);
    const std::string& assetsDir() const;
    void               releaseUnusedAssets();
    int                unusedAssets();

public:  // signals
    sigs::signal<void(int)> LoadingProgress;
    sigs::signal<void()>    LoadingDone;

protected:
    AssetHandle addAssetImpl(AssetPtr resource);
    AssetHandle getHandle(StringHash hash) const;

private:
    using AssetsByHash = std::unordered_map<StringHash, AssetHandle, StringHash::Hasher>;
    AssetStorage                                    _assets;
    AssetsByHash                                    _hashedAssets;
    std::unordered_map<AssetType, AssetFactoryFunc> _registry;
    std::string                                     _assetsDir{};

    std::mutex _loadingMutex;
    uint       _loadingCount;
    uint       _loadedCount;
};

template <typename T>
typename std::shared_ptr<typename std::enable_if<std::is_base_of<Asset, T>::value, T>::type> AssetManager::getAsset(
    AssetHandle handle) const
{
    return std::static_pointer_cast<T>(getAsset(handle));
}

template <typename T>
typename std::shared_ptr<typename std::enable_if<std::is_base_of<Asset, T>::value, T>::type> AssetManager::getAsset(
    StringHash hash) const
{
    return std::static_pointer_cast<T>(getAsset(getHandle(hash)));
}

namespace wren
{
    void bindAssetManager();
}

}  // atlas
