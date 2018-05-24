#include "NodeScript.h"
#include "Core/Engine.h"

#include <spdlog/spdlog.h>

namespace atlas
{
NodeScript::NodeScript(WrenHandle* handle)
    : _handle(handle)
{
}

NodeScript::~NodeScript()
{
}

bool NodeScript::initScript()
{
    wrenpp::VM& vm = Engine::vm();

    _onAttach = vm.method(_handle, "onAttach()");
    _onInit   = vm.method(_handle, "onInit()");
    _onUpdate = vm.method(_handle, "onUpdate(_)");

    return true;
}

bool NodeScript::isValid() const
{
    return true;
}
}
