#pragma once

#include <Hq/BasicTypes.h>
#include <rttr/rttr_enable.h>

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
    RTTR_ENABLE(Component)
};

}  // atlas namespace
