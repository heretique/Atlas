#pragma once

#include "Assets/Types.h"
#include "Core/StringHash.h"
#include "Core/Types.h"

#include <functional>
#include <memory>
#include <string>

namespace atlas
{
class Asset
{
public:
    Asset(AssetType type, const std::string& filename, u32 flags);
    virtual ~Asset();

    AssetHandle        handle() const;
    StringHash         hash() const;
    AssetType          type() const;
    u32                flags() const;
    const std::string& filename() const;
    bool               isLoaded() const;

    bool load(const std::istream& data);
    bool uploadGPU();

protected:
    virtual bool loadImpl(const std::istream& data) = 0;
    virtual bool isGPUResource();
    virtual bool uploadGPUImpl();

protected:
    AssetType   _type{AssetTypes::Undefined};
    std::string _filename{};
    StringHash  _hash{};
    u32         _flags{0};
    bool        _loaded{false};
    AssetHandle _handle{AssetHandle::invalid};

    friend class AssetManager;
};

inline AssetHandle Asset::handle() const
{
    return _handle;
}

inline StringHash Asset::hash() const
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

typedef std::function<AssetPtr(const std::string& filename, int flags)> AssetFactoryFunc;

}  // namespace atlas