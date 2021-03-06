#include "Geometry/Geometry.h"
#include "Assets/Types.h"
#include "Hq/Hash.h"
#include "Hq/PackUtils.h"
#include "Assets/AssetManager.h"
#include "Hq/Math/Box3.h"
#include "Core/Logger.h"

#include <algorithm>
#include <unordered_set>

#include <SDL2/SDL_filesystem.h>

#define TINYOBJLOADER_IMPLEMENTATION  // define this in only *one* .cc
#include "tiny_obj_loader.h"

namespace atlas
{
GeometryAsset::GeometryAsset(const std::string& filename, u32 flags)
    : Asset(AssetTypes::Geometry, filename, flags)
{
}

GeometryAsset::~GeometryAsset()
{
    bgfx_destroy_vertex_buffer(_vbh);
    bgfx_destroy_index_buffer(_ibh);
}

bgfx_vertex_buffer_handle_t GeometryAsset::vbo() const
{
    return _vbh;
}

bgfx_index_buffer_handle_t GeometryAsset::ibo() const
{
    return _ibh;
}

const hq::math::Box3& GeometryAsset::bounds() const
{
    return _aabb;
}

namespace
{
    using namespace tinyobj;

    struct EqualsIndex
    {
        bool operator()(const index_t& lhs, const index_t& rhs) const
        {
            return lhs.vertex_index == rhs.vertex_index &&      //
                   lhs.texcoord_index == rhs.texcoord_index &&  //
                   lhs.normal_index == rhs.normal_index;
        }
    };

    struct HasherIndex
    {
        size_t operator()(const index_t& k) const
        {
            return hq::make_hash(k.vertex_index, k.texcoord_index, k.normal_index);
        }
    };
}

bool GeometryAsset::loadImpl(std::istream& is)
{
    using namespace hq;
    tinyobj::attrib_t                vertexData;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;
    std::string                      err;
    std::string                      warn;

    std::string basePath     = SDL_GetBasePath();
    std::string relativePath = _filename.substr(0, _filename.find_last_of('/')) + "/";
    basePath += relativePath;
    tinyobj::MaterialFileReader materialReader(basePath);

    if (!tinyobj::LoadObj(
            &vertexData, &shapes, &materials, &warn, &err, const_cast<std::istream*>(&is), &materialReader))
    {
        Logger::log().error("Failed to load OBJ with error: {}", err.c_str());
        return false;
    }

    if (!err.empty())
    {
        Logger::log().warn(err);
    }

    assert(vertexData.texcoords.size());
    assert(vertexData.normals.size());
    assert(shapes.size());

    if (shapes.size() > 1)
    {
        Logger::log().warn("Multiple shapes/objects inside 3D model resources not supported!");
        Logger::log().warn("\n\t- affected resource: {}", _filename);
    }

    if (materials.size() > 1)
    {
        Logger::log().warn("Multiple materials per 3D model resource not supported!");
        Logger::log().warn("\n\t- affected resource: {}", _filename);
    }

    const shape_t&                                        shape = shapes.front();
    std::unordered_set<index_t, HasherIndex, EqualsIndex> uniqueVerticesCombination;
    _indices.reserve(shape.mesh.indices.size());

    // get unique triples (position, norm, uv)
    for (const index_t& index : shape.mesh.indices)
        uniqueVerticesCombination.insert(index);

    // supporting only short indices for models
    assert(uniqueVerticesCombination.size() < 64 * 1024 * 1024);

    // try insert again to get unique position
    // we don't do this in first loop because items in set
    // keep changing position while inserting
    for (const index_t& index : shape.mesh.indices)
    {
        auto returnPair = uniqueVerticesCombination.insert(index);
        _indices.emplace_back(
            static_cast<uint16_t>(std::distance(uniqueVerticesCombination.begin(), returnPair.first)));
    }

    // reverse order to get the normals right again
    // order was reversed by unique combination insertion
    //    std::reverse(_indices.begin(), _indices.end());

    // interleave vertex attributes
    _vertices.reserve(uniqueVerticesCombination.size());
    _aabb.min = hq::math::Vec3(FLT_MAX);
    _aabb.max = hq::math::Vec3(-FLT_MAX);
    for (const index_t& index : uniqueVerticesCombination)
    {
        SimpleMeshVertex vertex;
        vertex.x      = vertexData.vertices[static_cast<size_t>(index.vertex_index * 3)];
        vertex.y      = vertexData.vertices[static_cast<size_t>(index.vertex_index * 3 + 1)];
        vertex.z      = vertexData.vertices[static_cast<size_t>(index.vertex_index * 3 + 2)];
        vertex.normal = packNF4u(vertexData.normals[static_cast<size_t>(index.normal_index * 3)],      //
                                 vertexData.normals[static_cast<size_t>(index.normal_index * 3 + 1)],  //
                                 vertexData.normals[static_cast<size_t>(index.normal_index * 3 + 2)],  //
                                 0.f);
        //        vertex.tangent = packF4u(0.f, 0.f, 0.f, 0.f);
        //        vertex.color   = packUF4u(0.278, 0.843, 0.384, 1.f);

        vertex.u = vertexData.texcoords[static_cast<size_t>(index.texcoord_index * 2)];
        vertex.v = 1.f - vertexData.texcoords[static_cast<size_t>(index.texcoord_index * 2 + 1)];
        _vertices.emplace_back(vertex);
        // update bbox

        math::merge(_aabb, math::Vec3(vertex.x, vertex.y, vertex.z));
    }

    //    material_t material = materials.front();
    //    _texture            = Engine::assets().addAsset(AssetTypes::Texture, relativePath + material.diffuse_texname);

    return true;
}

}  // atlas

bool atlas::GeometryAsset::isGPUResource()
{
    return true;
}

bool atlas::GeometryAsset::uploadGPUImpl()
{
    if (_vertices.empty())
        return false;

    const bgfx_memory_t* vertexMem =
        bgfx_copy(reinterpret_cast<u8*>(_vertices.data()), _vertices.size() * SimpleMeshVertex::size());
    _vbh = bgfx_create_vertex_buffer(vertexMem, &SimpleMeshVertex::vertLayout, BGFX_BUFFER_NONE);

    const bgfx_memory_t* indexMem = bgfx_copy(reinterpret_cast<u8*>(_indices.data()), _indices.size() * sizeof(u16));
    _ibh                          = bgfx_create_index_buffer(indexMem, BGFX_BUFFER_NONE);

    assert(BGFX_HANDLE_IS_VALID(_vbh));
    assert(BGFX_HANDLE_IS_VALID(_ibh));

    _vertices.clear();
    _indices.clear();

    return true;
}
