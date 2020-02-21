#pragma once

#include "Hq/Serializable.h"


namespace hq
{
    class Serializer;
}


namespace atlas
{
class Component : public ::hq::Serializable
{
// Do not create type id, we want to enforce on every class
// that inherits from component and don't want component
// instantiable
// CREATE_TYPEID(Component)
public:
    ~Component() override {}
    void Serialize(::hq::Serializer& serializer) override {};
};

}  // atlas namespace
