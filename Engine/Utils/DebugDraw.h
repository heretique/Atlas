#pragma once

#include <Hq/Math/MathTypes.h>
#include <Assets/Types.h>

namespace atlas
{

struct DebugDrawImpl;

class DebugDraw
{
public:
    DebugDraw();
    ~DebugDraw();

    void initialize();

    void begin();
    void end();

    void drawBox3(const hq::math::Box3& box, const hq::math::Mat4x4& transform, const uint32_t color);

private:
    std::unique_ptr<DebugDrawImpl> _impl;
};

} // atlas namespace
