#include "SceneManager.h"

#include "Core/Engine.h"
#include "Nodes/Node.h"
#include "wrenpp/Wren++.h"
#include <spdlog/spdlog.h>

namespace atlas
{
SceneManager::SceneManager()
{
    _root = std::make_shared<Node>(NodeTypes::Root, "root");
}

SceneManager::~SceneManager()
{
}

void SceneManager::registerNodeType(NodeType nodeType, NodeFactoryFunc f)
{
    if (_registry.find(nodeType) != _registry.end())
    {
        Engine::log().warn("Node type '{}', already registered", NodeTypes::toName(nodeType));
        return;
    }

    _registry.insert(make_pair(nodeType, f));
}

NodePtr SceneManager::root() const
{
    return _root;
}

NodePtr SceneManager::addNode(NodeType nodeType, const std::string& name, NodePtr parent)
{
    assert(parent);

    NodePtr node = nullptr;
    auto    it   = _registry.find(nodeType);
    if (it == _registry.end())
    {
        Engine::log().error("Node type not registered: '{}'", NodeTypes::toName(nodeType));
        return nullptr;
    }

    node = it->second(name);
    assert(node);

    if (node->canAttach(parent.get()))
    {
        node->attach(parent);
        parent->addChild(node);
    }
    else
        return nullptr;

    return node;
}

void SceneManager::removeNode(NodePtr node)
{
}

bool SceneManager::reparentNode(NodePtr node, NodePtr parent)
{
    return false;
}

bool SceneManager::attachNodeScript(NodePtr node, const std::string& moduleName, const std::string& className)
{
}

void wren::bindSceneManager()
{
    Engine::vm()
        .beginModule("scripts/Scene")                                                                   //
        .bindClass<NodePtr>("NodePtr")                                                                  //
        .endClass()                                                                                     //
        .bindClass<SceneManager>("SceneManager")                                                        //
        .bindMethod<decltype(&SceneManager::root), &SceneManager::root>(false, "root")                  //
        .bindMethod<decltype(&SceneManager::addNode), &SceneManager::addNode>(false, "addNode(_,_,_)")  //
        .endClass()                                                                                     //
        .endModule();
}

}  // atlas namespace
