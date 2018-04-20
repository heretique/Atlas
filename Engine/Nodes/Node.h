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
using NodeVec         = std::vector<Node*>;
using NodeSet         = std::unordered_set<Node*>;
using NodePtr         = std::shared_ptr<Node>;
using NodePtrVec      = std::vector<NodePtr>;
using NodePtrSet      = std::unordered_set<NodePtr>;
using ComponentPtr    = std::shared_ptr<Component>;
using ComponentPtrVec = std::vector<ComponentPtr>;
using NodeId          = uint64_t;

/// Base node class. Not optimal, Components held temporarily inside a vector
class Node
{
public:
    Node(std::string name, NodePtr parent = nullptr);
    ~Node();
    NodeId      id() const;
    std::string name() const;
    void setName(std::string name);
    StringHash hash() const;
    bool       enabled() const;
    void enable(bool enabled);
    NodePtr parent() const;
    void addChild(NodePtr child);
    ComponentPtr addComponent(StringHash component);
    ComponentPtr getComponent(StringHash component);

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
    NodePtrSet           _children;
};

namespace wren
{
    void bindNode();
}

}  // atlas namespace
