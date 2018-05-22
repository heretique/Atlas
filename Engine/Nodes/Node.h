#pragma once

#include "Components/Component.h"
#include "Core/StringHash.h"
#include "Core/Types.h"
#include "Math/BoundingBox.h"
#include "Math/BoundingSphere.h"
#include "Types.h"
#include <memory>
#include <cereal/cereal.hpp>

namespace atlas
{
class Node;
class SceneManager;

/// Base node class. Not optimal, Components for now inside a vector
class Node
{
    friend class SceneManager;

public:
    Node(NodeType type, std::string name, NodePtr parent = nullptr);
    virtual ~Node();

    NodeType type() const;

    std::string name() const;
    void setName(std::string name);

    StringHash hash() const;

    bool enabled() const;
    void enable(bool enabled);

    Node*   parent() const;
    NodePtr parentPtr() const;
    size_t  childCount() const;
    Node* childAt(size_t index) const;
    NodePtr childPtrAt(size_t index) const;

    void attach(NodePtr parent);
    void init();
    void update(float dt);
    void destroy();

    template <typename Archive>
    void serialize(Archive& ar)
    {
        ar(                           //
            CEREAL_NVP(_type),        //
            CEREAL_NVP(_name),        //
            CEREAL_NVP(_nameHash),    //
            CEREAL_NVP(_enabled),     //
            CEREAL_NVP(_bbox),        //
            CEREAL_NVP(_bsphere),     //
            CEREAL_NVP(_nodeScript),  //
            CEREAL_NVP(_parent),      //
            CEREAL_NVP(_children)     //
            );
    }

protected:
    virtual bool canAttach(Node* node);
    virtual void onAttach();
    virtual void onInit();
    virtual void onUpdate(float dt);
    virtual void onDestroy();

private:
    void addChild(NodePtr child);
    void removeChild(Node* child);
    NodePtrVec children();
    bool attachScript(NodeScriptUPtr nodeScript);

private:
    NodeType             _type;
    std::string          _name;
    StringHash           _nameHash;
    bool                 _enabled;
    math::BoundingBox    _bbox;
    math::BoundingSphere _bsphere;
    NodePtr              _parent;
    NodePtrVec           _children;
    NodeScriptUPtr       _nodeScript;
};

namespace wren
{
    void bindNode();
}

}  // atlas namespace
