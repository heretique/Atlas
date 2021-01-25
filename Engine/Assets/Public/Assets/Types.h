#pragma once

#include "Hq/StringHash.h"
#include <memory>
#include <vector>
#include <unordered_map>

namespace atlas
{

class Asset;
using AssetPtr     = std::shared_ptr<Asset>;
using AssetStorage = std::vector<AssetPtr>;
using AssetType    = hq::StringHash;

namespace AssetNames
{
    static const std::string Undefined {"Undefined"};
    static const std::string Scene {"Scene"};
    static const std::string Geometry {"Geometry"};
    static const std::string Animation {"Animation"};
    static const std::string Audio {"Audio"};
    static const std::string Material {"Material"};
    static const std::string Code {"Code"};
    static const std::string Shader {"Shader"};
    static const std::string Texture {"Texture"};
    static const std::string ParticleEffect {"Particles"};
    static const std::string Pipeline {"Pipeline"};
    static const std::string Template {"Template"};
}

namespace AssetTypes
{
    static const AssetType Undefined {AssetNames::Undefined};
    static const AssetType Scene {AssetNames::Scene};
    static const AssetType Geometry {AssetNames::Geometry};
    static const AssetType Animation {AssetNames::Animation};
    static const AssetType Audio {AssetNames::Audio};
    static const AssetType Material {AssetNames::Material};
    static const AssetType Code {AssetNames::Code};
    static const AssetType Shader {AssetNames::Shader};
    static const AssetType Texture {AssetNames::Texture};
    static const AssetType ParticleEffect {AssetNames::ParticleEffect};
    static const AssetType Pipeline {AssetNames::Pipeline};
    static const AssetType Template {AssetNames::Template};
}  // AssetTypes namespace

namespace DefaultAssets
{
    static const std::string Triangle {"DefaultTriangle"};
    static const std::string Quad {"DefaultQuad"};
    static const std::string Hexagon {"DefaultHexagon"};
    static const std::string Tetrahedron {"DefaultTetrahedron"};
    static const std::string Pyramid {"DefaultPyramid"};
    static const std::string Cube {"DefaultCube"};
    static const std::string Cylinder {"DefaultCylinder"};
    static const std::string Cone {"DefaultCone"};
    static const std::string Sphere {"DefaultSphere"};
    static const std::string Torus {"DefaultTorus"};
    static const std::string UnlitMaterial {"DefaultUnlitMaterial"};
    static const std::string PBRMaterial {"DefaultPBRMaterial"};
    static const std::string EDLMaterial {"DefaultEDLMaterial"}; // Eye Dome Lighting Material
} // DefaultAssets namespace

}  // atlas namespace
