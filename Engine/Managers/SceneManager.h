#pragma once
#include "Components/Component.h"
#include "Core/NonCopyable.h"
#include "Core/StringHash.h"
#include "Nodes/Node.h"
#include "wrenpp/Wren++.h"

#include <string>
#include <unordered_map>

namespace atlas
{
class Engine;
class SceneManager : NonCopyable
{
    friend class Engine;

public:
    SceneManager();
    ~SceneManager();
    void registerComponentType(ComponentType componentType, std::string componentTypeName, ComponentFactoryFunc f);
    Node* root() const;
    Node* addNode(const std::string& name, Node* parent);
    void removeNode(Node* node);
    bool reparentNode(Node* node, Node* parent);
    Component* addComponent(Node* node, ComponentType type);
    void removeComponent(Node* node, ComponentType type);
    void update(float dt);
    void updateGUI();
    void attachScript(Node* node, std::string scriptName);

private:
    void updateNode(Node* node, float dt);
    void updateNodeGUI(Node* node);
    const std::string componentName(ComponentType type) const;

private:
    struct ComponentRegistryEntry
    {
        std::string          name;
        ComponentFactoryFunc factory;
    };
    using ComponentRegistry = std::unordered_map<ComponentType, ComponentRegistryEntry>;
    using ScriptRegistry    = std::unordered_map<std::string, wrenpp::Method>;

    Node*                               _root;
    ComponentRegistry                   _componentRegistry;
    ScriptRegistry                      _scriptRegistry;
    std::vector<std::unique_ptr<Node> > _nodes;
};

namespace wren
{
    void bindSceneManager();
}
}  // atlas namespace
