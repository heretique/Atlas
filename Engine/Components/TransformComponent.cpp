#include "TransformComponent.h"
#include "Core/Engine.h"

namespace atlas
{
const hq::math::Mat4x4& TransformComponent::world() const
{
    return _worldTransform;
}

hq::math::Mat4x4& TransformComponent::world()
{
    return _worldTransform;
}

const hq::math::Mat4x4& TransformComponent::local() const
{
    return _localTransform;
}

hq::math::Mat4x4& TransformComponent::local()
{
    return _localTransform;
}

}  // atlas namespace
