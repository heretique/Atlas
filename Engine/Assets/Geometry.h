#pragma once

#include "Math/BoundingBox.h"
#include "Assets/Asset.h"
#include "bgfx/bgfx.h"

namespace atlas
{
class GeometryAsset : public Asset
{
public:
    static AssetPtr factoryFunc(const std::string& name, u32 flags)
    {
        return std::make_shared<GeometryAsset>(name, flags);
    }

    GeometryAsset(const std::string& name, u32 flags);
    virtual ~GeometryAsset() override;

    virtual bool load(const std::istream& data) override;
    virtual AssetPtr clone() const override;

private:
    u32                             _vertexCount{0};
    u32                             _indexCount{0};
    bool                            _16BitIndices{true};
    bgfx::VertexBufferHandle        _vbh    = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle         _ibh    = BGFX_INVALID_HANDLE;
    bgfx::DynamicVertexBufferHandle _dynvbh = BGFX_INVALID_HANDLE;
    bgfx::DynamicIndexBufferHandle  _dynibh = BGFX_INVALID_HANDLE;
    math::BoundingBox               _aabb;
};

}  // atlas
