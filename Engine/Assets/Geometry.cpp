#include "Geometry.h"

#include "Assets/Types.h"
#include "Core/Engine.h"
#include "Core/Hash.h"
#include "Managers/AssetManager.h"
#include "Math/Vector3.h"

#include <algorithm>
#include <unordered_set>
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

bool GeometryAsset::loadImpl(const std::istream& is)
{
    tinyobj::attrib_t                vertexData;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;
    std::string                      err;

    tinyobj::MaterialFileReader materialReader(_filename.substr(0, _filename.find_last_of('/')));

    if (!tinyobj::LoadObj(&vertexData, &shapes, &materials, &err, const_cast<std::istream*>(&is), &materialReader))
    {
        Engine::log().error("Failed to load OBJ with error: {}", err.c_str());
        return false;
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

    const shape_t& shape = shapes.front();
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
        _indices.push_back(static_cast<uint16_t>(std::distance(uniqueVerticesCombination.begin(), returnPair.first)));
    }

    // reverse order to get the normals right again
    // order was reversed by unique combination insertion
    std::reverse(_indices.begin(), _indices.end());

    // interleave vertex attributes
    _vertices.reserve(uniqueVerticesCombination.size() * 3 * 3 * 2);
    for (const index_t& index : uniqueVerticesCombination)
    {
        math::Vector3 vertex(static_cast<size_t>(index.vertex_index * 3),
                             static_cast<size_t>(index.vertex_index * 3 + 1),
                             static_cast<size_t>(index.vertex_index * 3 + 2));
        _vertices.push_back(vertexData.vertices[vertex.x]);
        _vertices.push_back(vertexData.vertices[vertex.y]);
        _vertices.push_back(vertexData.vertices[vertex.z]);
        _vertices.push_back(vertexData.normals[static_cast<size_t>(index.normal_index * 3)]);
        _vertices.push_back(vertexData.normals[static_cast<size_t>(index.normal_index * 3 + 1)]);
        _vertices.push_back(vertexData.normals[static_cast<size_t>(index.normal_index * 3 + 2)]);
        _vertices.push_back(vertexData.texcoords[static_cast<size_t>(index.texcoord_index * 2)]);
        _vertices.push_back(1.0f - vertexData.texcoords[static_cast<size_t>(index.texcoord_index * 2 + 1)]);
        // update bbox
        _aabb.extend(vertex);
    }

    material_t material = materials.front();
    _texture            = Engine::assets().addAsset(AssetTypes::Texture, material.diffuse_texname);

    return true;

    return true;
}

}  // atlas
