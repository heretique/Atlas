#include "Node.h"

namespace atlas
{
Node::Node(std::string name, NodePtr parent)
    : _name(name)
    , _nameHash(StringHash(name))
    , _parent(parent)
{
}

}  // atlas namespace
