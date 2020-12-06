#include "TransformComponent.h"
#include "Core/Engine.h"

#include <rttr/registration>

namespace atlas
{
using namespace rttr;

RTTR_REGISTRATION
{
    registration::class_<TransformComponent>("TransformComponent")
        .constructor<>()
        .property("world", &TransformComponent::world)
        .property("local", &TransformComponent::local)
        .property_readonly("bbox", &TransformComponent::bbox);
}

}  // atlas namespace
