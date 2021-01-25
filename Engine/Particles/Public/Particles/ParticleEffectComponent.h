#pragma once

#include "Core/Component.h"
#include "Particles/ParticleEffect.h"
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
    float          drag, angularDrag;
    float          time, lifeSpan;
};

class ParticleEffectComponent : public Component
{
    friend class ParticleSystem;
    REFLECTABLE(Component)
public:
    ParticleEffectInfo _effectInfo;

private:
    std::vector<Particle> _particles;
    hq::math::Box3        _aabb;
};

}  // atlas namespace
