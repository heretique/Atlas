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
    _nodes.emplace_back(std::make_unique<Node>("root"));
    _root = _nodes.back().get();
}

SceneManager::~SceneManager()
{
}

void SceneManager::registerComponentType(ComponentType componentType, std::string componentTypeName,
                                         ComponentFactoryFunc f)
{
    if (_componentRegistry.find(componentType) != _componentRegistry.end())
    {
        Engine::log().warn("Component type '{}', already registered", componentName(componentTypeName));
        return;
    }

    _componentRegistry.insert(std::make_pair(componentType, ComponentRegistryEntry{componentTypeName, f}));
}

Node* SceneManager::root() const
{
    return _root;
}

Node* SceneManager::addNode(const std::string& name, Node* parent)
{
    assert(parent);

    _nodes.emplace_back(std::make_unique<Node>(name, parent));
    Node* node = _nodes.back().get();
    parent->addChild(node);

    return node;
}

void SceneManager::removeNode(Node* node)
{
}

bool SceneManager::reparentNode(Node* node, Node* parent)
{
    return false;
}

Component* SceneManager::addComponent(Node* node, ComponentType type)
{
    assert(node != nullptr);
    Component* component = node->getComponent(type);
    if (component != nullptr)
    {
        Engine::log().warn("Node: \"{}\" already has a component of type: \"{}\"", node->name(), componentName(type));
        return component;
    }

    auto compoIt = _componentRegistry.find(type);
    if (compoIt == _componentRegistry.end())
    {
        Engine::log().warn("Component type: \"{}\" not registered, when trying to add to node: \"{}\".",
                           componentName(type), node->name());
        return nullptr;
    }

    ComponentPtr compPtr = compoIt->second.factory();
    component            = compPtr.get();
    node->addComponent(std::move(compPtr));

    return component;
}

void SceneManager::removeComponent(Node* node, ComponentType type)
{
    assert(node != nullptr);
    node->removeComponent(type);
}

void SceneManager::update(float dt)
{
    updateNode(_root, dt);
}

void SceneManager::updateGUI()
{
    updateNodeGUI(_root);
}

void SceneManager::attachScript(Node* node, std::string scriptName)
{
    auto scriptIt = _scriptRegistry.find(scriptName);
    if (scriptIt == _scriptRegistry.end())
    {
        std::string scriptClass = scriptName.substr(scriptName.find_last_of("/") + 1);
        scriptName += ".wren";
        auto scriptAsset = Engine::assets().getAsset<ScriptAsset>(scriptName);
        if (scriptAsset != nullptr)
        {
            if (wrenpp::Result::Success == Engine::vm().executeString("main", scriptAsset->script()))
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

const std::string SceneManager::componentName(ComponentType type) const
{
    auto compIt = _componentRegistry.find(type);
    if (compIt != _componentRegistry.end())
    {
        return compIt->second.name;
    }

    return "";
}

void wren::bindSceneManager()
{
    Engine::vm()
        .beginModule("main")                                                                                         //
        .bindClass<SceneManager>("SceneManager")                                                                     //
        .bindMethod<decltype(&SceneManager::root), &SceneManager::root>(false, "root")                               //
        .bindMethod<decltype(&SceneManager::addNode), &SceneManager::addNode>(false, "addNode(_,_,_)")               //
        .bindMethod<decltype(&SceneManager::removeNode), &SceneManager::removeNode>(false, "removeNode(_)")          //
        .bindMethod<decltype(&SceneManager::reparentNode), &SceneManager::reparentNode>(false, "reparentNode(_,_)")  //
        .bindMethod<decltype(&SceneManager::addComponent), &SceneManager::addComponent>(false, "addComponent(_,_)")  //
        .bindMethod<decltype(&SceneManager::removeComponent), &SceneManager::removeComponent>(
            false, "removeComponent(_,_)")  //
        .endClass()                         //
        .endModule();

    Engine::wrenModule() +=
        "foreign class SceneManager {\n"
        "    foreign root\n"
        "    foreign addNode(type, name, parent)\n"
        "    foreign removeNode(node)\n"
        "    foreign reparentNode(node, newParent)\n"
        "    foreign addComponent(node, componentType)\n"
        "    foreign removeComponent(node, componentType)\n"
        "\n"
        "    attachScript(node, script) {\n"
        "        node.attachScript(script)\n"
        "        script.node = node\n"
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
