#include "TransformComponent.h"
#include "Core/Engine.h"

namespace atlas
{
const math::Matrix& TransformComponent::world() const
{
    return _worldTransform;
}

math::Matrix& TransformComponent::world()
{
    return _worldTransform;
}

const math::Matrix& TransformComponent::local() const
{
    return _localTransform;
}

math::Matrix& TransformComponent::local()
{
    return _localTransform;
}

}  // atlas namespace
