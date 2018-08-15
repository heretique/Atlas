#pragma once

#include "Components/Types.h"

namespace atlas
{
class Node;
class SceneManager;
class Component
{
    friend class SceneManager;

public:
    virtual ~Component();
    virtual void onUpdate(float /*dt*/);
    virtual void onLateUpdate(float /*dt*/);

    ComponentType type() const;
    Node*         node() const;

protected:
    Component(ComponentType type);
    void setNode(Node* node);

protected:
    ComponentType _type;
    Node*         _node;
};

}  // atlas namespace
