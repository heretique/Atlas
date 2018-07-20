#pragma once

#include "Components/Component.h"
#include "Core/StringHash.h"
#include "Core/Types.h"
#include "Math/BoundingBox.h"
#include "Math/BoundingSphere.h"

#include <memory>
#include <wrenpp/Wren++.h>

struct WrenHandle;

namespace atlas
{
class Node;
class NodeScript;
class SceneManager;

using NodeVec    = std::vector<Node*>;
using Components = std::unordered_map<ComponentType, ComponentPtr>;

/// Base node class. Not optimal, Components for now inside a vector
class Node
{
    friend class SceneManager;

public:
    Node(std::string name, Node* parent = nullptr);
    virtual ~Node();

    std::string name() const;
    void setName(std::string name);

    StringHash hash() const;

    bool dirty() const;
    void setDirty(bool dirty = true);

    bool enabled() const;
    void enable(bool enabled = true);

    Node*  parent() const;
    size_t childCount() const;
    Node* childAt(size_t index) const;

    void attach(Node* parent);
    void detach();
    void init();
    void update(float dt);
    void updateGUI();
    void destroy();
    void attachScript(WrenHandle* scriptInstance);
    // override this in every subclass to set the proper node class
    virtual void onSetScriptNode(wrenpp::Method& method);

    Component* getComponent(ComponentType type);

protected:
    virtual bool canAttach(Node* node);
    virtual void onAttach();
    virtual void onDetach();
    virtual void onInit();
    virtual void onUpdate(float dt);
    virtual void onGUI();
    virtual void onDestroy();

private:
    void addChild(Node* child);
    void removeChild(Node* child);
    NodeVec& children();
    void addComponent(ComponentPtr component);
    void removeComponent(ComponentType type);

private:
    std::string                 _name;
    StringHash                  _nameHash;
    bool                        _dirty;
    bool                        _enabled;
    Node*                       _parent;
    NodeVec                     _children;
    Components                  _components;
    std::unique_ptr<NodeScript> _nodeScript;
};

namespace wren
{
    void bindNode();
}

}  // atlas namespace
