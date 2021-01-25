#pragma once

#include "Hq/NonCopyable.h"
#include "Ecs/ISystem.h"
#include "enkiTS/TaskScheduler.h"

namespace atlas
{

class ParticleSystem : public hq::NonCopyable,
        public ISystem,
        public enki::TaskSet
{
public:
    ParticleSystem();
    ~ParticleSystem();

    void update(float dt);

    // ISystem interface
public:
    void runSystem(entt::registry &registry, float dt) override;

    // ITaskSet interface
public:
    void ExecuteRange(enki::TaskSetPartition range_, uint32_t threadnum_) override;
};

} // atlas namespace
