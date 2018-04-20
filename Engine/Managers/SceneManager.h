#pragma once
#include <string>

namespace atlas
{
class Node;
using NodePtr = std::shared_ptr<Node>;

class SceneManager
{
public:
    SceneManager();
    ~SceneManager();
    NodePtr root() const;
    NodePtr addNode(const std::string& name, NodePtr parent);

private:
    NodePtr _root;
};

namespace wren
{
    void bindSceneManager();
}
}  // atlas namespace
