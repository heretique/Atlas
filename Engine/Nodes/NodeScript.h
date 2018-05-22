#pragma once

#include "wrenpp/Wren++.h"
#include <memory>

namespace atlas
{
class Node;
class NodeScript;
using NodescriptUPtr = std::unique_ptr<NodeScript>;

class NodeScript
{
    friend class Node;

public:
    NodeScript(const std::string& moduleName, const std::string& className);
    virtual ~NodeScript();

private:
    wrenpp::Method _new;
    wrenpp::Method _onAttach;
    wrenpp::Method _onInit;
    wrenpp::Method _onUpdate;
    wrenpp::Method _onDestroy;
};
}
