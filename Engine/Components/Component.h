#pragma once

#include "Components/Types.h"

namespace atlas
{
class Component
{
public:
    virtual ~Component()
    {
    }

    ComponentType type() const
    {
        return _type;
    }

protected:
    Component(ComponentType type)
        : _type(type)
    {
    }
    ComponentType _type;
};

}  // atlas namespace
