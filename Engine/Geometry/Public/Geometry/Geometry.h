#pragma once

#include "Assets/Asset.h"
#include "Core/SimpleMeshVertex.h"
#include "Hq/Math/MathTypes.h"
#include "bgfx/c99/bgfx.h"
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

    bgfx_vertex_buffer_handle_t vbo() const;
    bgfx_index_buffer_handle_t  ibo() const;

    const hq::math::Box3& bounds() const;

    // Asset interface
protected:
    virtual bool loadImpl(std::istream& is) override;
    virtual bool isGPUResource() override;
    virtual bool uploadGPUImpl() override;

private:
    //    bool                          _16BitIndices{true};
    bgfx_vertex_buffer_handle_t   _vbh = BGFX_INVALID_HANDLE;
    bgfx_index_buffer_handle_t    _ibh = BGFX_INVALID_HANDLE;
    hq::math::Box3                _aabb;
    std::vector<SimpleMeshVertex> _vertices;
    std::vector<u16>              _indices;
};

}  // atlas
