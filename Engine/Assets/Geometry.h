#pragma once

#include "Assets/Asset.h"
#include "Math/BoundingBox.h"
#include "bgfx/bgfx.h"
#include <vector>

namespace atlas
{
class GeometryAsset : public Asset
{
public:
    static AssetPtr factoryFunc(const std::string& filename, u32 flags)
    {
        return std::make_shared<GeometryAsset>(filename, flags);
    }

    GeometryAsset(const std::string& filename, u32 flags);
    ~GeometryAsset() override;

    bool loadImpl(const std::istream& data) override;

private:
    bool                     _16BitIndices{true};
    bgfx::VertexBufferHandle _vbh = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle  _ibh = BGFX_INVALID_HANDLE;
    math::BoundingBox        _aabb;
    std::vector<float>       _vertices;
    std::vector<u16>         _indices;
    AssetHandle              _texture{AssetHandle::invalid};
};

}  // atlas
