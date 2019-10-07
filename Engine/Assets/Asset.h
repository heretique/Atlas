#pragma once

#include "Assets/Types.h"
#include "Hq/StringHash.h"
#include "Hq/BasicTypes.h"

#include <functional>
#include <memory>
#include <string>

namespace atlas
{
class Asset
{
public:
    virtual ~Asset();

    hq::StringHash     hash() const;
    AssetType          type() const;
    u32                flags() const;
    const std::string& filename() const;
    bool               isLoaded() const;

    bool load(std::istream& data);
    bool uploadGPU();

protected:
    Asset(AssetType type, const std::string& filename, u32 flags);

    virtual bool loadImpl(std::istream& data) = 0;
    virtual bool isGPUResource();
    virtual bool uploadGPUImpl();

protected:
    AssetType      _type {AssetTypes::Undefined};
    std::string    _filename {};
    hq::StringHash _hash {};
    u32            _flags {0};
    bool           _loaded {false};

    friend class AssetManager;
};

inline hq::StringHash Asset::hash() const
{
    return _hash;
}

inline AssetType Asset::type() const
{
    return _type;
}

inline u32 Asset::flags() const
{
    return _flags;
}

inline const std::string& Asset::filename() const
{
    return _filename;
}

inline bool Asset::isLoaded() const
{
    return _loaded;
}

using AssetFactoryFunc = std::function<AssetPtr(const std::string& filename, u32 flags)>;

}  // namespace atlas
