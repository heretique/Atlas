#pragma once

#include <Hq/BasicTypes.h>
#include "Reflection/Registration.h"

namespace atlas
{
// Used by serialization mostly,
// All components should publicly inherit from this
class Component
{
    RTTR_ENABLE()
};

class Selected : public Component
{
    REFLECTABLE(Component)
};

}  // atlas namespace
