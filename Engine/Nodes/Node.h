#pragma once

#include "Components/Component.h"
#include "Core/StringHash.h"
#include "Math/BoundingBox.h"
#include "Math/BoundingSphere.h"
#include <memory>
#include <cereal/cereal.hpp>

namespace atlas
{
class Node;
using NodePtr         = std::shared_ptr<Node>;
using NodePtrVec      = std::vector<NodePtr>;
using ComponentPtr    = std::shared_ptr<Component>;
using ComponentPtrVec = std::vector<ComponentPtr>;
using NodeId          = uint64_t;

class Node
{
public:
    Node(std::string name, NodePtr parent = nullptr);

    template <typename Archive>
    void serialize(Archive& ar)
    {
        ar(                           //
            CEREAL_NVP(_id),          //
            CEREAL_NVP(_name),        //
            CEREAL_NVP(_enabled),     //
            CEREAL_NVP(_components),  //
            CEREAL_NVP(_parent),      //
            CEREAL_NVP(_children)     //
            );
    }

private:
    NodeId               _id;
    std::string          _name;
    StringHash           _nameHash;
    bool                 _enabled;
    math::BoundingBox    _bbox;
    math::BoundingSphere _bsphere;
    ComponentPtrVec      _components;
    NodePtr              _parent;
    NodePtrVec           _children;
};
}  // atlas namespace
