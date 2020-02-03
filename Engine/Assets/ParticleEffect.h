#pragma once

#include "Asset.h"
#include "Hq/BasicTypes.h"
#include "Hq/Math/MathTypes.h"

namespace atlas
{

struct EmitterShape
{
    enum Enum
    {
        Point,
        Quad,
        Circle,
        Cube,
        Sphere,
        Cone,
        Cylinder
    };
};



struct ParticleEffectInfo
{

    u32 emitterShape;
    u32 spawnRate;
    // negative = infinite, 0.f = one shot, greater than 0.f = spawnTime
    float spawnTime;
    float lifetime;
    float lifetimeRandomness;
    float speed;
    float speedRandomness;
    // initial direction
    hq::math::Vec3 direction;
    // spread per axis in degrees (0 - 360)
    hq::math::Vec3 spread;
    hq::math::Vec3 rotationSpeed;
    hq::math::Vec3 rotationSpeedRandomness;
    // transform paticles in local or world coordinates
    bool localCoordinates;

    template <class Serializer>
    void Serialize(Serializer& serializer)
    {
        SERIALIZE(emitterShape);
        SERIALIZE(spawnRate);
        SERIALIZE(spawnTime);
        SERIALIZE(lifetime);
        SERIALIZE(lifetimeRandomness);
        SERIALIZE(speed);
        SERIALIZE(speedRandomness);
        SERIALIZE(direction);
        SERIALIZE(spread);
        SERIALIZE(rotationSpeed);
        SERIALIZE(rotationSpeedRandomness);
        SERIALIZE(localCoordinates);
    }
};

class ParticleEffectAsset : public Asset
{
public:
    static AssetPtr factoryFunc(const std::string& filename, u32 flags);

    ParticleEffectAsset(const std::string& filename, u32 flags);
    ~ParticleEffectAsset() override;
    // Asset interface
protected:
    bool loadImpl(std::istream &data) override;

private:
};
}  // atlas namespace
