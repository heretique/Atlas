#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "Component.h"

namespace atlas {

struct MeshComponent: public Component
{
public:
    template <class Archive>
    void serialize(Archive & ar)
    {
    }
};

} // namespace Atlas



#endif // MESHCOMPONENT_H
