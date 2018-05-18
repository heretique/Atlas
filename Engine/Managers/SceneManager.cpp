#include "SceneManager.h"

#include "Core/Engine.h"
#include "Nodes/Node.h"
#include "wrenpp/Wren++.h"

namespace atlas
{
SceneManager::SceneManager()
{
    _root = std::make_shared<Node>("root");
}

SceneManager::~SceneManager()
{
}

NodePtr SceneManager::root() const
{
    return _root;
}

NodePtr SceneManager::addNode(const std::string& name, NodePtr parent)
{
    assert(parent != nullptr);
    NodePtr node = std::make_shared<Node>(name, parent);
    parent->addChild(node);
    return node;
}

void wren::bindSceneManager()
{
    Engine::vm()
        .beginModule("scripts/Scene")                                                                 //
        .bindClass<NodePtr>("NodePtr")                                                                //
        .endClass()                                                                                   //
        .bindClass<SceneManager>("SceneManager")                                                      //
        .bindMethod<decltype(&SceneManager::root), &SceneManager::root>(false, "root")                //
        .bindMethod<decltype(&SceneManager::addNode), &SceneManager::addNode>(false, "addNode(_,_)")  //
        .endClass()                                                                                   //
        .endModule();
}

}  // atlas namespace
