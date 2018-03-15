#pragma once

#include "Assets/Asset.h"
#include "Core/SimpleMeshVertex.h"
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

    // Asset interface
protected:
    virtual bool loadImpl(const std::istream& data) override;
    virtual bool isGPUResource() override;
    virtual bool uploadGPUImpl() override;

private:
    //    bool                          _16BitIndices{true};
    bgfx::VertexBufferHandle      _vbh = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle       _ibh = BGFX_INVALID_HANDLE;
    math::BoundingBox             _aabb;
    std::vector<SimpleMeshVertex> _vertices;
    std::vector<u16>              _indices;
    AssetHandle                   _texture{AssetHandle::invalid};
};

}  // atlas
