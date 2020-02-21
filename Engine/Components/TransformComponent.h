#pragma once

#include "Component.h"
#include "Hq/Math/MathTypes.h"

namespace atlas
{
class TransformComponent : public Component
{
    DECLARE_SUPER(Component)
    CREATE_TYPEID(TransformComponent)
public:
    const hq::math::Mat4x4& world() const;
    hq::math::Mat4x4&       world();
    const hq::math::Mat4x4& local() const;
    hq::math::Mat4x4&       local();

private:
    hq::math::Mat4x4 _worldTransform {hq::math::Mat4x4::Identity};
    hq::math::Mat4x4 _localTransform {hq::math::Mat4x4::Identity};
};
REGISTER_SERIALIZABLE_TYPE(TransformComponent)

}  // atlas namespace
