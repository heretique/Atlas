#pragma once
#include "Core/NonCopyable.h"
#include "Core/StringHash.h"
#include "Nodes/Node.h"

#include <string>
#include <unordered_map>

namespace atlas
{
using NodeRegistry = std::unordered_map<NodeType, NodeFactoryFunc>;

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

private:
    void updateNode(Node* node, float dt);

private:
    NodePtr      _root;
    NodeRegistry _registry;
};

namespace wren
{
    void bindSceneManager();
}
}  // atlas namespace
