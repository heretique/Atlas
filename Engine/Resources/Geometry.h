#ifndef GEOMETRYRESOURCE_H
#define GEOMETRYRESOURCE_H

#include "bgfx/bgfx.h"
#include "Math/BoundingBox.h"

namespace atlas {

class GeometryAsset : public Asset
{
public:
    static Asset *factoryFunc( const std::string &name, int flags ) { return new GeometryAsset( name, flags ); }
    static void releaseFunc(Asset* asset) { delete asset; asset = nullptr; }

    GeometryAsset( const std::string &name, int flags );
    ~GeometryAsset();
    Asset *clone();

    void initDefault();
    void release();
    bool load( const istream& data) override;

private:

    u32                             _vertexCount{0};
    u32                             _indexCount{0};
    bool                            _16BitIndices{true};
    bgfx::VertexBufferHandle        _vbh = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle         _ibh = BGFX_INVALID_HANDLE;
    bgfx::DynamicVertexBufferHandle _dynvbh = BGFX_INVALID_HANDLE;
    bgfx::DynamicIndexBufferHandle  _dynibh = BGFX_INVALID_HANDLE;
    math::BoundingBox               _aabb;
};

} // atlas



#endif // GEOMETRYRESOURCE_H
