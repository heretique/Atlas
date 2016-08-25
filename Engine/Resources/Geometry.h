#ifndef GEOMETRYRESOURCE_H
#define GEOMETRYRESOURCE_H

#include "bgfx/bgfx.h"

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
    bool load( const char *data, int size );

private:
    bgfx::VertexBufferHandle        _vbh;
    bgfx::IndexBufferHandle         _ibh;
    bgfx::DynamicVertexBufferHandle _dynvbh;
    bgfx::DynamicIndexBufferHandle  _dynibh;
};

} // atlas



#endif // GEOMETRYRESOURCE_H
