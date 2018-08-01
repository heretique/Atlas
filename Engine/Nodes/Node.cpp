#include "Node.h"
#include "Core/Engine.h"
#include "NodeScript.h"
#include <algorithm>

namespace atlas
{
Node::Node(std::string name, Node* parent)
    : _name(name)
    , _nameHash(StringHash(name))
    , _dirty(true)
    , _enabled(true)
    , _parent(parent)
{
}

Node::~Node()
{
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

bool Node::dirty() const
{
    return _dirty;
}

void Node::setDirty(bool dirty)
{
    _dirty = dirty;
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
    return _parent;
}

size_t Node::childCount() const
{
    return _children.size();
}

Node* Node::childAt(size_t index) const
{
    return _children[index];
}

void Node::attach(Node* parent)
{
    _parent = parent;
    onAttach();
    if (_nodeScript && _nodeScript->_onAttach)
        _nodeScript->_onAttach(this);
}

void Node::detach()
{
    onDetach();
    if (_nodeScript && _nodeScript->_onDetach)
        _nodeScript->_onDetach();
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

void Node::updateGUI()
{
    onGUI();
    if (_nodeScript && _nodeScript->_onGUI)
        _nodeScript->_onGUI();
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

void Node::onDetach()
{
}

void Node::onInit()
{
}

void Node::onUpdate(float /*dt*/)
{
}

void Node::onGUI()
{
}

void Node::onDestroy()
{
}

void Node::onSetScriptNode(wrenpp::Method& method)
{
    method(this);
}

Component* Node::getComponent(ComponentType type)
{
    auto compIt = _components.find(type);
    if (compIt != _components.end())
    {
        return compIt->second.get();
    }

    return nullptr;
}

void Node::addChild(Node* child)
{
    auto found = std::find(_children.begin(), _children.end(), child);
    if (found == _children.end())
        _children.emplace_back(child);
}

void Node::addComponent(ComponentPtr component)
{
    _components.insert(std::make_pair(component->type(), std::move(component)));
}

void Node::removeComponent(ComponentType type)
{
    auto it = _components.find(type);
    if (it != _components.end())
        _components.erase(it);
}

void Node::attachScript(WrenHandle* scriptInstance)
{
    _nodeScript = std::make_unique<NodeScript>(scriptInstance);
    _nodeScript->initScript(this);
    init();
}

void wren::bindNode()
{
    Engine::vm()
        .beginModule("main")                                                                       //
        .bindClass<Node, std::string, Node*>("Node")                                               //
        .bindMethod<decltype(&Node::name), &Node::name>(false, "name")                             //
        .bindMethod<decltype(&Node::setName), &Node::setName>(false, "name=(_)")                   //
        .bindMethod<decltype(&Node::hash), &Node::hash>(false, "hash")                             //
        .bindMethod<decltype(&Node::enabled), &Node::enabled>(false, "enabled")                    //
        .bindMethod<decltype(&Node::enable), &Node::enable>(false, "enabled=(_)")                  //
        .bindMethod<decltype(&Node::parent), &Node::parent>(false, "parent")                       //
        .bindMethod<decltype(&Node::childCount), &Node::childCount>(false, "childCount")           //
        .bindMethod<decltype(&Node::childAt), &Node::childAt>(false, "childAt(_)")                 //
        .bindMethod<decltype(&Node::attachScript), &Node::attachScript>(false, "attachScript(_)")  //
        .bindMethod<decltype(&Node::getComponent), &Node::getComponent>(false, "getComponent(_)")  //
        .endClass()
        .endModule();

    Engine::wrenModule() +=
        "foreign class Node {\n"
        "    foreign name\n"
        "    foreign name=(rhs)\n"
        "    foreign hash\n"
        "    foreign enabled\n"
        "    foreign enabled=(rhs)\n"
        "    foreign parent // parent: Node\n"
        "    foreign childCount\n"
        "    foreign childAt(index) // return: Node\n"
        "    foreign attachScript(script)\n"
        "    foreign getComponent(componentType)\n"
        "}\n";
}

}  // atlas namespace
