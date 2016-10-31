#include "Base.h"
#include "ResourceManager.h"
#include "Geometry.h"
#include <fmt/printf.h>

#define TINYGLTF_LOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "tinygltf/tiny_gltf_loader.h"

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

bool GeometryResource::load(const char *data, const uint size)
{
    tinygltf::Scene scene;
    tinygltf::TinyGLTFLoader loader;
    string err;
    string baseDir = "";
    bool ret = loader.LoadASCIIFromString(&scene, &err, data, size, baseDir);
    if (!ret)
    {
        fmt::print("Failed to load geometry resource: {}\n", _name);
        fmt::print("{}\n", err);
        return false;
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

