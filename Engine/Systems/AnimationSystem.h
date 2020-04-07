#pragma once

#include "Hq/NonCopyable.h"
#include "ISystem.h"
#include "enkiTS/TaskScheduler.h"

namespace atlas
{

class AnimationSystem : public hq::NonCopyable,
        public ISystem,
        public enki::TaskSet
{
public:
    AnimationSystem();
    ~AnimationSystem();

    void update(float dt);

    // ISystem interface
public:
    void runSystem(entt::registry &registry, float dt) override;

    // ITaskSet interface
public:
    void ExecuteRange(enki::TaskSetPartition range_, uint32_t threadnum_) override;
};

} // atlas namespace
