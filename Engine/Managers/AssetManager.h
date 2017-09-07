#pragma once

#include "Resources/Asset.h"

#include <functional>
#include <map>
#include <string>
#include <type_traits>
#include <signals-cpp/signals.hpp>

namespace atlas
{
class Asset;

//
// Resource Manager
//

typedef std::function<AssetPtr(const std::string& name, int flags)> AssetTypeFactoryFunc;

struct AssetRegEntry
{
    std::string          typeString;
    AssetTypeFactoryFunc factoryFunc;  // Factory to create asset object
};

// ========================

class AssetManager
{
public:
    AssetManager();
    ~AssetManager();

    void registerAssetType(int type, const std::string& typeString, AssetTypeFactoryFunc ff);

    AssetHandle addAsset(int type, const std::string& name, int flags);
    uint removeAsset(AssetHandle handle);
    const AssetPtr& getAsset(AssetHandle handle) const;
    AssetPtr& getAsset(AssetHandle handle);

    template <typename T>
    typename std::shared_ptr<typename std::enable_if<std::is_base_of<Asset, T>::value, T>::type> getAsset(
        AssetHandle handle)
    {
        return std::static_pointer_cast<T>(getAsset(handle));
    }

    AssetPtr getNextAsset(int type, AssetHandle start) const;
    AssetPtr findAsset(int type, const std::string& name) const;
    bool loadAssets();
    void loadAssetsAsync();
    void setAssetsDir(const std::string& path);
    void releaseUnusedAssets();
    int  unusedAssets();

public:  // signals
    sigs::signal<void(int)> LoadingProgress;
    sigs::signal<void()>    LoadingDone;

protected:
    AssetHandle addAsset(AssetPtr resource);

private:
    AssetStorage _assets;
    std::map<int, AssetRegEntry> _registry;
    std::string _assetDir;
};

}  // atlas
