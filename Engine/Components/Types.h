#pragma once

#include "Core/StringHash.h"

#include <functional>
#include <memory>

namespace atlas
{
class Component;

using ComponentType        = StringHash;
using ComponentPtr         = std::unique_ptr<Component>;
using ComponentFactoryFunc = std::function<ComponentPtr()>;

namespace ComponentNames
{
    static const std::string Undefined{"Undefined"};
    static const std::string Camera{"Camera"};
    static const std::string Transform{"Transform"};
    static const std::string Mesh{"Mesh"};
    static const std::string Material{"Material"};
}

namespace ComponentTypes
{
    static const ComponentType Undefined{ComponentNames::Undefined};
    static const ComponentType Camera{ComponentNames::Camera};
    static const ComponentType Transform{ComponentNames::Transform};
    static const ComponentType Mesh{ComponentNames::Mesh};
    static const ComponentType Material{ComponentNames::Material};
}
}
