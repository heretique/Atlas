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
    NodeScript(WrenHandle* handle);
    ~NodeScript();
    bool initScript(Node* node);
    bool isValid() const;

private:
    WrenHandle*    _handle;
    wrenpp::Method _nodeSetter;
    wrenpp::Method _onAttach;
    wrenpp::Method _onDetach;
    wrenpp::Method _onInit;
    wrenpp::Method _onUpdate;
    wrenpp::Method _onGUI;
    wrenpp::Method _onDestroy;
};
}
