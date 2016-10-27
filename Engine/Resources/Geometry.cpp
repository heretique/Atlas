#include "Base.h"
#include "ResourceManager.h"
#include "Geometry.h"

namespace atlas {

void GeometryResource::initializationFunc()
{

}


void GeometryResource::releaseFunc()
{

}


GeometryResource::GeometryResource( const string &name, int flags ) :
    Resource( (int)ResourceTypes::Geometry, name, flags )
{
    initDefault();
}


GeometryResource::~GeometryResource()
{
    release();
}

bool GeometryResource::load(const char *data, int size)
{
    return Resource::load(data, size);
}


Resource *GeometryResource::clone()
{
    return nullptr;
}


void GeometryResource::initDefault()
{

}


void GeometryResource::release()
{

}

} // atlas

