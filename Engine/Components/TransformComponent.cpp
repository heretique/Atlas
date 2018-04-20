#include "TransformComponent.h"
#include "Core/SerializationArchives.h"

namespace atlas
{
const math::Transform& TransformComponent::world() const
{
    return _worldTransform;
}

math::Transform& TransformComponent::world()
{
    return _worldTransform;
}

const math::Transform& TransformComponent::local() const
{
    return _localTransform;
}

math::Transform& TransformComponent::local()
{
    return _localTransform;
}

}  // atlas namespace
CEREAL_REGISTER_TYPE(atlas::TransformComponent)
