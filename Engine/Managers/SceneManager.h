#pragma once
#include "Core/NonCopyable.h"
#include "Core/StringHash.h"
#include "Nodes/Node.h"
#include "wrenpp/Wren++.h"

#include <string>
#include <unordered_map>

namespace atlas
{
using NodeRegistry   = std::unordered_map<NodeType, NodeFactoryFunc>;
using ScriptRegistry = std::unordered_map<std::string, wrenpp::Method>;

class Engine;
class SceneManager : NonCopyable
{
    friend class Engine;

public:
    SceneManager();
    ~SceneManager();
    void registerNodeType(NodeType nodeType, NodeFactoryFunc f);
    NodePtr root() const;
    NodePtr addNode(NodeType nodeType, const std::string& name, NodePtr parent);
    void removeNode(NodePtr node);
    bool reparentNode(NodePtr node, NodePtr parent);
    void update(float dt);
    void updateGUI();
    void attachScript(NodePtr node, std::string scriptName);

private:
    void updateNode(Node* node, float dt);
    void updateNodeGUI(Node* node);

private:
    NodePtr        _root;
    NodeRegistry   _nodeRegistry;
    ScriptRegistry _scriptRegistry;
};

namespace wren
{
    void bindSceneManager();
}
}  // atlas namespace
