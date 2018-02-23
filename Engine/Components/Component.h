#pragma once

namespace atlas
{
struct Component
{
};

template <class Archive>
void serialize(Archive& ar, Component&)
{
    ar();
}

}  // atlas namespace
