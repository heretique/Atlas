#pragma once

#include <Hq/BasicTypes.h>

namespace atlas
{

// Used by serialization mostly,
// All components should publicly inherit from this
struct Component {};

struct Selected : public Component {
//    u8 _nil; // using only to bypass Entt ETO (empty type optimization)

//    template <class Serializer>
//    void Serialize(Serializer& serializer)
//    {
//    }
};

} // atlas namespace
