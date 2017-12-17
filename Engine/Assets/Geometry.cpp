#include "Geometry.h"
#include "Managers/LogManager.h"

#define TINYOBJLOADER_IMPLEMENTATION  // define this in only *one* .cc
#include "tinyobj/tiny_obj_loader.h"

namespace atlas
{
class membuf : public std::basic_streambuf<char>
{
public:
    membuf(const char* p, size_t l)
    {
        setg((char*)p, (char*)p, (char*)p + l);
    }
};

class memstream : public std::istream
{
public:
    memstream(const char* p, size_t l)
        : std::istream(&_buffer)
        , _buffer(p, l)
    {
        rdbuf(&_buffer);
    }

private:
    membuf _buffer;
};

GeometryAsset::GeometryAsset(const std::string& name, u32 flags)
    : Asset(static_cast<int>(AssetTypes::Geometry), name, flags)
{
}

GeometryAsset::~GeometryAsset()
{
    bgfx::destroy(_vbh);
    bgfx::destroy(_ibh);
    bgfx::destroy(_dynvbh);
    bgfx::destroy(_dynibh);
}

bool GeometryAsset::load(const std::istream& is)
{
    //    tinygltf::Scene scene;
    //    tinygltf::TinyGLTFLoader loader;
    //    string err;
    //    string baseDir = "";
    //    bool ret = loader.LoadASCIIFromString(&scene, &err, data, size,
    //    baseDir);
    //    if (!ret)
    //    {
    //        fmt::print("Failed to load geometry resource: {}\n", _name);
    //        fmt::print("{}\n", err);
    //        return false;
    //    }

    //    memstream is(data, size);

    tinyobj::attrib_t                attrib;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    bool        ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, const_cast<std::istream*>(&is));
    if (!ret)
    {
        LOGERROR("Failed to load OBJ with error: %s\n", err.c_str());
    }

    return true;
}

AssetPtr GeometryAsset::clone() const
{
    return nullptr;
}

}  // atlas
