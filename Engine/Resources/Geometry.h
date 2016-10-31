#ifndef GEOMETRYRESOURCE_H
#define GEOMETRYRESOURCE_H

#include "bgfx/bgfx.h"
#include "Math/BoundingBox.h"

namespace atlas {

class GeometryResource : public Resource
{
public:
    static void initializationFunc();
    static void releaseFunc();
    static Resource *factoryFunc( const std::string &name, int flags )
        { return new GeometryResource( name, flags ); }

    GeometryResource( const std::string &name, int flags );
    ~GeometryResource();
    Resource *clone();

    void initDefault();
    void release();
    bool load( const char *data, const uint size );

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
