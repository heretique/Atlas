#include "Core/TransformComponent.h"
#include "Reflection/Reflection.h"

namespace atlas
{
using namespace rttr;

RTTR_REGISTRATION
{
    registration::class_<TransformComponent>("TransformComponent")
        .constructor<>()
        .property("world", &TransformComponent::world)
        .property("local", &TransformComponent::local)
        .property_readonly("bbox", &TransformComponent::bbox)(policy::ctor::as_object);
}

}  // atlas namespace
