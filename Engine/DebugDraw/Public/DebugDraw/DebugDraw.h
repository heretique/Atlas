#pragma once

#include <Hq/Math/MathTypes.h>
#include <Assets/Types.h>

namespace atlas
{
class DebugDraw
{
public:
    static void initialize();

    static void begin();
    static void end();

    static void drawBox3(const hq::math::Box3& box, const uint32_t color);
    static void drawBox3(const hq::math::Box3& box, const hq::math::Mat4x4& transform, const uint32_t color);
    static void drawFrustum(const hq::math::Frustum& frustum, const uint32_t color);
    static void drawFrustum(const hq::math::Frustum& frustum, const hq::math::Mat4x4& transform, const uint32_t color);
    static void drawRay3(const hq::math::Ray3& ray, const float length, const uint32_t color);
    static void drawRay3(const hq::math::Ray3& ray, const hq::math::Mat4x4& transform, const float length,
                         const uint32_t color);
};

}  // atlas namespace
