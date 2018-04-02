#include "Geometry.h"

#include "Assets/Types.h"
#include "Core/Engine.h"
#include "Core/Hash.h"
#include "Core/PackUtils.h"
#include "Managers/AssetManager.h"
#include "Math/Vector3.h"

#include <algorithm>
#include <unordered_set>

#include <SDL2/SDL_filesystem.h>
#include <spdlog/spdlog.h>

#define TINYOBJLOADER_IMPLEMENTATION  // define this in only *one* .cc
#include "tinyobj/tiny_obj_loader.h"

namespace atlas
{
GeometryAsset::GeometryAsset(const std::string& filename, u32 flags)
    : Asset(AssetTypes::Geometry, filename, flags)
{
}

GeometryAsset::~GeometryAsset()
{
    bgfx::destroy(_vbh);
    bgfx::destroy(_ibh);
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
            return make_hash(k.vertex_index, k.texcoord_index, k.normal_index);
        }
    };
}

bool GeometryAsset::loadImpl(std::istream& is)
{
    tinyobj::attrib_t                vertexData;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;
    std::string                      err;

    std::string basePath     = SDL_GetBasePath();
    std::string relativePath = _filename.substr(0, _filename.find_last_of('/')) + "/";
    basePath += relativePath;
    tinyobj::MaterialFileReader materialReader(basePath);

    if (!tinyobj::LoadObj(&vertexData, &shapes, &materials, &err, const_cast<std::istream*>(&is), &materialReader))
    {
        Engine::log().error("Failed to load OBJ with error: {}", err.c_str());
        return false;
    }

    if (!err.empty())
    {
        Engine::log().warn(err);
    }

    assert(vertexData.texcoords.size());
    assert(vertexData.normals.size());
    assert(materials.size());
    assert(shapes.size());

    if (shapes.size() > 1)
    {
        Engine::log().warn("Multiple shapes/objects inside 3D model resources not supported!");
        Engine::log().warn("\n\t- affected resource: {}", _filename);
    }

    if (materials.size() > 1)
    {
        Engine::log().warn("Multiple materials per 3D model resource not supported!");
        Engine::log().warn("\n\t- affected resource: {}", _filename);
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
    std::reverse(_indices.begin(), _indices.end());

    // interleave vertex attributes
    _vertices.reserve(uniqueVerticesCombination.size());
    for (const index_t& index : uniqueVerticesCombination)
    {
        SimpleMeshVertex vertex;
        vertex.x       = vertexData.vertices[static_cast<size_t>(index.vertex_index * 3)];
        vertex.y       = vertexData.vertices[static_cast<size_t>(index.vertex_index * 3 + 1)];
        vertex.z       = vertexData.vertices[static_cast<size_t>(index.vertex_index * 3 + 2)];
        vertex.normal  = packF4u(vertexData.normals[static_cast<size_t>(index.normal_index * 3)],      //
                                vertexData.normals[static_cast<size_t>(index.normal_index * 3 + 1)],  //
                                vertexData.normals[static_cast<size_t>(index.normal_index * 3 + 2)],  //
                                0.f);
        vertex.tangent = packF4u(0.f, 0.f, 0.f, 0.f);
        vertex.color   = packF4u(0.f, 0.f, 0.f, 0.f);

        vertex.u = vertexData.texcoords[static_cast<size_t>(index.texcoord_index * 2)];
        vertex.v = 1.f - vertexData.texcoords[static_cast<size_t>(index.texcoord_index * 2 + 1)];
        _vertices.emplace_back(vertex);
        // update bbox
        _aabb.extend(math::Vector3(vertex.x, vertex.y, vertex.z));
    }

    material_t material = materials.front();
    _texture            = Engine::assets().addAsset(AssetTypes::Texture, relativePath + material.diffuse_texname);

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

    const bgfx::Memory* vertexMem =
        bgfx::copy(reinterpret_cast<u8*>(_vertices.data()), _vertices.size() * SimpleMeshVertex::size());
    _vbh = bgfx::createVertexBuffer(vertexMem, SimpleMeshVertex::vertDecl);

    const bgfx::Memory* indexMem = bgfx::copy(reinterpret_cast<u8*>(_indices.data()), _indices.size() * sizeof(u16));
    _ibh                         = bgfx::createIndexBuffer(indexMem);

    assert(bgfx::isValid(_vbh));
    assert(bgfx::isValid(_ibh));

    _vertices.clear();
    _indices.clear();

    return true;
}
