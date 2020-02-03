#pragma once

#include "Assets/ParticleEffect.h"
#include "Hq/Math/MathTypes.h"

namespace atlas
{


struct Particle
{
    hq::math::Vec3 position;
    hq::math::Quat rotation;
    hq::math::Vec3 scale;
    hq::math::Vec3 velocity;
    hq::math::Vec3 angularVelocity;
    float drag, angularDrag;
    float time, lifeSpan;

};

class ParticleEffectComponent
{
friend class ParticleSystem;
public:
    ParticleEffectInfo _effectInfo;

private:
    std::vector<Particle> _particles;
    hq::math::Box3 _aabb;
};

} // atlas namespace
