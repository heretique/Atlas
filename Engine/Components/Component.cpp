#include "Component.h"

namespace atlas
{
Component::~Component() {}

void Component::onUpdate(float) {}

void Component::onLateUpdate(float) {}

ComponentType Component::type() const
{
    return _type;
}

Node* Component::node() const
{
    return _node;
}

Component::Component(ComponentType type)
    : _type(type)
    , _node(nullptr)
{
}

void Component::setNode(Node* node)
{
    _node = node;
}
}
