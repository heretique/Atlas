#pragma once

#include "Hq/NonCopyable.h"

namespace atlas
{

class ParticleSystem : public hq::NonCopyable
{
public:
    ParticleSystem();
    ~ParticleSystem();

    void update(float dt);
};

} // atlas namespace
