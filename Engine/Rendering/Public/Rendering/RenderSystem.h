#pragma once

#include "Hq/NonCopyable.h"
#include "Ecs/ISystem.h"

namespace atlas
{

class RenderSystem : public hq::NonCopyable, public ISystem
{
public:
    RenderSystem();
    ~RenderSystem();

    void render();

    // ISystem interface
public:
    void runSystem(entt::registry &registry, float dt) override;
};

} // atlas namespace
