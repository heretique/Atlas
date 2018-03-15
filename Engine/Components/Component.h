#pragma once

namespace atlas
{
class Component
{
public:
    virtual ~Component()
    {
    }
    template <class Archive>
    void serialize(Archive&)
    {
    }

private:
};

}  // atlas namespace
