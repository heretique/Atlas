#include "Node.h"
#include "Core/Engine.h"
#include <wrenpp/Wren++.h>

namespace atlas
{
Node::Node(std::string name, Node* parent)
    : _name(name)
    , _nameHash(StringHash(name))
    , _parent(parent)
{
}

NodeId Node::id() const
{
    return _id;
}

std::string Node::name() const
{
    return _name;
}

void Node::setName(std::string name)
{
    _name = name;
}

StringHash Node::hash() const
{
    return _nameHash;
}

bool Node::enabled() const
{
    return _enabled;
}

void Node::enable(bool enabled)
{
    _enabled = enabled;
}

NodePtr Node::parent() const
{
    return _parent;
}

void wren::bindNode()
{
    Engine::vm()
        .beginModule("scripts/Scene")                                              //
        .bindClass<Node, std::string, NodePtr>("Node")                             //
        .bindMethod<decltype(&Node::id), &Node::id>(false, "id")                   //
        .bindMethod<decltype(&Node::name), &Node::name>(false, "name")             //
        .bindMethod<decltype(&Node::setName), &Node::setName>(false, "name=(_)")   //
        .bindMethod<decltype(&Node::hash), &Node::hash>(false, "hash")             //
        .bindMethod<decltype(&Node::enabled), &Node::enabled>(false, "enabled")    //
        .bindMethod<decltype(&Node::enable), &Node::enable>(false, "enabled=(_)")  //
        .bindMethod<decltype(&Node::parent), &Node::parent>(false, "parent")       //

        .endClass()
        .endModule();
}

}  // atlas namespace
