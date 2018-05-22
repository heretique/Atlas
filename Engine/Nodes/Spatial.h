#pragma once

#include "Node.h"

namespace atlas
{
class SpatialNode : public Node
{
public:
    static NodePtr factoryFunc(const std::string& name)
    {
        return std::make_shared<SpatialNode>(NodeTypes::Spatial, name);
    }

    SpatialNode(NodeType nodeType, const std::string& name);
};
}
