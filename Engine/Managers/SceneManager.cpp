#include "SceneManager.h"

#include "Assets/Script.h"
#include "Core/Engine.h"
#include "Managers/AssetManager.h"
#include "Nodes/Node.h"
#include "Nodes/NodeScript.h"
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
    if (_nodeRegistry.find(nodeType) != _nodeRegistry.end())
    {
        Engine::log().warn("Node type '{}', already registered", NodeTypes::toName(nodeType));
        return;
    }

    _nodeRegistry.insert(make_pair(nodeType, f));
}

NodePtr SceneManager::root() const
{
    return _root;
}

NodePtr SceneManager::addNode(NodeType nodeType, const std::string& name, NodePtr parent)
{
    assert(parent);

    NodePtr node = nullptr;
    auto    it   = _nodeRegistry.find(nodeType);
    if (it == _nodeRegistry.end())
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

void SceneManager::update(float dt)
{
    updateNode(_root.get(), dt);
}

void SceneManager::updateGUI()
{
    updateNodeGUI(_root.get());
}

void SceneManager::attachScript(NodePtr node, std::string scriptName)
{
    auto scriptIt = _scriptRegistry.find(scriptName);
    if (scriptIt == _scriptRegistry.end())
    {
        std::string scriptClass = scriptName.substr(scriptName.find_last_of("/") + 1);
        scriptName += ".wren";
        auto scriptAsset = Engine::assets().getAsset<ScriptAsset>(scriptName);
        if (scriptAsset != nullptr)
        {
            if (wrenpp::Result::Success == Engine::vm().executeString(scriptAsset->script()))
            {
                wrenEnsureSlots(Engine::vm().ptr(), 1);
                wrenGetVariable(Engine::vm().ptr(), "main", scriptClass.c_str(), 0);
                WrenHandle*    variable    = wrenGetSlotHandle(Engine::vm().ptr(), 0);
                wrenpp::Method constructor = Engine::vm().method(variable, "new()");
                constructor();
                _scriptRegistry.insert(std::make_pair(scriptName, std::move(constructor)));
            }
        }
    }
    else
    {
        scriptIt->second();
    }
    WrenHandle* handle = wrenGetSlotHandle(Engine::vm().ptr(), 0);
    node->attachScript(handle);
}

void SceneManager::updateNode(Node* node, float dt)
{
    node->update(dt);
    size_t childCount = node->childCount();
    while (childCount && node->_enabled)
    {
        updateNode(node->childAt(childCount - 1), dt);
        --childCount;
    }
    node->setDirty(false);
}

void SceneManager::updateNodeGUI(Node* node)
{
    node->updateGUI();
    size_t childCount = node->childCount();
    while (childCount && node->_enabled)
    {
        updateNodeGUI(node->childAt(childCount - 1));
        --childCount;
    }
}

void wren::bindSceneManager()
{
    Engine::vm()
        .beginModule("main")                                                                                         //
        .bindClass<NodePtr>("NodePtr")                                                                               //
        .endClass()                                                                                                  //
        .bindClass<SceneManager>("SceneManager")                                                                     //
        .bindMethod<decltype(&SceneManager::root), &SceneManager::root>(false, "root")                               //
        .bindMethod<decltype(&SceneManager::addNode), &SceneManager::addNode>(false, "addNode(_,_,_)")               //
        .bindMethod<decltype(&SceneManager::removeNode), &SceneManager::removeNode>(false, "removeNode(_)")          //
        .bindMethod<decltype(&SceneManager::reparentNode), &SceneManager::reparentNode>(false, "reparentNode(_,_)")  //
        .endClass()                                                                                                  //
        .endModule();

    Engine::wrenModule() +=
        "foreign class SceneManager {\n"
        "    foreign root\n"
        "    foreign addNode(type, name, parent)\n"
        "    foreign removeNode(node)\n"
        "    foreign reparentNode(node, newParent)\n"
        "\n"
        "    attachScript(node, script) {\n"
        "        node.get.attachScript(script)\n"
        "        script.node = node.get\n"
        "        script.onInit()\n"
        "    }\n"
        "}\n"
        "\n"
        "\n"
        "class NodeScript {\n"
        "    node { _node }\n"
        "    node=(rhs) { _node = rhs }\n"
        "    onInit() {}\n"
        "    onAttach() {}\n"
        "    onDetach() {}\n"
        "    onUpdate(dt) {}\n"
        "    onGUI() {}\n"
        "    onDestoy() {}\n"
        "}\n";
}

}  // atlas namespace
