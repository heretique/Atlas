#pragma once

#include "Core/StringHash.h"
#include "Core/Types.h"

#include <functional>
#include <memory>

namespace wrenpp
{
class VM;
}

namespace atlas
{
class Node;
class NodeScript;

using NodeType       = StringHash;
using NodeVec        = std::vector<Node*>;
using NodePtr        = std::shared_ptr<Node>;
using NodeRef        = std::weak_ptr<Node>;
using NodePtrVec     = std::vector<NodePtr>;
using NodeScriptUPtr = std::unique_ptr<NodeScript>;

using NodeFactoryFunc = std::function<NodePtr(const std::string& nodeName)>;
using NodeBindFunc    = std::function<void(wrenpp::VM& vm)>;

namespace NodeTypeNames
{
    static const std::string Undefined{"Undefined"};
    static const std::string Root{"Root"};
    static const std::string Spatial{"Spatial"};
    static const std::string Camera{"Camera"};
}

namespace NodeTypes
{
    static const NodeType Undefined{NodeTypeNames::Undefined};
    static const NodeType Root{NodeTypeNames::Root};
    static const NodeType Spatial{NodeTypeNames::Spatial};

    const std::string& toName(NodeType type);
}

namespace wren
{
    void bindNodeTypes();
}
}
