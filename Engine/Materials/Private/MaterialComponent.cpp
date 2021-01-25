#include "Materials/MaterialComponent.h"
#include "Reflection/Reflection.h"

namespace atlas
{
REFLECT(MaterialComponent)
{
    using namespace rttr;

    registration::class_<MaterialComponent>("MaterialComponent")
        .constructor<>()
        .property("material", &MaterialComponent::material)(policy::ctor::as_object);
}

}  // namespace atlas
