#pragma once

#include "Hq/Math/MathTypes.h"

namespace atlas
{
class TransformComponent
{
public:
    const hq::math::Mat4x4& world() const;
    hq::math::Mat4x4&       world();
    const hq::math::Mat4x4& local() const;
    hq::math::Mat4x4&       local();

private:
    hq::math::Mat4x4 _worldTransform {hq::math::Mat4x4::Identity};
    hq::math::Mat4x4 _localTransform {hq::math::Mat4x4::Identity};
};

}  // atlas namespace
