#include "Base.h"
#include "ResourceManager.h"
#include "Geometry.h"
#include <fmt/printf.h>

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tinyobj/tiny_obj_loader.h"


namespace atlas {



class membuf : public std::basic_streambuf<char> {
public:
  membuf(const char *p, size_t l) {
    setg((char*)p, (char*)p, (char*)p + l);
  }
};

class memstream : public std::istream {
public:
  memstream(const char *p, size_t l) :
    std::istream(&_buffer),
    _buffer(p, l) {
    rdbuf(&_buffer);
  }

private:
  membuf _buffer;
};


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

bool GeometryResource::load(const char *data, const uint size)
{
//    tinygltf::Scene scene;
//    tinygltf::TinyGLTFLoader loader;
//    string err;
//    string baseDir = "";
//    bool ret = loader.LoadASCIIFromString(&scene, &err, data, size, baseDir);
//    if (!ret)
//    {
//        fmt::print("Failed to load geometry resource: {}\n", _name);
//        fmt::print("{}\n", err);
//        return false;
//    }

    memstream is(data, size);

    tinyobj::attrib_t attrib;
    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;

    string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, &is);
    if (!ret)
    {
        print("Failed to load OBJ with error: {}", err);
    }



    return true;
}


Resource *GeometryResource::clone()
{
    return nullptr;
}


void GeometryResource::initDefault()
{
    _16BitIndices = true;
    _vbh = BGFX_INVALID_HANDLE;
    _ibh = BGFX_INVALID_HANDLE;
    _dynvbh = BGFX_INVALID_HANDLE;
    _dynibh = BGFX_INVALID_HANDLE;
    _aabb.min = math::Vector3(0, 0, 0);
    _aabb.max = math::Vector3(0, 0, 0);
}


void GeometryResource::release()
{
    bgfx::destroyVertexBuffer(_vbh);
    bgfx::destroyIndexBuffer(_ibh);
    bgfx::destroyDynamicVertexBuffer(_dynvbh);
    bgfx::destroyDynamicIndexBuffer(_dynibh);
}

} // atlas

