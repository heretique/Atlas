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

    _onAttach  = vm.method(_handle, "onAttach()");
    _onDetach  = vm.method(_handle, "onDetach()");
    _onInit    = vm.method(_handle, "onInit()");
    _onUpdate  = vm.method(_handle, "onUpdate(_)");
    _onGUI     = vm.method(_handle, "onGUI()");
    _onDestroy = vm.method(_handle, "onDestroy()");

    return true;
}

bool NodeScript::isValid() const
{
    return _handle != nullptr;
}
}
