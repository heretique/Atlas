#include "Node.h"
#include "Core/Engine.h"
#include "NodeScript.h"
#include <wrenpp/Wren++.h>

namespace atlas
{
Node::Node(NodeType type, std::string name, NodePtr parent)
    : _type(type)
    , _name(name)
    , _nameHash(StringHash(name))
    , _parent(parent)
{
}

Node::~Node()
{
}

NodeType Node::type() const
{
    return _type;
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

Node* Node::parent() const
{
    return _parent.get();
}

NodePtr Node::parentPtr() const
{
    return _parent;
}

size_t Node::childCount() const
{
    return _children.size();
}

Node* Node::childAt(size_t index) const
{
    return _children[index].get();
}

NodePtr Node::childPtrAt(size_t index) const
{
    return _children[index];
}

void Node::attach(NodePtr parent)
{
    _parent = parent;
    onAttach();
    if (_nodeScript && _nodeScript->_onAttach)
        _nodeScript->_onAttach();
}

void Node::init()
{
    // TODOCM: add initialization code
    onInit();
    if (_nodeScript && _nodeScript->_onInit)
        _nodeScript->_onInit();
}

void Node::update(float dt)
{
    onUpdate(dt);
    if (_nodeScript && _nodeScript->_onUpdate)
        _nodeScript->_onUpdate(dt);
}

void Node::destroy()
{
    onDestroy();
    if (_nodeScript && _nodeScript->_onDestroy)
        _nodeScript->_onDestroy();
}

bool Node::canAttach(Node* /*node*/)
{
    return true;
}

void Node::onAttach()
{
}

void Node::onInit()
{
}

void Node::onUpdate(float /*dt*/)
{
}

void Node::onDestroy()
{
}

void Node::addChild(NodePtr child)
{
    auto found = std::find(_children.begin(), _children.end(), child);
    if (found == _children.end())
        _children.emplace_back(child);
}

bool Node::attachScript(WrenHandle* scriptInstance)
{
    _nodeScript = std::make_unique<NodeScript>(scriptInstance);
    return _nodeScript->initScript();
}

void wren::bindNode()
{
    Engine::vm()
        .beginModule("scripts/Scene")                                                              //
        .bindClass<Node, NodeType, std::string, NodePtr>("Node")                                   //
        .bindMethod<decltype(&Node::name), &Node::name>(false, "name")                             //
        .bindMethod<decltype(&Node::setName), &Node::setName>(false, "name=(_)")                   //
        .bindMethod<decltype(&Node::hash), &Node::hash>(false, "hash")                             //
        .bindMethod<decltype(&Node::enabled), &Node::enabled>(false, "enabled")                    //
        .bindMethod<decltype(&Node::enable), &Node::enable>(false, "enabled=(_)")                  //
        .bindMethod<decltype(&Node::parent), &Node::parent>(false, "parent")                       //
        .bindMethod<decltype(&Node::parentPtr), &Node::parentPtr>(false, "parentPtr")              //
        .bindMethod<decltype(&Node::childCount), &Node::childCount>(false, "childCount")           //
        .bindMethod<decltype(&Node::childAt), &Node::childAt>(false, "childAt(_)")                 //
        .bindMethod<decltype(&Node::childPtrAt), &Node::childPtrAt>(false, "childPtrAt(_)")        //
        .bindMethod<decltype(&Node::attachScript), &Node::attachScript>(false, "attachScript(_)")  //
        .endClass()
        .endModule();
}

}  // atlas namespace
